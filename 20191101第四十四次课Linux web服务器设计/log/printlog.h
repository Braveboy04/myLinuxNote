

#ifndef __PRINTLOG_H__
#define __PRINTLOG_H__

#include <stdlib.h>  
#include <string.h>  
#include <time.h>  
#include <stdio.h>  
#include <stdarg.h> 
#include <limits.h>
#include <unistd.h>


#define Windows_OS 1  
#define Linux_OS   2  

#define OS_TYPE    2  
  
#if (OS_TYPE == Windows_OS)  
#include <io.h>  
#endif  

#if (OS_TYPE == Linux_OS)  
#include <dirent.h>  
#include <errno.h>  
#define _DEBUG  
#endif  


#include <openssl/err.h>
#include <curses.h>


#define MAXLEN 	4194304 
#define LOGPATH	"/tmp/log.txt"


/* Application parameters */
#define DEBUG (0)


/* Debug prints */

#if DEBUG
	#define DBG_PRINT(fmt, args...)  do{printf("[*]xxx: <%s>: " fmt "\n" \
						,__func__, ##args); } while (false)
#else
	#define DBG_PRINT(...) do {} while (false)
#endif




#if DEBUG
	#define DBG_FPRINT(fmt,args ...)  do {PrintfLog("[!] error: <%s>: <%s>: <%d>:" fmt "\n" \
						,__FILE__, __FUNCTION__,__LINE__ ,##args); } while (false)
#else
	#define DBG_FPRINT(...)  do{} while (false)
#endif



#ifdef __cplusplus
#if __cplusplus

extern "C"{
#endif
#endif /* __cplusplus */

extern int FilePathParse(const char* sFilePath, char* sDir, char* sFileName, char* sSufix, int* iFileLen);
extern int FileRename(const char* sFilePath);
extern char* GetCurrentTime();
extern int WriteToFile(const char* sFilePath, const char* sInfo);
extern void PrintfLog(const char *fmt, ...);
extern size_t get_executable_path( char* processdir,char* processname, size_t len);
extern void PrintPath();



#ifdef __cplusplus
#if __cplusplus
}

#endif
#endif /* __cplusplus */


#endif /* __PRINTLOG_H__ */
