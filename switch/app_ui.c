/********************************************************************************************************
 * @file    app_ui.c
 *
 * @brief   This is the source file for app_ui
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/

#if (__PROJECT_TL_SWITCH__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "tuyaSwitch.h"
#include "app_ui.h"

#define MOVE_RATE 45

/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */
void led_on(u32 pin)
{
	//printf("LED%d on\n", pin);
    drv_gpio_write(pin, LED_ON);
}

void led_off(u32 pin)
{
	//printf("LED%d off\n", pin);
    drv_gpio_write(pin, LED_OFF);
}

void light_on(void)
{
	//printf("Light on\n");
    led_on(LED1);
    led_on(LED2);
    led_on(LED3);
    led_on(LED4);
}

void light_off(void)
{
	//printf("Light off\n");
    led_off(LED1);
    led_off(LED2);
    led_off(LED3);
    led_off(LED4);
}

void light_init(void)
{
	light_off();
}

s32 zclLightTimerCb(void *arg)
{
    u32 interval = 0;

    if(g_switchAppCtx.sta == g_switchAppCtx.oriSta){
        g_switchAppCtx.times--;
        if(g_switchAppCtx.times <= 0){
            g_switchAppCtx.timerLedEvt = NULL;
            return -1;
        }
    }

    g_switchAppCtx.sta = !g_switchAppCtx.sta;
    if(g_switchAppCtx.sta){
        light_on();
        interval = g_switchAppCtx.ledOnTime;
    }else{
        light_off();
        interval = g_switchAppCtx.ledOffTime;
    }

    return interval;
}

void light_blink_start(u8 times, u16 ledOnTime, u16 ledOffTime)
{
    u32 interval = 0;
    g_switchAppCtx.times = times;

    if(!g_switchAppCtx.timerLedEvt){
        if(g_switchAppCtx.oriSta){
            light_off();
            g_switchAppCtx.sta = 0;
            interval = ledOffTime;
        }else{
            light_on();
            g_switchAppCtx.sta = 1;
            interval = ledOnTime;
        }
        g_switchAppCtx.ledOnTime = ledOnTime;
        g_switchAppCtx.ledOffTime = ledOffTime;
        //printf("timer4 led blink start \n");
        g_switchAppCtx.timerLedEvt = TL_ZB_TIMER_SCHEDULE(zclLightTimerCb, NULL, interval);
    }
}

void light_blink_stop(void)
{
    if(g_switchAppCtx.timerLedEvt){
        TL_ZB_TIMER_CANCEL(&g_switchAppCtx.timerLedEvt);

        g_switchAppCtx.times = 0;
        if(g_switchAppCtx.oriSta){
            light_on();
        }else{
            light_off();
        }
    }
}

