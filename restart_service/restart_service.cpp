// restart_service.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include "afx.h"
//#include "afxwin.h"
//#include "WinSvc.h"

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

	if(d>60*60){
		//std::cout << "[reboot]" << std::endl;
		
		//shoot(false, true); //halt
		//shoot(true, true); //reboot

		return 10;
	}

	return 0;
}

int checkReboot2(const std::string & filename)
{
	FILE *stream;
	char list[30];
	int  i, numread, numwritten;
	
	if( (stream  = fopen( filename.c_str(), "r+t" )) == NULL ){
		//printf( "The file 'data' was not opened\n" );
	}else{
		//printf( "The file 'data' was opened\n" );
		numread = fread( list, sizeof( char ), 9, stream );
		list[numread] = 0x00;
		
		//std::cout << "[" << list << "]" << std::endl;
		
		int N = atoi(list);
		
		//std::cout << "[" << N << "]" << std::endl;
		char buffer[10];
		
		int result = fseek( stream, 0, SEEK_SET);
		
		int bReboot = 0;

		if(N==5){
			std::string str = itoa(0, buffer, 10);
			str += "count";
			numwritten = fwrite( str.c_str(), sizeof( char ), 9, stream );
			//std::cout << "[" << numwritten << "]" << std::endl;
			//std::cout << "[" << "reboot" << "]" << std::endl;
			
			bReboot = 10;
		}else{
			++N;
			std::string str = itoa(N, buffer, 10);
			str += "count";
			numwritten = fwrite( str.c_str(), sizeof( char ), 9, stream );
			//std::cout << "[" << numwritten << "]" << std::endl;
			//std::cout << "[" << "wait" << "]" << std::endl;
		}

		if( fclose( stream ) ){
			//printf( "The file 'data' was not closed\n" );
		}

		return bReboot;
	}

	return 0;
}

int saveToFile(const std::string & filename, std::string str)
{
	FILE *stream;

	bool bOK = false;

	if( (stream  = fopen( filename.c_str(), "r+t" )) == NULL ){
		if( (stream  = fopen( filename.c_str(), "w+t" )) == NULL ){
			bOK = false;
		}else{
			bOK = true;
		}
	}else{
		bOK = true;
	}

	if(bOK){
		int result = fseek( stream, 0, SEEK_END);
		int numwritten = fwrite( str.c_str(), sizeof( char ), str.length(), stream );
		fclose( stream );
	}

	return 0;
}

std::string getDateTime()
{
	/*
	char tmpbuf[128], ampm[] = "AM";
    __time64_t ltime;
    struct __timeb64 tstruct;
    struct tm *today, *gmt;
	*/
	
	char tmpbuf[128];

	std::string result = "";

	_tzset();
	
	_strdate( tmpbuf );
	result += tmpbuf;

	result += ' ';

	_strtime( tmpbuf );
	result += tmpbuf;
	
	__time64_t ltime;
	struct tm *gmt;

	//result = _ctime64( &ltime );

	_time64( &ltime );
	//printf( "Time in seconds since UTC 1/1/70:\t%ld\n", ltime );
	//printf( "UNIX time and date:\t\t\t%s", _ctime64( &ltime ) );
    //gmt = _gmtime64( &ltime );
    //printf( "Coordinated universal time:\t\t%s", asctime( gmt ) );

	result = _ctime64( &ltime );


	//gmt = _gmtime64( &ltime );
	//result = asctime( gmt );
	

	return result;
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::string action = "";
	if(argc>1){
		action = argv[1];
	}else{
		return 0;
	}

	//if(action=="restarts"){
	if(strcmp(action.c_str(),"restarts")==0){
	std::string servicename = "IS_Loader";
	//std::cout << "[" << "service" << "]" << std::endl;
	Sleep(1000*60*1);
	//std::cout << "[" << "service" << "]" << std::endl;
	checkRestartService(servicename);
	}else
	//if(action=="reboot"){
	if(strcmp(action.c_str(),"reboot")==0){
		std::string filename = "c:\\is\\monitoring.watch";
		//std::cout << "[" << "reboot 1" << "]" << std::endl;
		//if(10==checkReboot2(filename)){
		if(10==checkReboot(filename)){
			std::string str = getDateTime();
			str += "\t[reboot]\n";

			std::string logfilename = "reboot.log";

			saveToFile(logfilename, str);
			Sleep(1000*60*5);
			//std::cout << "[" << "reboot 2" << "]" << std::endl;
			shoot(true, true); //reboot
		}else{
			//std::cout << "[" << "reboot 3" << "]" << std::endl;
		}
	}else
	if(strcmp(action.c_str(),"manreboot")==0){
		std::string filename = "c:\\pipec\\reboot.xml";

		if( _unlink( filename.c_str() ) == -1 ){
			//perror( "[Could not delete]" );
		}else{
			//printf( "[Deleted]" );
			//Sleep(1000*60*5);
			shoot(true, true); //reboot
		}

	}else{
		//std::cout << "[" << "unknown action" << "]" << std::endl;
	}

	return 0;
}

