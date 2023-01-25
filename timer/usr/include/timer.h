#ifndef _TIMER_H_
#define _TIMER_H_
#include "boards.h"
#include "nrf.h"
#include "nrf_drv_timer.h"
#include "bsp.h"
#include "app_error.h"

#define MAX_TIMER_INFO     4
#define LED1_TIMER         "LED1"
#define LED2_TIMER         "LED2"
#define LED3_TIMER         "LED3"
#define LED4_TIMER         "LED"

typedef struct timer_info
{
     struct timer_info *_this;
     const nrf_drv_timer_t instance;
     uint32_t ms;
     uint32_t ticks;
     uint32_t cc_channel;
     uint32_t clear_mask;
     const char *name;
}timer_info;

typedef struct timer_obj
{
     struct timer_obj *_this;
     bool (*RegisterFunc)(struct timer_obj *, void(*callback)(nrf_timer_event_t, void *));
     bool (*StartFunc)(struct timer_obj *, const char *);
     bool (*StopFunc)(struct timer_obj *, const char *);
     void (*RemoveObjFunc)(struct timer_obj *);
     timer_info *private;
}timer_obj;

timer_obj *CreateTimerObj(void);
#endif