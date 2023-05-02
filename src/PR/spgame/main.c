/**************************************************************************
 *                                                                        *
 *               Copyright (C) 1995, Silicon Graphics, Inc.               *
 *                                                                        *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright  law.  They  may not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *                                                                        *
 *************************************************************************/

/*
 * File:	onetri.c
 * Create Date:	Mon Apr 17 11:45:57 PDT 1995
 *
 * VERY simple app, draws a couple triangles spinning.
 *
 */

#include <ultra64.h>
#include <PR/ramrom.h>	/* needed for argument passing into the app */
#include <assert.h>

#include "main.h"

/*
 * Symbol genererated by "makerom" to indicate the end of the code segment
 * in virtual (and physical) memory
 */
extern char _codeSegmentEnd[];

/*
 * Symbols generated by "makerom" to tell us where the static segment is
 * in ROM.
 */
extern char _staticSegmentRomStart[], _staticSegmentRomEnd[];

/*
 * Stacks for the threads as well as message queues for synchronization
 * This stack is ridiculously large, and could also be reclaimed once
 * the main thread is started.
 */
u64	bootStack[STACKSIZE/sizeof(u64)];

static void	idle(void *);
static void	mainproc(void *);

static OSThread	idleThread;
static u64	idleThreadStack[STACKSIZE/sizeof(u64)];

static OSThread	mainThread;
static u64	mainThreadStack[STACKSIZE/sizeof(u64)];

static OSThread	rmonThread;
static u64      rmonStack[RMON_STACKSIZE/sizeof(u64)];

/* this number (the depth of the message queue) needs to be equal
 * to the maximum number of possible overlapping PI requests.
 * For this app, 1 or 2 is probably plenty, other apps might
 * require a lot more.
 */
#define NUM_PI_MSGS     8

static OSMesg PiMessages[NUM_PI_MSGS];
static OSMesgQueue PiMessageQ;

OSMesgQueue	dmaMessageQ, rspMessageQ, rdpMessageQ, retraceMessageQ;
OSMesg		dmaMessageBuf, rspMessageBuf, rdpMessageBuf, retraceMessageBuf;
OSIoMesg	dmaIOMessageBuf;	/* see man page to understand this */

/*
 * Dynamic data.
 */
Dynamic dynamic;

/*
 * necessary for RSP tasks:
 */
u64 dram_stack[SP_DRAM_STACK_SIZE64];	/* used for matrix stack */
u64 rdp_output_len;			/* RSP writes back size of RDP data */
u64 rdp_output[4096*16];		/* RSP writes back RDP data */
    
/*
 * Task descriptor.
 */
OSTask	tlist =
{
    M_GFXTASK,			/* task type */
    OS_TASK_DP_WAIT,		/* task flags */
    NULL,			/* boot ucode pointer (fill in later) */
    0,				/* boot ucode size (fill in later) */
    NULL,			/* task ucode pointer (fill in later) */
    SP_UCODE_SIZE,		/* task ucode size */
    NULL,			/* task ucode data pointer (fill in later) */
    SP_UCODE_DATA_SIZE,		/* task ucode data size */
    &(dram_stack[0]),		/* task dram stack pointer */
    SP_DRAM_STACK_SIZE8,	/* task dram stack size */
    &(rdp_output[0]),		/* task output buffer ptr (not always used) */
    &rdp_output_len,		/* task output buffer size ptr */
    NULL,			/* task data pointer (fill in later) */
    0,				/* task data size (fill in later) */
    NULL,			/* task yield buffer ptr (not used here) */
    0				/* task yield buffer size (not used here) */
};

Gfx		*glistp;	/* global for test case procs */
    
/*
 * global variables
 */
static float	theta = 0.0;
static int	rdp_DRAM_io = 0;
static int	do_texture = 0;
static int	debugflag = 0;
static int      draw_buffer = 0;

