#ifndef LV_CONF_H
#define LV_CONF_H

#define LV_USE_LOG      0
#define LV_USE_PERF_MONITOR 0
#define LV_USE_MEM_MONITOR 0

#define LV_COLOR_DEPTH     16
#define LV_COLOR_16_SWAP   1

#define LV_USE_DRAW_SW     1
#define LV_USE_TICK_CUSTOM 1
#define LV_TICK_CUSTOM_INCLUDE <Arduino.h>
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())

#define LV_FONT_MONTSERRAT_14 1
#define LV_USE_LABEL          1

#define LV_USE_DISP_REFR_TIMER 1

#define LV_USE_TICK_CUSTOM 1
#define LV_TICK_CUSTOM_INCLUDE <Arduino.h>
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())
#define LV_TXT_ENC LV_TXT_ENC_UTF8

#endif // LV_CONF_H