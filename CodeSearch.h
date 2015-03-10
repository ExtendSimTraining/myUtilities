**************************
** Block: CodeSearch
** Lib: myUtiliteis.lib 
** Lib: Version: 9.0.0
**************************

**************************
** Developer:
** myName
** myContactInfo
**************************

**************************
** Purpse: To search for code in an equation block
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

string gsDynText[];
string gsBlockInfoArray[][6];

on wFindBtn
{
	integer i, j;
	integer liNumBlocks;
	integer liFound;
	integer liRecNo;
	integer liDimension;
	integer liContinue;
	integer liCalcBeforeStartPop;
	integer liCalcBeforeStartChk;
				
	
	string lsDynText;
	string lsBlockName;
	string lsDialogName;
	string lsCalcBeforeStartPop;
	string lsCalcBeforeStartChk;
	
	DisposeArray(gsBlockInfoArray);
	DynamicDataTable(MyBlockNumber(), "wBlockInfoTTbl", gsBlockInfoArray);

	liRecNo = 0;
	lsDialogName = "EQ_EquationText";
	liNumBlocks = numBlocks();
	for(i = 0; i < liNumBlocks; i++)	// look through all of the blocks in the model
	{
		liContinue = False;
		lsBlockName = BlockName(i);
		if(lsBlockName == "Equation" or lsBlockName == "Optimizer" or lsBlockName == "Query Equation"
			 or lsBlockName == "Equation(I)" or lsBlockName == "Query Equation (I)" or lsBlockName == "Queue Equation" )
			liContinue = True;
		
		if(liContinue == False)
			Continue;
	
		liDimension = GetDimensionByName(i, lsDialogName);
		if(liDimension <= 0)	
			liDimension = 1;
	
		DisposeArray(gsDynText);
		MakeArray(gsDynText, liDimension);
		for(j = 0; j < liDimension; j++)
		{
			//GetDialogVariable(blockNum, paramNameStr, row, col)
			gsDynText[j] = GetDialogVariable(i, lsDialogName, j, 0);
			lsDynText = gsDynText[j];
			//StrFind(s, findString, caseSens, diacSens);
			liFound = StrFind(lsDynText, wContains, False, False);
			if(liFound < 0)
				Continue;
				
			liCalcBeforeStartPop = -1;
			lsCalcBeforeStartChk = GetDialogVariable(i, "CalcBeforeStart_chk", 0, 0);
			liCalcBeforeStartChk = StrToReal(lsCalcBeforeStartChk);
			
			if(liCalcBeforeStartChk == True)
			{
				lsCalcBeforeStartPop = GetDialogVariable(i, "CalcBeforeStart_pop", 0, 0);
				liCalcBeforeStartPop = StrToReal(lsCalcBeforeStartPop);	
			}
				
			liRecNo++;
			MakeArray(gsBlockInfoArray, liRecNo);
			gsBlockInfoArray[liRecNo-1][0] = i;			
			gsBlockInfoArray[liRecNo-1][1] = lsBlockName;			
			gsBlockInfoArray[liRecNo-1][2] = GetBlockLabel(i);	
			gsBlockInfoArray[liRecNo-1][3] = GetBlockLabel(GetEnclosingHBlockNum2(i));	
			if(liCalcBeforeStartPop >= 0)
				gsBlockInfoArray[liRecNo-1][4] = liCalcBeforeStartPop;	
			
			gsBlockInfoArray[liRecNo-1][5] = gsDynText[j];	
			
			//DynamicDataTable(blockNum, dataTableName, dynArray)		
			DynamicDataTable(MyBlockNumber(), "wBlockInfoTTbl", gsBlockInfoArray);			
		}
	}
}