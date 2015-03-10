**************************
** Block: myUtiliteis.lib \ DBDocumentation
**************************

**************************
** Developer:
** myName
** myContactInfo
**************************

**************************
** Purpse: To pull out notes from tables & fields in a database
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

string63 DB_DatabaseName;	

************************************
procedure CallDIPopupButton()
{
	integer liMyBlockNum;
	
	liMyBlockNum = MyBlockNumber();
	
	//set the database name
	DIPopupButton(liMyBlockNum, "DB_DatabaseName_pop", TRUE);
	SetPopupLabels("DB_DatabaseName_pop", DB_DatabaseName);

}	//procedure CallDIPopupButton()


************************************
procedure DB_CheckIndexes()
{
	integer liMyBlockNum;
	
	liMyBlockNum = MyBlockNumber();

	//if a database has been selected, get the index
	if(DB_DatabaseName != "Select a database"  &&  DB_DatabaseName != "")
	{
		wDBIdx_prm = DBDatabaseGetIndex(DB_DatabaseName);
		if(wDBIdx_prm < 1)											
		{
			UserError("The database named, """ + DB_DatabaseName + """, in block number " + liMyBlockNum + " could not be found.");
			return;
		}
	}
}	//procedure DB_CheckIndexes()

************************************
integer DB_GetNumDatabases()
{

	integer i;
	integer liNumDBSlots;
	integer liNumDB;
	string63 lsNextName;

	liNumDBSlots = DBDatabasesGetNum();
	liNumDB = 0;
	
	for(i=1; i<=liNumDBSlots; i++)
		{
		lsNextName = DBDatabaseGetName(i);
		if(lsNextName != ""  &&  StrFind(lsNextName, "_", FALSE, FALSE) != 0)
			liNumDB++;
		}
	
	return(liNumDB);

}//integer DB_GetNumDatabases()

************************************
on DialogOpen
{
	integer liPopIsCloned;
	integer liDialogIsOpen;
	
	liPopIsCloned = CloneFind(MyBlockNumber(), "DB_DatabaseName_pop", 1);
	liDialogIsOpen = BlockDialogIsOpen(MyBlockNumber());
	
	if(liPopIsCloned ) 
		liPopIsCloned = TRUE;
	else
		liPopIsCloned = FALSE;
	
	if(liPopIsCloned  ||  liDialogIsOpen)
	{
		CallDIPopupButton();
		DB_CheckIndexes();
	}

}	// on DialogOpen


************************************
on DB_DatabaseName_pop
{
	integer i;
	integer liOldIndex;
	integer liNewIndex;
	integer liNumDB;
	
	liNumDB = DB_GetNumDatabases();
	if(liNumDB > 0)
	{
		liOldIndex = DBDatabaseGetIndex(DB_DatabaseName);		
		liNewIndex = DBDatabasePopupSelector(liOldIndex);
		
		if(liNewIndex != liOldIndex)
		{
			if(liNewIndex != -1)
				wDBIdx_prm = liNewIndex;
			else
				wDBIdx_prm = blank;
			
			//store the database name
			if(wDBIdx_prm > 0)
				DB_DatabaseName = DBDatabaseGetName(wDBIdx_prm);
			else
				DB_DatabaseName = "Select a database";
			SetPopupLabels("DB_DatabaseName_pop", DB_DatabaseName);
			
		}
	}	
	else //if(numDatabases <= 0)
	{
		UserError("There are no databases in this model. Create a database by selecting new database from the database menu.");
	}

}	//on DB_DatabaseName_pop


************************************
on wSendNotesToDocNow
{
	integer i, j;
	integer liExists;
	integer liNumTables;
	integer liNumFields;
	integer liFileNum;
	
	string lsNote;
	
	liExists = DBDatabaseExists(wDBIdx_prm);
	if(liExists == 0)
	{
		UserError("Please select a valid datbase in the DBDocumentation block number: " + myBlockNumber());
		abort;
	}
	
	liFileNum = FileNew(wFileName, "Enter the DB Doc File Name.");
	lsNote = "DB Doc for: " + DB_DatabaseName;
	FileWrite(liFileNum, lsNote , False, True);
		
	liNumTables = DBTablesGetNum(wDBIdx_prm);
	for( i = 1; i <= liNumTables; i++)
	{
		liExists = DBTableExists(wDBIdx_prm, i);
		if(liExists == False)
			Continue;
			

		lsNote = ""	;
		FileWrite(liFileNum, lsNote , False, True);

		lsNote = "(T) ";
		lsNote += DBTableGetName(wDBIdx_prm, i);
		lsNote += " : ";
		lsNote += DBToolTipsGet(wDBIdx_prm, i, 0);
//		// Read notes here?
//		lsNote += "this table note";
		FileWrite(liFileNum, lsNote , False, True);
		
		liNumFields = DBFieldsGetNum(wDBIdx_prm, i);
		for(j = 1; j <= liNumFields; j++)
		{
			//lsNote = "(T) ";
			//lsNote += DBTableGetName(wDBIdx_prm, i);
			//lsNote += " : ";
			//lsNote += "(F) "	;
			lsNote = StrPutAscII(09) + DBFieldGetName(wDBIdx_prm, i, j);
			lsNote += " : ";
			// Read notes here?
			lsNote += DBToolTipsGet(wDBIdx_prm, i, j);
//			lsNote += "this field note";
			FileWrite(liFileNum, lsNote , False, True);			
		}		
	}
	
	FileClose(liFileNum);
}	// on wSendNotesToDocNow


************************************
on wOpenDB
{
	DBDatabaseOpenViewer(wDBIdx_prm, -1);
}


