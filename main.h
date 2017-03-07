/* 
 * file:        main_base.h
 * author:      redxef
 * date:        September 13, 2016, 2:51 PM
 * version:     0.1.1
 * 
 * desc:
 * The header file for the main module. The needed operation frequency is
 * 204,8kHz for 20ms PWM. ---> set FOSC to 4MHz and use a prescaler of 64.
 * changes:
 * 0.1.1:
 * Added a maximum and minimum define for the pwm output. now the pwm is only
 * between 0 and 180 deg.
 */

#ifndef __MAIN_H_
#define	__MAIN_H_

#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin) - needed for 32mhz
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = ON       // PLL Enable (4x PLL disabled)
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will not cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

#pragma warning push
#pragma warning disable 752     // conversions to shorter data types are also ok
#pragma warning disable 520     // we don't want to have warnings for unused functions pop up
#pragma warning disable 1498    // we know what we are doing with the pointers
#pragma warning pop

/******************************************************************************/
/* INCLUDES *******************************************************************/
/******************************************************************************/
#include <xc.h>
#include <stdint.h>
#include <stdlib.h>
#include "stdfc.h"
#include "ringbuffer.h"

/******************************************************************************/
/* MACROS *********************************************************************/
/******************************************************************************/
#define _XTAL_FREQ          4000000

#define BDEVID              1

#define ARGBUFFLEN          (8+1)
#define STATUS_READING      0x01
#define STATUS_CMD_DONE     0x02
#define STATUS_ARG0_DONE    0x04
#define STATUS_ARG1_DONE    0x08
#define STATUS_LINE_DONE    0x10
#define STATUS_ID_READING   0x20
#define STATUS_ID_SET       0x40
#define STATUS_ARGERR       0x80

#define status_ifset(st)      (status & st)
#define status_ifnset(st)     (!(status & st))
#define status_set(st)        (status |= st)
#define status_clear(st)      (status &= ~st)

#define PWM_MIN             36      //   0 deg     0 rad
#define PWM_MID             96      //  90 deg  pi/2 rad
#define PWM_MAX             157     // 180 deg    pi rad

#define USE_PWM_LOCK

/******************************************************************************/
/* PUBLIC VARS ****************************************************************/
/******************************************************************************/
struct ringbuffer uart_rec_buff_;
struct ringbuffer *uart_rec_buff;
uint8_t cmdbuff[ARGBUFFLEN];        //holds the command string
uint8_t arg0buff[ARGBUFFLEN];       //holds the first argument
uint8_t arg1buff[ARGBUFFLEN];       //holds the second argument
uint8_t status;                     //status of the command validation
uint8_t usestr;                     //the strings to use for the command
                                    // 0: none
                                    // 1: cmdbuff
                                    // 2: +arg0buff
                                    // 3: +arg1buff
/******************************************************************************/
/* FUNCTION PROTOTYPES ********************************************************/
/******************************************************************************/
void initFrequency(void);
void initRegisters(void);
void initVariables(void);
void initUART(void);
void initTMR6(void);

void setDutyPWM1(uint16_t duty);
void initPWM1(void);

void setDutyPWM2(uint16_t duty);
void initPWM2(void);

void setDutyPWM3(uint16_t duty);
void initPWM3(void);

void setDutyPWM4(uint16_t duty);
void initPWM4(void);

void initInt(void);

void putc(char c);
void putstr(const char *c);

#endif

