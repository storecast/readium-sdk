#ifndef __RSDK_MACROS_H__
#define __RSDK_MACROS_H__

#define DO_TRACE 0
#define DO_PRINT 1

extern int JNI_API_CALLS;
extern int ADB_API_CALLS;

#include <android/log.h>

#define  LOG_TAG    "Readium JNI 2"

#define WARN(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define WARNING(tag, ...) __android_log_print(ANDROID_LOG_WARN,tag,__VA_ARGS__)

#if DO_PRINT == 1
#define PRINT(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#else
#define PRINT(...) ((void)0)
#endif

#if DO_TRACE
#define  TRACE_ADB_API_CALL(...)  	__android_log_print(ANDROID_LOG_INFO,LOG_TAG,"## %04d - %s\n", ++ADB_API_CALLS,__VA_ARGS__)
#define  JNI_CALL(...)  	__android_log_print(ANDROID_LOG_INFO,LOG_TAG,"#  %04d - %s\n", ++JNI_API_CALLS,__VA_ARGS__)
#else
#define TRACE_ADB_API_CALL(msg) 	((void)0)
#define  JNI_CALL(...) 		((void)0)
#endif

#endif /* #ifndef __RSDK_MACROS_H__ */
