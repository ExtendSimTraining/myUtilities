**************************
** Block: Clock DayHour
** Lib: myUtiliteis.lib 
** Lib: Version: 9.0.0
**************************

**************************
** Developer:
** myName
** myContactInfo
**************************

**************************
** Purpse: To animate the simulation time
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
** ToDo;
**************************

**************************
**	STATIC VARIABLE AND CONSTANTS
**************************

// Event Posting Variables
integer	myIndex;
real	timeArray[];			
integer timeBlocks[];


**********************
procedure UpdateTheClock()
{
	real lrHour;
	real lrDay;

	lrDay = ConvertTimeUnits(CurrentTime, GetTimeUnits(), 6); // 6 is days
	lrHour = ConvertTimeUnits(CurrentTime, GetTimeUnits(), 5); // 5 is hour
	lrHour = lrHour - Floor(lrDay)*24;

	lrDay = FixDecimal(lrDay, 0);
	lrHour = FixDecimal(lrHour, 0);

	wHour = lrHour;
	wDay = lrDay;
	
	AnimationText(1, lrDay);
	AnimationShow(1);

	AnimationText(2, lrHour);
	AnimationShow(2);
	
}


**************************
**	MESSAGE HANDLERS
**************************
// This message handler is used to grab its index number in the time array.
on checkdata
{
	myIndex = sysGlobalint0;
	sysGlobalint0 += 1.0;
}

**********************
// This message handler is used to get access to the time array.
on initsim
{	
	GetPassedArray(sysGlobal0, timeArray);
	
	if(wUpdateOnlyIfAnimationOn)
		timeArray[myIndex] = 1e300;
	else
		timeArray[myIndex] = 0.0;
	
	GetPassedArray(sysGlobal7,timeBlocks);
	timeBlocks[myIndex] = MyBlockNumber();
	
	AnimationText(1, 0);
	AnimationTextSize(1, 32);
	AnimationTextAlign(1, 1);
	AnimationColor(1, 43690, 65534, 56797, 1);
	AnimationShow(1);	

	AnimationText(2, 0);
	AnimationTextSize(2, 32);
	AnimationTextAlign(2, 1);
	AnimationColor(2, 43690, 65534, 56797, 1);
	AnimationShow(2);	
	
	wDay = 0;
	wHour = 0;
	
	if(wUpdateOnlyIfAnimationOn == False)
		GetSimulateMsgs(False);
}

**********************
on Simulate
{
	if(AnimationOn == True)
		UpdateTheClock();
}

**********************
on FinalCalc
{
	UpdateTheClock();
}

**********************
// When this block wakes up from the executive, it will set the next wake up time, then it will update the displayed Current Day.
on BlockReceive0
{
	integer lrLastAnimationSetting;	
	real lrNextTime;

	// If I am not forcing it to wake up on the HOUR, then I want to wake up on the Simulate message.
	if(wUpdateOnlyIfAnimationOn == True)
		return;
		
	// If I am forcing it then I want to capture the current animation setting and then force the animation update.
	lrLastAnimationSetting = AnimationOn;
	AnimationOn = True;	
	
	if(wForceAnimationUpdateHour)	// force wake up on the hour
		lrNextTime = CurrentTime + ConvertTimeUnits(1, 5, GetTimeUnits());  // Hours are 5
	else if (wForceAnimationUpdateDay)	// force wake up on the day
		lrNextTime = CurrentTime + ConvertTimeUnits(1, 6, GetTimeUnits());	// Days are 6

	// Set the next wake up time
	timeArray[myIndex] = lrNextTime;

	// Update the clock animation
	UpdateTheClock();
	
	// Set the animation back to the original setting.
	AnimationOn = lrLastAnimationSetting;	
}

