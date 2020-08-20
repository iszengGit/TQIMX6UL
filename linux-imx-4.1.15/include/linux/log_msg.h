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
	LOG_VERBOSE,
} LOG_LEVEL;


#ifdef __cplusplus
extern "C" {
#endif

#define  log_msg(level, fmt, arg...)

#define log_v(fmt, arg...) 	log_msg(LOG_VERBOSE, fmt, ##arg)
#define log_d(fmt, arg...) 	log_msg(LOG_DEBUG, fmt, ##arg)
#define log_i(fmt, arg...) 	log_msg(LOG_INFO,fmt, ##arg)
#define log_w(fmt, arg...) 	log_msg(LOG_WARN,fmt, ##arg)
#define log_e(fmt, arg...) 	log_msg(LOG_ERROR,fmt, ##arg)


#ifdef __cplusplus
}
#endif

#endif /* __ZQL_SYSLSI_APDEV_log_msg_H__ */
