// service.cpp : Defines the entry point for the console application.
//




//#include <windows.h>
//#include <iostream.h>
//#include "windows.h"
//#include "afx.h"
//#include "afxwin.h"
#include "stdafx.h"

bool shoot(bool reboot, bool force)
{
OSVERSIONINFO ver;
ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
GetVersionEx(&ver);
if (ver.dwPlatformId == VER_PLATFORM_WIN32_NT) // в семействе NT для выключения необходимо иметь привилегию SE_SHUTDOWN_NAME
        {
        HANDLE hToken;
        TOKEN_PRIVILEGES* NewState;
        OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&hToken);
        NewState=(TOKEN_PRIVILEGES*)malloc(sizeof(TOKEN_PRIVILEGES) + sizeof (LUID_AND_ATTRIBUTES));
        NewState->PrivilegeCount = 1;
        LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&NewState->Privileges[0].Luid);
        NewState->Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        AdjustTokenPrivileges(hToken, FALSE, NewState, NULL, NULL,NULL);
        free(NewState);
        CloseHandle(hToken);
        }

UINT mode = 0;
if (reboot)
        mode += EWX_REBOOT;
else
        mode += EWX_POWEROFF;

if (force) mode += EWX_FORCE;

return ExitWindowsEx(mode ,0);
}




int checkRestartService(const std::string & servicename)
{
	SERVICE_STATUS ssStatus;    
	DWORD dwOldCheckPoint;    
	SC_HANDLE schSCManager=OpenSCManager(      
		NULL,	// pointer to machine name string      
		NULL,	// pointer to database name string      
		SC_MANAGER_ALL_ACCESS 	// type of access    
	);    
	if(schSCManager==NULL){ //error handler    
	}    
	SC_HANDLE schService = OpenService(        
		schSCManager,          // SCM database        
		//TEXT("pageserver"),    // service name        
		TEXT(servicename.c_str()),
		SERVICE_ALL_ACCESS);    
	if (schService == NULL){ //error handler    
	}
	// Geting the status    
		if (!QueryServiceStatus( schService, &ssStatus) ){//error handler    
			//std::cout << "[" << "error QueryServiceStatus" << "]" << std::endl;
		}

		//std::cout << "[" << "--" << "]" << std::endl;


		if(!ControlService(schService, SERVICE_CONTROL_STOP, &ssStatus)){
			//some
			//std::cout << "[" << "error ControlService" << "]" << std::endl;
		}

	// Start the service

		if (!StartService( schService, 0, NULL)){
			//std::cout << "[" << "error StartService" << "]" << std::endl;
			//[error] Service1->LogMessage( GetError() + "\nFailed to start \"pageserver\" service!!!", EVENTLOG_ERROR_TYPE, 0, 6);
		}



	CloseServiceHandle( schSCManager);    
	CloseServiceHandle(schService);

	return 0;
}

int checkReboot(const std::string & filename)
{
	LPVOID lpMsgBuf;

	
	//std::cout << "[start]" << std::endl;

	char lpszString[100];

	/*
	//FILE * hFile;
	//hFile = fopen("c:\\miranda32.exe", "rb");
	HANDLE hFile=NULL; 
	//hFile = (HANDLE)fopen("c:\\autoexec.bat", "br+");
	hFile = CreateFile("c:\\readme.txt",GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
	if (hFile!=NULL)
	{
		FILETIME fc;
		FILETIME fa;
		FILETIME fw;
		if (GetFileTime(hFile,&fc,&fa,&fw)!=0)
		{
			//.....
		}
		else
			std::cout << "error gettime" << std::endl;
	}
	else
		std::cout << "error open" << std::endl;
	*/

	HANDLE hFile=NULL; 
	//hFile = CreateFile("c:\\readme.txt",GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
	hFile = CreateFile(filename.c_str(),GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);

	if(hFile == NULL){
		//std::cout << "[" << "ass" << "]" << std::endl;
	}

	//std::cout << "[" << hFile << "]" << std::endl;

    FILETIME ftCreate, ftAccess, ftWrite, ftNow;
    SYSTEMTIME stUTC, stLocal, stNow;
	
	GetSystemTime(&stNow);
	SystemTimeToFileTime(&stNow, &ftNow);

    // Retrieve the file times for the file.
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite)){
			if (!FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				GetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL ))
			{
			// Handle the error.
				//std::cout << "[xyi]" << std::endl;
			return FALSE;
			}


			//std::cout << "[" << (LPCTSTR)lpMsgBuf << "]" << std::endl;
		return FALSE;
	}
	
	//LocalFree( lpMsgBuf );

	//std::cout << "[*]" << std::endl;

    // Convert the last-write time to local time.
    FileTimeToSystemTime(&ftWrite, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

    // Build a string showing the date and time.
    
	wsprintf(lpszString, "%02d/%02d/%d  %02d:%02d",
        stLocal.wDay, stLocal.wMonth, stLocal.wYear,
        stLocal.wHour, stLocal.wMinute);
	
	//std::cout << "[" << lpszString << "]" << std::endl;

	long int x = CompareFileTime(&ftWrite, &ftNow);
	//std::cout << "[" << x << "]" << std::endl;


	/*
	long    i = 60000000L;
	clock_t start, finish;
	
	start = clock();
	while( i-- );
	finish = clock();
	double  duration = (double)(finish - start);// / CLOCKS_PER_SEC;
	printf( "%2.1f seconds *%d* \n", duration , CLOCKS_PER_SEC);
	
	printf("[%d %d]", start, finish);
	*/


	time_t t1, t2;
	struct tm stm1;
	struct tm stm2;
	
	stm1.tm_year = stUTC.wYear-1900;
	stm1.tm_mon = stUTC.wMonth;
	stm1.tm_mday = stUTC.wDay;

	stm1.tm_hour = stUTC.wHour;
	stm1.tm_min = stUTC.wMinute;
	stm1.tm_sec = stUTC.wSecond;


	stm2.tm_year = stNow.wYear-1900;
	stm2.tm_mon = stNow.wMonth;
	stm2.tm_mday = stNow.wDay;

	stm2.tm_hour = stNow.wHour;
	stm2.tm_min = stNow.wMinute;
	stm2.tm_sec = stNow.wSecond;

	t1 = mktime(&stm1); //t1 = _mktime64(&stm1);
	t2 = mktime(&stm2); //t2 = _mktime64(&stm2);

	unsigned int d = t2-t1;

	//printf("[%d-%d-%d %d:%d:%d]", stm1.tm_year, stm1.tm_mon, stm1.tm_mday, stm1.tm_hour, stm1.tm_min, stm1.tm_sec);
	//printf("[%d-%d-%d %d:%d:%d]", stm2.tm_year, stm2.tm_mon, stm2.tm_mday, stm2.tm_hour, stm2.tm_min, stm2.tm_sec);
	

	//printf("[%d-%d=%d]", t2, t1, d);

	if(d>60*30){
		//std::cout << "[reboot]" << std::endl;
		
		//shoot(false, true); //halt
		shoot(true, true); //reboot
	}

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{

	std::string servicename = "Messenger";
	checkRestartService(servicename);

	std::string filename = "c:\\readme.txt";
	checkReboot(filename);

	return 0;
}