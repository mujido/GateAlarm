/*
 * rtctime.h
 *
 * Created: 10/23/2019 10:24:17 PM
 *  Author: kurt
 */ 


#ifndef RTCTIME_H_
#define RTCTIME_H_

#include <stdint.h>

uint32_t rtc_get_seconds(void);

void rtc_increment_overflow_counter();

#endif /* RTCTIME_H_ */