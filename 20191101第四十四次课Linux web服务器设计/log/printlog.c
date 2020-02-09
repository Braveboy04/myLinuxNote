#include "printlog.h"


int WriteToFile(const char* sFilePath, const char* sInfo) 
{  
    if (sFilePath == NULL)  
    {  
        return 0;  
    }  
  
    if (strcmp(sFilePath, "") == 0)  
    {  
        return 0;  
    }  
  
    FileRename(sFilePath);  
  
    FILE* fp = fopen(sFilePath, "a+");  
    if (fp == NULL)  
    {  
        return -1;  
    }  
    fwrite(sInfo, strlen(sInfo+1), 1, fp);  
    fclose(fp);  
    return 0;  
}  


int FileRename(const char* sFilePath)  
{  
    int  iFileLen       = 0;  
    char sFileName[256] = {0};  
    char sDir[256]      = {0};  
    char sSufix[256]    = {0};  
  
    if(0 == FilePathParse(sFilePath, sDir, sFileName, sSufix, &iFileLen))  
    {  
        return 0;  
    }     
  
    if (iFileLen > MAXLEN)  
    {  
        char tmpPath[256] = {0};  
#if (OS_TYPE == Windows_OS)  
        int i = 0;  
        struct _finddata_t FileInfo;  
  
        while (1)  
        {  
            sprintf(tmpPath, "%s%s_%d.%s", sDir, sFileName, i++, sSufix);  
            if(_findfirst(tmpPath, &FileInfo) == -1L)  
            {  
                break;  
            }  
        }  
#endif  
  
#if (OS_TYPE == Linux_OS)  
        struct dirent *dp = NULL;  
        DIR *pDir = opendir(sDir);  
        char allFileName[4096] = {0};  
        while((dp = readdir(pDir)) != NULL)  
        {  
            strcat(allFileName, "$");  
            strcat(allFileName, dp->d_name);               
        }  
  
        char tmpName[32] = {0};   
        int i = 0;    
        while(1)  
        {             
            sprintf(tmpName, "%s_%d.%s", sFileName, i++, sSufix);  
            if(NULL == strstr(allFileName, tmpName))  
            {  
                break;  
            }  
        }  
        strcpy(tmpPath, sDir);  
        strcat(tmpPath, tmpName);  
#endif  
        if(-1 == rename(sFilePath, tmpPath))  
        {  
            DBG_PRINT("rename failed. error code:%d, error message:%s\n", errno, strerror(errno));  
            return 0;  
        }  
    }  
  
    return 1;  
}  


int FilePathParse(const char* sFilePath, char* sDir, char* sFileName, char* sSufix, int* iFileLen)  
{  
	int i=0;
    FILE * fp = fopen(sFilePath, "a+");  
    if (fp == NULL)  
    {  
        return 0;  
    }  
    fseek(fp, 0L, SEEK_END);  
    *iFileLen = ftell(fp);  
    fclose(fp); 
  
  
    int iLen = strlen(sFilePath);  
    int iPos = -1;  
    for (i=0; i<iLen; i++)  
    {  
        if ((sFilePath[i] == '\\') || (sFilePath[i] == '/'))  
        {  
            iPos = i;  
        }  
    }  
    char strFile[256] = {0};  
    int j = 0;  
    for (i=iPos+1; i<iLen; i++)  
    {  
        strFile[j++] = sFilePath[i];  
    }  
    strFile[j] = '\0';  
  
  
    if (iPos >= 0)  
    {  
        int k = 0;  
        for (i=0; i<=iPos; i++)  
        {  
            sDir[k++] = sFilePath[i];  
        }  
        sDir[k] = '\0';  
    }  
      
 
    int k = 0;  
    for (i =0; i <((int)strlen(strFile)); i++)  
    {  
        if (strFile[i] == '.')  
        {  
            k = i;  
            sFileName[i] = '\0';  
            break;  
        }   
        else   
        {  
            sFileName[i] = strFile[i];  
        }  
    }  
   
    if(k > 0)  
    {  
        int j = 0;  
        for (i=k+1; i<((int)strlen(strFile)); i++)  
        {  
            sSufix[j++] = strFile[i];  
        }  
        sSufix[j] = '\0';  
    }  
      
    return 1;  
} 


char* GetCurrentTime()   
{  
    char* strTime = (char*)malloc(32);  
    memset(strTime, 0, 32);  
  
    time_t timeTmp;  
    time(&timeTmp);  
    struct tm *ptm = localtime(&timeTmp);     
    struct tm *ptm2 = (struct tm *)malloc(sizeof(struct tm));  
    memset(ptm2, 0, sizeof(struct tm));  
    memcpy(ptm2, ptm, sizeof(struct tm));     
    sprintf(strTime, "[%04d-%02d-%02d %02d:%02d:%02d] ", (1900+ptm2->tm_year), (1+ptm2->tm_mon), ptm2->tm_mday, ptm2->tm_hour, ptm2->tm_min, ptm2->tm_sec);  
    free(ptm2);  
    ptm2 = NULL;  
    return strTime;  
}  


void PrintfLog(const char *fmt, ...)  
{  
    char* buf = (char*)malloc(40960);  
    memset(buf, 0, 40960);  
    va_list args;     
    va_start(args, fmt);  
    vsnprintf(buf, 4096, fmt, args);  
    va_end(args);  
      
    char* sTime = GetCurrentTime();  
  
    char* sTmp = (char*)malloc(41000);  
    memset(sTmp, 0, 41000);  
    strcpy(sTmp, sTime);  
    strcat(sTmp, buf);  
  
#ifdef _DEBUG  
    puts(sTmp);  
#endif  
  
    strcat(sTmp, "\n\r");  
    WriteToFile(LOGPATH, sTmp);  
  
    free(sTime);  
    sTime = NULL;  
    free(buf);  
    buf = NULL;  
    free(sTmp);  
    sTmp = NULL;  
}




size_t get_executable_path( char* processdir,char* processname, size_t len)
{
	char * path_end;
	if(readlink("/proc/self/exe", processdir,len) <=0)
		return -1;
	path_end = strrchr(processdir, '/');
	if(path_end == NULL)
		return -1;
	++path_end;
	strcpy(processname, path_end);
	*path_end ='\0';
	return (size_t)(path_end - processdir);
}

void PrintPath()
{
	char path[1024];
	char processname[1024];
	char buf[1024];
	get_executable_path(path, processname, sizeof(path));
	DBG_FPRINT("directory:%s processname:%s\n",path,processname);
	getcwd(buf,sizeof(buf));
	DBG_FPRINT("WorkPath:<%s>\n",buf);
}