void
boot(void)
{
    int i;
    char *ap;
    u32 *argp;
    u32 argbuf[16];

    /* notice that you can't call osSyncPrintf() until you set
     * up an idle thread.
     */
    
    osInitialize();

    argp = (u32 *)RAMROM_APP_WRITE_ADDR;
    for (i=0; i<sizeof(argbuf)/4; i++, argp++) {
	osPiRawReadIo((u32)argp, &argbuf[i]); /* Assume no DMA */
    }
    /* Parse the options */
    ap = (char *)argbuf;
    while (*ap != '\0') {
	while (*ap == ' ')
	    ap++;
	if ( *ap == '-' && *(ap+1) == 'd') {
	    debugflag = 1;
	    ap += 2;
	} else if ( *ap == '-' && *(ap+1) == 't') {
	    do_texture = 1;
	    ap += 2;
	} else if ( *ap == '-' && *(ap+1) == 'r') {
	    rdp_DRAM_io = 1;
	    ap += 2;
	}
    }
    
    osCreateThread(&idleThread, 1, idle, (void *)0,
		   idleThreadStack+STACKSIZE/sizeof(u64), 10);
    osStartThread(&idleThread);

    /* never reached */
}

static void
idle(void *arg)
{
    /* Initialize video */
    osCreateViManager(OS_PRIORITY_VIMGR);
    osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);
    
    osViSetSpecialFeatures( OS_VI_GAMMA_OFF | OS_VI_GAMMA_DITHER_OFF );
    osViSetSpecialFeatures( OS_VI_DITHER_FILTER_ON );

    /*
     * Start PI Mgr for access to cartridge
     */
    osCreatePiManager((OSPri)OS_PRIORITY_PIMGR, &PiMessageQ, PiMessages, 
		      NUM_PI_MSGS);
    
    /*
     * Start RMON for debugging & data xfer (make sure to start 
     * PI Mgr first)
     */
    osCreateThread(&rmonThread, 0, rmonMain, (void *)0,
		   (void *)(rmonStack+RMON_STACKSIZE/8), 
		   (OSPri) OS_PRIORITY_RMON );
    osStartThread(&rmonThread);
    
    /*
     * Create main thread
     */
    osCreateThread(&mainThread, 3, mainproc, arg,
		   mainThreadStack+STACKSIZE/sizeof(u64), 10);
    
    if (!debugflag)
	osStartThread(&mainThread);

    /*
     * Become the idle thread
     */
    osSetThreadPri(0, 0);

    for (;;);
}

#if 0
#define DO_DUMPTASK
#include "dumptask.c"
#endif

int rsp_ticks = 0;
int rdp_ticks = 0;


/*
 * This is the main routine of the app.
 */
