#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"

static bool RegisterTimer(timer_obj *, void(*callback)(nrf_timer_event_t, void *));
static bool StartTimer(timer_obj *, const char *);
static bool StopTimer(timer_obj *this, const char *act_name);
static void RemoveTimerObj(timer_obj *);

static timer_info private[MAX_TIMER_INFO] =
{
     {
          .instance = NRF_DRV_TIMER_INSTANCE(0),
          .ms = 1000,
          .cc_channel = NRF_TIMER_CC_CHANNEL0,
          .clear_mask = NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK,
          .name       = LED1_TIMER
     },
     {
          .instance = NRF_DRV_TIMER_INSTANCE(1),
          .ms = 2000,
          .cc_channel = NRF_TIMER_CC_CHANNEL1,
          .clear_mask = NRF_TIMER_SHORT_COMPARE1_CLEAR_MASK,
          .name       = LED2_TIMER
     },
     {
          .instance = NRF_DRV_TIMER_INSTANCE(2),
          .ms = 1000,
          .cc_channel = NRF_TIMER_CC_CHANNEL2,
          .clear_mask = NRF_TIMER_SHORT_COMPARE2_CLEAR_MASK,
          .name       = LED3_TIMER
     },
     {
          .instance = NRF_DRV_TIMER_INSTANCE(3),
          .ms = 2000,
          .cc_channel = NRF_TIMER_CC_CHANNEL3,
          .clear_mask = NRF_TIMER_SHORT_COMPARE3_CLEAR_MASK,
          .name       = LED4_TIMER
     },
};

static timer_obj _this_ =
{
     .RegisterFunc  = RegisterTimer,
     .StartFunc     = StartTimer,
     .StopFunc      = StopTimer,
     .RemoveObjFunc = RemoveTimerObj,
};

timer_obj *CreateTimerObj(void)
{
     _this_.private = malloc(sizeof(private));
     ASSERT(_this_.private);

     memcpy(_this_.private, private, sizeof(private));

     return &_this_;
}

static void RemoveTimerObj(timer_obj *this)
{
     if (this->private)
     {
          free(this->private);
          this->private = NULL;
     }
}

static bool RegisterTimer(timer_obj *this, void(*callback)(nrf_timer_event_t event_type, void *p_context))
{
    uint32_t err_code = NRF_SUCCESS;
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;

    for (uint8_t i = 0; i < MAX_TIMER_INFO; i++)
    {
         timer_info *p_this = &this->private[i];
         err_code = nrf_drv_timer_init(&p_this->instance, &timer_cfg, callback);
         APP_ERROR_CHECK(err_code);

         p_this->ticks = nrf_drv_timer_ms_to_ticks(&p_this->instance, p_this->ms);

         nrf_drv_timer_extended_compare(&p_this->instance, p_this->cc_channel, p_this->ticks, p_this->clear_mask, true);
    }

    return true;
}

static bool StartTimer(timer_obj *this, const char *act_name)
{
     for (uint8_t i = 0; i < MAX_TIMER_INFO; i++)
     {
          timer_info *p_this = &this->private[i];
          if (p_this && (!strncmp(p_this->name, act_name, strlen(act_name)+1)))
          {
               nrf_drv_timer_enable(&p_this->instance);
               goto success;
          }
     }
     return false;

     success:
     return true;
}

static bool StopTimer(timer_obj *this, const char *act_name)
{
     for (uint8_t i = 0; i < MAX_TIMER_INFO; i++)
     {
          timer_info *p_this = &this->private[i];
          if (p_this && (!strncmp(p_this->name, act_name, strlen(p_this->name))))
          {
               nrf_drv_timer_disable(&p_this->instance);
               goto success;
          }
     }
     return false;

     success:
     return true;
}