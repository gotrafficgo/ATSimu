#pragma once

#include "setting.h"

class ErrorLog  
{
public:
	CString FileNameArray[MAX_LOG_FILE_SUM];   // for the names of opened files
	FILE* FileArray[MAX_LOG_FILE_SUM];   
	int FileNameSum;
	CString Error_File_Directory;

public:
	ErrorLog();
	virtual ~ErrorLog();
	FILE*  OpenedFile(CString FileName);
	void CloseAllFiles();
	void Log(char *FileName, char* info=NULL);
	void LogIntData(char *FileName, int data);
	void LogStrData(char *FileName, CString data);
	void LogDoubleData(char *FileName, double data);
	

	/////////////////////////////////////////////
	void LogStr(CString str,char charType=NULL);
	void LogInt(char *FileName, int theInt);
	void LogInTheFile(char *FileName, CString str);
};

