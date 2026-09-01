#include "stdafx.h"
#include "traffic.h"
#include "ErrorLog.h"
#include "Global.h"

ErrorLog::ErrorLog()
{
	for (int i=0; i<MAX_LOG_FILE_SUM; i++)
	{
		FileNameArray[i]="";
		FileArray[i]=NULL;
	}
	FileNameSum=0;

	CString Current_Directory= Get_Current_Directory();
	Error_File_Directory=Current_Directory+"\\OutputData\\";
	CreateDirectory(Error_File_Directory ,NULL); 
}

ErrorLog::~ErrorLog()
{

}

void ErrorLog::CloseAllFiles()
{
		for (int i=0; i<FileNameSum; i++)
			fclose(FileArray[i]);
}

FILE* ErrorLog::OpenedFile(CString FileName)
{
	for (int i=0; i<FileNameSum;i++)
	{
		int flag= FileNameArray[i].Compare(FileName);
		
		if (flag==0)
			return FileArray[i];
	}
	return NULL;
}

void ErrorLog::Log(char * FileName, char* info)
{
	FILE *file=NULL;
	errno_t err;

	const int Buf_Size=1000;
	char buf[Buf_Size];
	strcpy_s(buf, Buf_Size, this->Error_File_Directory );
	strcat_s(buf, Buf_Size, FileName);

	char* FilePathAndName=&buf[0];  
	CString strFilePathAndName= FilePathAndName;   //char* --> CString

	file=OpenedFile(strFilePathAndName);
	if (file==NULL)
	{
		err = fopen_s(&file,  FilePathAndName,  "a");   

		FileArray[FileNameSum]=file;
		FileNameArray[FileNameSum]=strFilePathAndName;
		FileNameSum++;
	}
	fwrite(info, sizeof(char), strlen(info),file);
}



void ErrorLog::LogIntData(char *FileName, int data)
{
	CString str;
	str.Format("%d", data);
	Log(FileName, str.GetBuffer(0));	
 	str.ReleaseBuffer();
}

void ErrorLog::LogStrData(char *FileName, CString data)
{
	Log(FileName, data.GetBuffer(0));
 	data.ReleaseBuffer();
}

void ErrorLog::LogDoubleData(char *FileName, double data)
{
	CString str;
	str.Format("%f", data);
	Log(FileName, str.GetBuffer(0));	
 	str.ReleaseBuffer();
}







///////////////////////////////////////////////////////////////////
void ErrorLog::LogInt(char *FileName, int theInt)
{
	CString str;
	str.Format("%d", theInt);
	LogInTheFile(FileName, str);
}

void ErrorLog::LogInTheFile(char *FileName, CString str)
{
	extern int simu_time;
	CString strTime;
	strTime.Format("%d", simu_time);
	str= str+"---"+strTime+"\n";
	Log(FileName, str.GetBuffer(0));
	str.ReleaseBuffer();
}

//'T'---str + simu time
//'S'---str + sys time
void ErrorLog::LogStr(CString str, char charType)
{
	extern int simu_time;
	CString strTime;
	if (charType=='T')     //simu time
	{
		strTime.Format("%d", simu_time);
		str= str+"---"+strTime+"\n";
		Log("Simulation.log" , str.GetBuffer(0));
 		str.ReleaseBuffer();
		return ;
	}

	if (charType=='S')    //system time
	{
		SYSTEMTIME systime;
		GetSystemTime(&systime);
		CString strTemp;
		CString SystemTime;

		//year; month; day
		strTemp.Format("%d",systime.wYear);   
		SystemTime = strTemp+".";
		strTemp.Format("%d",systime.wMonth);
		SystemTime = SystemTime+ strTemp+".";
		strTemp.Format("%d",systime.wDay);
		SystemTime =SystemTime + strTemp+"    ";
		//hr; min; sec
		strTemp.Format("%d",systime.wHour);
		SystemTime =SystemTime+ strTemp+":";
		strTemp.Format("%d",systime.wMinute);
		SystemTime = SystemTime+ strTemp+":";
		strTemp.Format("%d",systime.wSecond);
		SystemTime = SystemTime+ strTemp+"\n";

		str= str+"     "+ SystemTime;
		Log("Simulation.log"  ,  str.GetBuffer(0));
 		str.ReleaseBuffer();

		return ;
	}
	
	str= str+"\n";
	Log("Simulation.log"  ,  str.GetBuffer(0));
 	str.ReleaseBuffer();

}

