/*
 * Driver header file for Samsung JPEG Encoder/Decoder
 *
 * Peter Oh, Copyright (c) 2009 Samsung Electronics
 * 	http://www.samsungsemi.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __ZQL_SYSLSI_APDEV_log_msg_H__
#define __ZQL_SYSLSI_APDEV_log_msg_H__

#ifndef TAG
#define TAG "ZQL"
#endif

typedef enum log_level_t
{
	LOG_ERROR,
	LOG_WARN,
	LOG_INFO,
	LOG_DEBUG,
} LOG_LEVEL;

const static char *log_level_str[] = {"ERR", "WNG", "INF", "DBG"};
static enum log_level_t default_level = LOG_DEBUG;


#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG
#define log_msg(level, fmt, arg...)	\
do{ \
	if(level<=default_level){ \
		printf("\t\t[ %s <==> %s ] %s: " \
			fmt, log_level_str[level], TAG, __func__, ##arg);\
	}\
} while(0)

#define log_d(fmt,arg...) \
do{ \
	if(LOG_DEBUG<=default_level){ \
		printf("\t\t[ %s <==> %s ] %s: " \
			fmt, log_level_str[LOG_DEBUG], TAG, __func__, ##arg);\
	}\
} while(0)

#else
#define  log_msg(level, fmt, arg...)
#define log_d(fmt,arg...)
#endif


#ifdef __cplusplus
}
#endif

#endif /* __ZQL_SYSLSI_APDEV_log_msg_H__ */
