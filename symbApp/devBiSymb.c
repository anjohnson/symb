/* base/src/dev $Id$ */

/* @(#)devBiSymb.c	1.1	6/4/93
 *      Device Support for VxWorks Global Symbol Binary Input Records
 *
 *
 *      Author:         Carl Lionberger
 *      Date:           060893
 *
 *      Experimental Physics and Industrial Control System (EPICS)
 *
 *      Copyright 1991, the Regents of the University of California,
 *      and the University of Chicago Board of Governors.
 *
 *      This software was produced under  U.S. Government contracts:
 *      (W-7405-ENG-36) at the Los Alamos National Laboratory,
 *      and (W-31-109-ENG-38) at Argonne National Laboratory.
 *
 *      Initial development by:
 *              The Controls and Automation Group (AT-8)
 *              Ground Test Accelerator
 *              Accelerator Technology Division
 *              Los Alamos National Laboratory
 *
 *      Co-developed with
 *              The Controls and Computing Group
 *              Accelerator Systems Division
 *              Advanced Photon Source
 *              Argonne National Laboratory
 *              
 *              The Control Systems Group
 *              Systems Engineering Department
 *              Lawrence Berkeley Laboratory
 *
 *      NOTES:
 * Derived from soft record device support.
 * Modification Log:
 * -----------------
 * wfl  06-Jun-96       call devSymbFind() to parse PARM field and
 *                      look up symbol
 * anj  14-Oct-96	Changed devSymbFind() parameters.
 * wfl  12-Dec-96       created from longin support
 */

#include	<vxWorks.h>
#include        <sysSymTbl.h>
#include	<types.h>
#include	<stdioLib.h>
#include	<string.h>

#include	<alarm.h>
#include	<dbDefs.h>
#include	<dbAccess.h>
#include        <recSup.h>
#include	<devSup.h>
#include	<module_types.h>
#include	<biRecord.h>
#include	<devSymb.h>

/* Create the dset for devBiSymb */
static long init_record();
static long read_bi();

struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	read_bi;
}devBiSymb={
	5,
	NULL,
	NULL,
	init_record,
	NULL,
	read_bi};

long testb = 1;

static long init_record(pbi)
    struct biRecord	*pbi;
{
    /* determine address of record value */
    if (devSymbFind(pbi->name, &pbi->inp, &pbi->dpvt))
    {
        recGblRecordError(S_db_badField,(void *)pbi,
            "devBiSymb (init_record) Illegal NAME or INP field");
        return(S_db_badField);
    }

    return(0);		
}

static long read_bi(pbi)
    struct biRecord	*pbi;
{
    long status;
    struct vxSym *private = (struct vxSym *) pbi->dpvt;

    if (pbi->dpvt)
    {
       pbi->val = *((unsigned short *)(*private->ppvar) + private->index);
       status = 0;
    }
    else
       status = 1;

    if(RTN_SUCCESS(status)) pbi->udf=FALSE;

    return(2); /*don't convert*/
}