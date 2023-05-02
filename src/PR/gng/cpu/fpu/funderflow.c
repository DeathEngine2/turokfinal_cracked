/*
 * fpu/funderflow.c
 *
 *
 *
 * Copyright 1991, Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics, Inc.;
 * the contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of Silicon Graphics, Inc.
 *
 * RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to restrictions
 * as set forth in subdivision (c)(1)(ii) of the Rights in Technical Data
 * and Computer Software clause at DFARS 252.227-7013, and/or in similar or
 * successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished -
 * rights reserved under the Copyright Laws of the United States.
 */

#ident "$Revision: 1.2 $"

/*
 *					
 *	Floating Point Exerciser - basic functions with simple single
 *	underflow
 *					
 */

#include <ultra64.h>
#include <os_internal.h>
#include <osint.h>
#include <fpu.h>
#include "cpu.h"

static jmp_buf fault_buf;

#define DIVUND		0x20000
#define	SR_EXCEPT	(SR_CU0 | SR_CU1 | SR_IE)

extern void	Delay(u32);
extern void	UNDERFLOW_OP(void);


int
funderflow()
{
    register long status;
    register int fail;
    u32	     stat;

    fail = 0;

    if (__osSetJmp((u32 *)fault_buf))
    {
	if (!((stat = __osGetFpcCsr()) & DIVUND)) {

            PRINTF("%s:%d: stat=0x%08x\n", __FILE__, __LINE__, stat);

	    fail = 1;
	}
	__osSetFpcCsr(0);
    }
    else
    {
	RegisterNoFault((u32 *)fault_buf);

	/* clear cause register */
	__osSetCause(0);

	/* enable cache and fpu - cache ecc errors still enabled */
	status = __osGetSR();
	__osSetSR(SR_EXCEPT);

	/* clear the FPSR */
	__osSetFpcCsr(0);

	/* set up fpu status register for exception */
	__osSetFpcCsr(FP_ENABLE);

	UNDERFLOW_OP();

	Delay(10);

	/* error if it does not generate an exception */
	fail = 1;

        PRINTF("%s:%d: FAILED!\n", __FILE__, __LINE__);

	ClearNoFault();
    }

    /* report any error */
    
    return(fail);
}
