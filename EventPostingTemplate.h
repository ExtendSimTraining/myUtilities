**************************
** Block: EventPostingTemplate
** Lib: myUtiliteis.lib 
** Lib: Version: 9.0.0
**************************

**************************
** Developer:
** myName
** myContactInfo
**************************

**************************
** Purpse: 
**	This block is an Event Posting template block.  This contains all the code
**	needed to make this block event posting.
**************************

**************************
** Version Info:
** V1.0 RJC Initial Post
** V2.0 RJC Pulled out source code to external source file
**************************

**************************
** Global Variables:
** Name / Documentation
**************************

**************************
** User Messages:
** Name / Documentation
**************************

**************************
** ToDo:
**************************

**************************
** 	CONSTANTS AND STATIC VARIABLES
**************************

integer giMyIndex;
integer giTimeBlocks[];
real 	grTimeArray[];

**************************
** 	PROCEDURES AND FUNCTIONS
**************************


**************************
** 	MESSAGE HANDLERS
**************************

// If the dialog data is inconsistent for simulation, abort.
on CheckData
{
	// Event Posting Code
	giMyIndex = sysGlobalInt0;
	sysGlobalInt0 += 1;
}

**************************
// Initialize any simulation variables.
on InitSim
{
	// Event Posting Code
	getPassedArray(SysGlobal0, grTimeArray);
	grTimeArray[giMyIndex] = StartTime;

	getPassedArray(sysGlobal7, giTimeBlocks);
	giTimeBlocks[giMyIndex] = myBlockNumber();
}

**************************
on BlockReceive0
{
	// Don't forget to schedule when this block should wake up next.
	grTimeArray[giMyIndex] = CurrentTime + 99;
	
	// Write custom code here!
}