void cmdSendReport()
{
	printf("cmdSendReport\n");
    if(zb_isDeviceJoinedNwk()){
    	//light_blink_start(5, 500, 500);
#if 1
        epInfo_t dstEpInfo;
        TL_SETSTRUCTCONTENT(dstEpInfo, 0);

        dstEpInfo.profileId = HA_PROFILE_ID;
#if FIND_AND_BIND_SUPPORT
        dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
#else
        // dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
        dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
        dstEpInfo.dstEp = 1; // TUYA_SWITCH_ENDPOINT;
        dstEpInfo.dstAddr.shortAddr = 0x0000; // 0xfffc; coordinator
#endif
        //zcl_onOff_toggleCmd(TUYA_SWITCH_ENDPOINT, &dstEpInfo, FALSE);
    	zclAttrInfo_t *pAttrEntry;
    	pAttrEntry = zcl_findAttribute(TUYA_SWITCH_ENDPOINT, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_VOLTAGE);
    	zcl_sendReportCmd(TUYA_SWITCH_ENDPOINT, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
    			ZCL_CLUSTER_GEN_POWER_CFG, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);
    	pAttrEntry = zcl_findAttribute(TUYA_SWITCH_ENDPOINT, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING);
		zcl_sendReportCmd(TUYA_SWITCH_ENDPOINT, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
				ZCL_CLUSTER_GEN_POWER_CFG, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

#else
        brc_toggle();
#endif
    }
}
void cmdToggle(void)
{
	//printf("cmdToggle\n");
	if(zb_isDeviceJoinedNwk())
	{
		//light_blink_start(1, 500, 0);
#if 1
		epInfo_t dstEpInfo;
		TL_SETSTRUCTCONTENT(dstEpInfo, 0);

		dstEpInfo.profileId = HA_PROFILE_ID;
#if FIND_AND_BIND_SUPPORT
		dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
#else
        dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
        // dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
        // dstEpInfo.dstEp = TUYA_SWITCH_ENDPOINT;
        // dstEpInfo.dstAddr.shortAddr = 0xfffc;
#endif
		zcl_onOff_toggleCmd(TUYA_SWITCH_ENDPOINT, &dstEpInfo, FALSE);
#else
		brc_toggle();
#endif
	}
}

void cmdOnOff(u8 state)
{
    printf("state %d\n", state);
	if(zb_isDeviceJoinedNwk())
	{
		epInfo_t dstEpInfo;
		TL_SETSTRUCTCONTENT(dstEpInfo, 0);

		dstEpInfo.profileId = HA_PROFILE_ID;
        dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
        // dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
        // dstEpInfo.dstEp = TUYA_SWITCH_ENDPOINT;
        // dstEpInfo.dstAddr.shortAddr = 0xfffc;
        if(state)
        {
            zcl_onOff_onCmd(TUYA_SWITCH_ENDPOINT, &dstEpInfo, FALSE);
        }
        else
        {
            zcl_onOff_offCmd(TUYA_SWITCH_ENDPOINT, &dstEpInfo, FALSE);
        }
    }
}

void cmdMoveOnOff(u8 dir)
{
	//printf("cmdMoveOnOff\n");
	if(zb_isDeviceJoinedNwk())
	{
		epInfo_t dstEpInfo;
		TL_SETSTRUCTCONTENT(dstEpInfo, 0);

		dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEpInfo.dstEp = TUYA_SWITCH_ENDPOINT;
		dstEpInfo.dstAddr.shortAddr = 0xfffc;
		dstEpInfo.profileId = HA_PROFILE_ID;

		//moveToLvl_t move2Level;

		//move2Level.optPresent = 0;
		//move2Level.transitionTime = 0x0A;
		//move2Level.level = lvl;

		//zcl_level_move2levelCmd(TUYA_SWITCH_ENDPOINT, &dstEpInfo, FALSE, &move2Level);
		move_t move = {
            .moveMode = dir,
            .rate = MOVE_RATE,
            .optPresent = 0,
        };

		zcl_level_moveWithOnOffCmd(TUYA_SWITCH_ENDPOINT, &dstEpInfo, FALSE, &move);
	}
}

void cmdStopWithOnOff(void)
{
	//printf("cmdStop\n");
    if(zb_isDeviceJoinedNwk())  {
    	epInfo_t dstEpInfo;
        TL_SETSTRUCTCONTENT(dstEpInfo, 0);

        dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
        dstEpInfo.dstEp = TUYA_SWITCH_ENDPOINT;
        dstEpInfo.dstAddr.shortAddr = 0xfffc;
        dstEpInfo.profileId = HA_PROFILE_ID;

        stop_t stop = {.optPresent = 0};
        zcl_level_stopWithOnOffCmd(TUYA_SWITCH_ENDPOINT, &dstEpInfo, FALSE, &stop);
    }
}

s32 battVoltageCb(void *arg) {
	u16 voltage, percentage;
	u8 converted_voltage, percentage2;
	voltage = drv_get_adc_data();
	//printf("voltage %d\n", voltage);
	converted_voltage = (u8)(voltage/100);
	percentage = ((voltage - BATTERY_SAFETY_THRESHOLD)/4);
	if (percentage > 0xc8) percentage=0xc8;
	percentage2 = (u8)percentage;
	//printf("converted voltage %d diff %d", converted_voltage, (voltage - BATTERY_SAFETY_THRESHOLD));
	//printf(" , percentage2 %d\n", percentage2);
	zcl_setAttrVal(TUYA_SWITCH_ENDPOINT, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_VOLTAGE, &converted_voltage);
	zcl_setAttrVal(TUYA_SWITCH_ENDPOINT, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING, &percentage2);
	return 0;
}

void short_press_net(app_btn_ctx_t* ctx) {
    light_blink_start(5,300,700);
    cmdSendReport();
}
void hold_start_net(app_btn_ctx_t* ctx) {
    printf("Net key long hold!\n");
    light_blink_stop();
    light_blink_start(255, 300, 300);
    g_switchAppCtx.state = APP_FACTORY_NEW_DOING;
    zb_factoryReset();
    //not really sure it needed
    zb_resetDevice();
}
void empty_handler(app_btn_ctx_t* ctx) {}
void short_press_up(app_btn_ctx_t* ctx) { cmdOnOff(1); }
void short_press_down(app_btn_ctx_t* ctx) { cmdOnOff(0); } 
// these are backwards lol
void hold_start_up(app_btn_ctx_t* ctx) { cmdMoveOnOff(0); }
void hold_start_down(app_btn_ctx_t* ctx) { cmdMoveOnOff(1); }
void hold_done_up_or_down(app_btn_ctx_t* ctx) { cmdStopWithOnOff(); }

void button_check(app_btn_ctx_t* ctx, u8 on, void (*short_press)(app_btn_ctx_t*), void (*hold_start)(app_btn_ctx_t*), void (*hold_done)(app_btn_ctx_t*)) {
    // net button = 7s hold for reset, face button = 1s 
    u32 holdTime = ctx->idx == VK_NET ? (7*1000*1000) : (1*1000*1000);
    if(gpio_read(ctx->gpio) == on) {
        if(ctx->led != 0) led_on(ctx->led);
        if(ctx->state == BUTTON_RELEASED) {
            ctx->pressTime = clock_time();
            ctx->state = BUTTON_PRESSED;
        }
        else if(ctx->state == BUTTON_PRESSED && clock_time_exceed(ctx->pressTime, holdTime)){
            ctx->state = BUTTON_LONG_HOLD;
            hold_start(ctx);
        }
        g_switchAppCtx.keyPressed = 1;
    } else {
        if(ctx->led != 0) led_off(ctx->led);
        if(ctx->state == BUTTON_PRESSED) {
            short_press(ctx);
        } else if(ctx->state == BUTTON_LONG_HOLD) {
            hold_done(ctx);
        }
        ctx->state = BUTTON_RELEASED;
    }
}

void app_key_handler(void){
    // this gets picked up by the button checks.
    g_switchAppCtx.keyPressed = 0; 
    button_check(&g_switchAppCtx.buttons[0], 0, short_press_up, hold_start_up, hold_done_up_or_down);
    button_check(&g_switchAppCtx.buttons[1], 0, short_press_down, hold_start_down, hold_done_up_or_down);
    button_check(&g_switchAppCtx.buttons[2], 0, short_press_up, hold_start_up, hold_done_up_or_down);
    button_check(&g_switchAppCtx.buttons[3], 0, short_press_down, hold_start_down, hold_done_up_or_down);
    button_check(&g_switchAppCtx.buttonNet , 1, short_press_net, hold_start_net, empty_handler);
}

#endif  /* __PROJECT_TL_SWITCH__ */
