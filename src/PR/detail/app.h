
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
 * File:	onetri.h
 * Create Date:	Mon Apr 17 11:45:47 PDT 1995
 *
 */

#define	STATIC_SEGMENT		1
#define	CFB_SEGMENT		2
#define	CZB_SEGMENT		3

#define	SCREEN_HT	240
#define	SCREEN_WD	320

/* this stack size is in bytes, and is a lot larger
 * than this program needs.
 */
#define	STACKSIZE	0x2000

#define	GLIST_LEN	2048
/*
 * ifdef needed because this file is included by "spec"
 */
#ifdef _LANGUAGE_C

/*
 * Layout of dynamic data.
 *
 * This structure holds the things which change per frame. It is advantageous
 * to keep dynamic data together so that we may selectively write back dirty
 * data cache lines to DRAM prior to processing by the RCP.
 *
 */
typedef struct {
	Mtx	projection;
	Mtx	modeling;
	Mtx	viewing;
        Mtx     identity;
	Mtx	rotate;
	Gfx	glist[GLIST_LEN];
} Dynamic;

extern Dynamic	dynamic;

/* these are the static display lists */

extern Gfx 	rspinit_dl[];
extern Gfx 	rdpinit_dl[];
extern Gfx	clearcfb_dl[];
extern Gfx	clearczb_dl[];
extern Gfx 	shadetri_dl[];
extern Gfx 	textri_dl[];
extern Gfx 	matrices_dl[];
extern Gfx 	river_dl[];

/* CPU addresses for the color frame buffer */

extern u16      cfb[][SCREEN_WD*SCREEN_HT];
extern u16      czb[SCREEN_WD*SCREEN_HT];

/* RSP address for the color frame buffer */

extern u16	rsp_cfb[];
extern u16	rsp_czb[];

#endif	/* _LANGUAGE_C */