static void
mainproc(void *arg)
{
    OSTask		*tlistp;
    Dynamic		*dynamicp;
    char		*staticSegment;
    void sp_do_work( Gfx ** );
    OSTime      start_time;

    /*
     * Setup the message queues
     */
    osCreateMesgQueue(&dmaMessageQ, &dmaMessageBuf, 1);
    
    osCreateMesgQueue(&rspMessageQ, &rspMessageBuf, 1);
    osSetEventMesg(OS_EVENT_SP, &rspMessageQ, NULL);
    
    osCreateMesgQueue(&rdpMessageQ, &rdpMessageBuf, 1);
    osSetEventMesg(OS_EVENT_DP, &rdpMessageQ, NULL);
    
    osCreateMesgQueue(&retraceMessageQ, &retraceMessageBuf, 1);
    osViSetEvent(&retraceMessageQ, NULL, 1);
    
    /*
     * Stick the static segment right after the code/data segment
     */
    staticSegment = _codeSegmentEnd;
    osPiStartDma(&dmaIOMessageBuf, OS_MESG_PRI_NORMAL, OS_READ,
		 (u32)_staticSegmentRomStart, staticSegment,
		 (u32)_staticSegmentRomEnd - (u32)_staticSegmentRomStart,
		 &dmaMessageQ);
    
    /*
     * Wait for DMA to finish
     */
    (void)osRecvMesg(&dmaMessageQ, NULL, OS_MESG_BLOCK);
    
    /*
     * Main game loop
     */
    while (1) {

	/*
	 * pointers to build the display list.
	 */
	tlistp = &tlist;
	dynamicp = &dynamic;

	guOrtho(&dynamicp->projection,
		-(float)SCREEN_WD/2.0F, (float)SCREEN_WD/2.0F,
		-(float)SCREEN_HT/2.0F, (float)SCREEN_HT/2.0F,
		1.0F, 10.0F, 1.0F);
	guRotate(&dynamicp->modeling, theta, 0.0F, 0.0F, 1.0F);

	glistp = dynamicp->glist;

	/*
	 * Tell RCP where each segment is
	 */
	gSPSegment(glistp++, 0, 0x0);	/* Physical address segment */
	gSPSegment(glistp++, STATIC_SEGMENT, OS_K0_TO_PHYSICAL(staticSegment));
	gSPSegment(glistp++, CFB_SEGMENT, OS_K0_TO_PHYSICAL(cfb[draw_buffer]));

	/*
	 * Initialize RDP state.
	 */
	gSPDisplayList(glistp++, rdpinit_dl);

	/*
	 * Initialize RSP state.
	 */
	gSPDisplayList(glistp++, rspinit_dl);

	/*
	 * Clear color framebuffer.
	 */
	gSPDisplayList(glistp++, clearcfb_dl);

	/* simple triangle: */

#if 0
        if (do_texture) {
            gSPDisplayList(glistp++, textri_dl);
        } else {
            gSPDisplayList(glistp++, shadetri_dl);
        }
#endif

	sp_do_work( &glistp );


	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);

	assert((glistp-dynamicp->glist) < GLIST_LEN);



	/* 
	 * Build graphics task:
	 *
	 */
	tlistp->t.ucode_boot = (u64 *) rspbootTextStart;
	tlistp->t.ucode_boot_size = (u32)rspbootTextEnd - (u32)rspbootTextStart;

	/*
	 * choose which ucode to run:
	 */
	if (rdp_DRAM_io) {
	    /* re-direct output to DRAM: */
		tlistp->t.ucode = (u64 *) gspFast3D_dramTextStart;
		tlistp->t.ucode_data = (u64 *) gspFast3D_dramDataStart; 
	} else {
	    /* RSP output over XBUS to RDP: */
		tlistp->t.ucode = (u64 *) gspFast3DTextStart;
		tlistp->t.ucode_data = (u64 *) gspFast3DDataStart;
	}
	
	/* initial display list: */
	tlistp->t.data_ptr = (u64 *) dynamicp->glist;
	tlistp->t.data_size = (u32)((glistp - dynamicp->glist) * sizeof(Gfx));

	/*
	 * Write back dirty cache lines that need to be read by the RCP.
	 */
	osWritebackDCache(&dynamic, sizeof(dynamic));
	
	/*
	 * start up the RSP task
	 */

#ifdef DO_DUMPTASK
	gdDumpTask( tlistp ); /* */
#endif

	start_time = osGetTime();

	osSpTaskStart(tlistp);
	
	/* wait for SP completion */
	(void)osRecvMesg(&rspMessageQ, NULL, OS_MESG_BLOCK);

	rsp_ticks = osGetTime() - start_time;
	rdp_ticks = 0;


	if (rdp_DRAM_io) {
		/* guParseRdpDL(rdp_output, rdp_output_len, 0); */

	    /*
	     * Send the DP display list to the DP:
	     */
	    osDpSetNextBuffer(&(rdp_output[0]),rdp_output_len);

	}

	(void)osRecvMesg(&rdpMessageQ, NULL, OS_MESG_BLOCK);

	rdp_ticks = osGetTime() - start_time - rsp_ticks;


	/* setup to swap buffers */
	osViSwapBuffer(cfb[draw_buffer]);

	/* Make sure there isn't an old retrace in queue 
	 * (assumes queue has a depth of 1) 
	 */
	if (MQ_IS_FULL(&retraceMessageQ))
	    (void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);
	
	/* Wait for Vertical retrace to finish swap buffers */
	(void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);



	draw_buffer ^= 1;

	theta += 1.0F;
    }
}
