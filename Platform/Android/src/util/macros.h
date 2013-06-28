#ifndef __RSDK_MACROS_H__
#define __RSDK_MACROS_H__

#define DO_TRACE 0
#define DO_RECORD 0
#define DO_PRINT 1

extern int JNI_API_CALLS;
extern int ADB_API_CALLS;

#define BOOL_STR(x) (x ? "true" : "false")

#ifdef ANDROID
#include <android/log.h>
#define  LOG_TAG    "Readium JNI 2"
#if DO_PRINT == 1
#define PRINT(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#endif
#define WARN(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define WARNING(tag, ...) __android_log_print(ANDROID_LOG_WARN,tag,__VA_ARGS__)

#if DO_RECORD
#define  RECORD_TAG    "Readium RECORD"
#define  LOG_ADB_API_CALL(...)  	__android_log_print(ANDROID_LOG_INFO, RECORD_TAG,"## %04d - %s\n", ++ADB_API_CALLS,__VA_ARGS__)
#else
#define LOG_ADB_API_CALL(...) 	((void)0)
#endif

#if DO_TRACE
#define  TRACE_ADB_API_CALL(...)  	__android_log_print(ANDROID_LOG_INFO,LOG_TAG,"## %04d - %s\n", ++ADB_API_CALLS,__VA_ARGS__)
#define  JNI_CALL(...)  	__android_log_print(ANDROID_LOG_INFO,LOG_TAG,"#  %04d - %s\n", ++JNI_API_CALLS,__VA_ARGS__)
#else
#define TRACE_ADB_API_CALL(msg) 	((void)0)
#define  JNI_CALL(...) 		((void)0)
#endif
#else

#define PRINT(...)  printf(__VA_ARGS__)
#define WARN(...)  printf(__VA_ARGS__)
#define WARNING(tag, ...)  printf(__VA_ARGS__)

#if DO_TRACE
#define TRACE_ADB_API_CALL(msg) 	printf ("## %04d - %s\n", ++ADB_API_CALLS, msg)
#define  JNI_CALL(msg)   		printf ("#  %04d - %s\n", ++JNI_API_CALLS, msg)
#else
#define TRACE_ADB_API_CALL(msg) 	((void)0)
#define  JNI_CALL(...) 		((void)0)
#endif
#endif

#if DO_PRINT == 0
#undef PRINT
#define PRINT(...) ((void)0)
#endif

#ifndef MIN
#define MIN(a, b)  ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b)  ((a) > (b) ? (a) : (b))
#endif

#ifndef BOUND
#define BOUND(x, a, b) ((x) < (a) ? (a) : ((x) > (b) ? (b)  : (x)))
#endif


#endif
