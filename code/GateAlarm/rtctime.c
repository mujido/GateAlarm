/*
 * time.c
 *
 * Created: 10/23/2019 1:08:03 PM
 *  Author: kurt
 */ 

#include "rtctime.h"
#include <driver_init.h>

#define RTC_OVERFLOW_PERIOD 0x10000UL

static volatile uint8_t rtc_overflow_counter = 0;

uint32_t rtc_get_seconds(void)
{
    return rtc_overflow_counter * RTC_OVERFLOW_PERIOD + RTC.CNT;
}

void rtc_increment_overflow_counter()
{
    ++rtc_overflow_counter;
}
