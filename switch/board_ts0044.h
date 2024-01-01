/********************************************************************************************************
 * @file    board_ts_0041.h
 *
 * @brief   This is the header file for board TS0041 - Lonsonho switch Model ZBWS01A
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

#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

// BUTTON
// Top Left, Button 1
#define BUTTON1                     GPIO_PB4
#define PB4_FUNC                    AS_GPIO
#define PB4_OUTPUT_ENABLE           0
#define PB4_INPUT_ENABLE            1
#define PULL_WAKEUP_SRC_PB4         PM_PIN_PULLUP_10K //PM_PIN_UP_DOWN_FLOAT//PM_PIN_PULLUP_10K//PM_PIN_PULLDOWN_100K

// Bottom Left, Button 2
#define BUTTON2                     GPIO_PC2
#define PC2_FUNC                    AS_GPIO
#define PC2_OUTPUT_ENABLE           0
#define PC2_INPUT_ENABLE            1
#define PULL_WAKEUP_SRC_PC2         PM_PIN_PULLUP_10K

// Top Right, Button 3
#define BUTTON3                     GPIO_PD2
#define PD2_FUNC                    AS_GPIO
#define PD2_OUTPUT_ENABLE           0
#define PD2_INPUT_ENABLE            1
#define PULL_WAKEUP_SRC_PD2         PM_PIN_PULLUP_10K

// Bottom Right, Button 4
#define BUTTON4                     GPIO_PC3
#define PC3_FUNC                    AS_GPIO
#define PC3_OUTPUT_ENABLE           0
#define PC3_INPUT_ENABLE            1
#define PULL_WAKEUP_SRC_PC3         PM_PIN_PULLUP_10K

// NET button.... find out if this is right!
#define BUTTON_NET                  GPIO_PB5
#define PB5_FUNC                    AS_GPIO
#define PB5_OUTPUT_ENABLE           0
#define PB5_INPUT_ENABLE            1
#define PULL_WAKEUP_SRC_PB5         PM_PIN_PULLDOWN_100K //PM_PIN_UP_DOWN_FLOAT//PM_PIN_PULLUP_10K//PM_PIN_PULLDOWN_100K

// LED1
#define LED1                        GPIO_PA0
#define PA0_FUNC                    AS_GPIO
#define PA0_OUTPUT_ENABLE           1
#define PA0_INPUT_ENABLE            0
//to disable led blink on wake, led off with 1
#define PA0_DATA_OUT                1

// LED2
#define LED2                        GPIO_PC0
#define PC0_FUNC                    AS_GPIO
#define PC0_OUTPUT_ENABLE           1
#define PC0_INPUT_ENABLE            0
//to disable led blink on wake, led off with 1
#define PC0_DATA_OUT                1

// LED3
#define LED3                        GPIO_PD4
#define PD4_FUNC                    AS_GPIO
#define PD4_OUTPUT_ENABLE           1
#define PD4_INPUT_ENABLE            0
//to disable led blink on wake, led off with 1
#define PD4_DATA_OUT                1

// LED4
#define LED4                        GPIO_PD7
#define PD7_FUNC                    AS_GPIO
#define PD7_OUTPUT_ENABLE           1
#define PD7_INPUT_ENABLE            0
//to disable led blink on wake, led off with 1
#define PD7_DATA_OUT                1

//TS0044 have LEDs active low
#define LED_ON						0//1
#define LED_OFF						1//0

#define PM_WAKEUP_LEVEL             PM_WAKEUP_LEVEL_LOW

// UART
#if ZBHCI_UART
    #error please configurate uart PIN!!!!!!
#endif

// DEBUG
#if UART_PRINTF_MODE
    #define DEBUG_INFO_TX_PIN       GPIO_PB1
#endif


enum{
    VK_SW1 = 0x01,
    VK_SW2 = 0x02,
    VK_SW3 = 0x03,
    VK_SW4 = 0x04,
    VK_NET = 0x05,
};

#define KB_MAP_NORMAL   {{VK_SW1, VK_SW2, VK_SW3, VK_SW4},}

#define KB_MAP_NUM      KB_MAP_NORMAL
#define KB_MAP_FN       KB_MAP_NORMAL

#define KB_DRIVE_PINS  {NULL }
#define KB_SCAN_PINS   {BUTTON1,  BUTTON2, BUTTON3, BUTTON4}


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
