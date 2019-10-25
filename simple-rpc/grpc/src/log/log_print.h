/************************************************
  > File Name: log_print.h
  > Author: Philo
  > Mail: 
  > Created Time: Fri 18 Oct 2019 05:54:13 PM CST
*************************************************/
#ifndef LOG_PRINT_H
#define LOG_PRINT_H

#include <zlog.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef enum EN_LOG_LEVEL
{
    EN_LOG_DEBUG = ZLOG_LEVEL_DEBUG,
    EN_LOG_INFO = ZLOG_LEVEL_INFO,
    EN_LOG_NOTICE = ZLOG_LEVEL_NOTICE,
    EN_LOG_WARN = ZLOG_LEVEL_WARN,
    EN_LOG_ERROR = ZLOG_LEVEL_ERROR,
    EN_LOG_FATAL = ZLOG_LEVEL_FATAL
} EnLogLevel;

#define LOG_PRINT(level, format, args...) \
    dzlog(__FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, level, format, ##args)

#ifdef __cplusplus
}
#endif

#endif // end of LOG_PRINT_H
