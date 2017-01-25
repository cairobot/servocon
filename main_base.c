/* 
 * file:        main.c
 * author:      redxef
 * date:        September 13, 2016, 2:38 PM
 * version:     0.1.2
 * 
 * desc:
 * The controller for the servos. The controller shall have 4 pwm outputs,
 * which each control one servo motor. 
 * changes:
 * 0.1.2:
 * Added a maximum and minimum guard for the pwm.
 */

#include "main_base.h"

void initFrequency(void) {
        // set the clk source, see data sheet page 56, 58, and 65
        // the source is selected with pragmas in main.h
        // otherwise the scs bits could be used
        
        // select the frequency of the device, see data sheet page 65 for more info
        // we may not use the lf oscillator, since we need the accuracy for the
        // servos
        
        // currently set to 4MHz (hf needed)
        IRCF0 = 1;
        IRCF1 = 0;
        IRCF2 = 1;
        IRCF3 = 1;
        
        OSCTUNE = 0b00000000;
}

void initRegisters(void) {
        PORTA  = 0b00000000;                    // init port vals
        ANSELA = 0b00000000;                    // init analog or digital
        TRISA  = 0b00000000;                    // init io
        
        PORTB  = 0b00000000;
        ANSELB = 0b00000000;
        TRISB  = 0b00010010;                    // rb1 is input, bc eusart rx, rb4 bc pushbutton
        
        WPUB = 0b00010000;                      // pull up for pushbutton on rb4
        nWPUEN = 0;
}

void initVariables(void) {
        uart_rec_buff = ringbufferInit(&uart_rec_buff_);
        memset(cmdbuff, 0, ARGBUFFLEN);
        memset(arg0buff, 0, ARGBUFFLEN);
        memset(arg1buff, 0, ARGBUFFLEN);
        status = 0;
        usestr = 0;
}

void initUART(void) {
        RXDTSEL = 0; // select receive pin
        TXCKSEL = 0; // select transmit pin
        
        CREN = 1; //rc en
        TXEN = 1; //tm en
        SYNC = 0; //no clk
        
        //Set Baudrate for BT Module to 9600
        BRGH = 0;   //high f
        BRG16 = 1;  //16 bit res instead of 8
        SPBRG = 25; //Datasheet Page 299
        
        SPEN = 1; //enable uart
        
        while (RCIF)
                RCREG;
}

void initTMR6(void) {
        PR6 = 0xff;                             // for 1024 bit pwm
        T6CON = 0b00000011;                     // set prescaler (current 64)(bit 0 and 1) see data sheet page 191
        T6CON |= 0x04;                          // turn the timer on
        
        // these settings result in a 16ms duty cycle when FOSC is set to 4Mhz (pre/(FOSC/4))*1024 = duty time
}

void setDutyPWM1(uint16_t duty) {
#ifdef USE_PWM_LOCK
        if (duty < PWM_MIN)
                duty = PWM_MIN;
        else if (duty > PWM_MAX)
                duty = PWM_MAX;
#else
        if (duty > 0x03ff)
                duty = 0x03ff;
#endif
        
        CCP1CON &= ~0b00110000;                 //clear lsbs
        CCP1CON |= (duty & 0x02) << 4 ;         //lsbs
        CCPR1L = (uint8_t) (duty >> 2);
}

// using tmr 6
void initPWM1(void) {
        // see data sheet page 209 for config instructions
        TRISB |= 1 << 3;
        CCP1CON = 0b00001100;                   // enter pwm mode
        setDutyPWM1(0);                        // set the duty
        CCPTMRS |= 0x02 << 0;                   // choose the timer; 0x00 -> tmr2, 0x01 -> tmr4, 0x02 -> tmr6
        TMR6IF = 0;                             // clear int flag for full pwm cycle
        while(!TMR6IF);                         // wait until reset
        TRISB &= ~(1 << 3);
}

void setDutyPWM2(uint16_t duty) {
#ifdef USE_PWM_LOCK
        if (duty < PWM_MIN)
                duty = PWM_MIN;
        else if (duty > PWM_MAX)
                duty = PWM_MAX;
#else
        if (duty > 0x03ff)
                duty = 0x03ff;
#endif
        
        CCP2CON &= ~0b00110000;                 //clear lsbs
        CCP2CON |= (duty & 0x02) << 4 ;         //lsbs
        CCPR2L = (uint8_t) (duty >> 2);
}

// using tmr 6
void initPWM2(void) {
        // see data sheet page 209 for config instructions
        CCP2SEL = 1;
        TRISA |= 1 << 7;
        CCP2CON = 0b00001100;                   // enter pwm mode
        setDutyPWM2(0);                         // set the duty
        CCPTMRS |= 0x02 << 2;                   // choose the timer; 0x00 -> tmr2, 0x01 -> tmr4, 0x02 -> tmr6
        TMR6IF = 0;                             // clear int flag for full pwm cycle
        while(!TMR6IF);                         // wait until reset
        TRISA &= ~(1 << 7);
}

void setDutyPWM3(uint16_t duty) {
#ifdef USE_PWM_LOCK
        if (duty < PWM_MIN)
                duty = PWM_MIN;
        else if (duty > PWM_MAX)
                duty = PWM_MAX;
#else
        if (duty > 0x03ff)
                duty = 0x03ff;
#endif
        
        CCP3CON &= ~0b00110000;                 //clear lsbs
        CCP3CON |= (duty & 0x02) << 4 ;         //lsbs
        CCPR3L = (uint8_t) (duty >> 2);
}

// using tmr 6
void initPWM3(void) {
        // see data sheet page 209 for config instructions
        TRISA |= 1 << 3;
        CCP3CON = 0b00001100;                   // enter pwm mode
        setDutyPWM3(0);                        // set the duty
        CCPTMRS |= 0x02 << 4;                   // choose the timer; 0x00 -> tmr2, 0x01 -> tmr4, 0x02 -> tmr6
        TMR6IF = 0;                             // clear int flag for full pwm cycle
        while(!TMR6IF);                         // wait until reset
        TRISA &= ~(1 << 3);
}

void setDutyPWM4(uint16_t duty) {
#ifdef USE_PWM_LOCK
        if (duty < PWM_MIN)
                duty = PWM_MIN;
        else if (duty > PWM_MAX)
                duty = PWM_MAX;
#else
        if (duty > 0x03ff)
                duty = 0x03ff;
#endif
        
        CCP4CON &= ~0b00110000;                 //clear lsbs
        CCP4CON |= (duty & 0x02) << 4 ;         //lsbs
        CCPR4L = (uint8_t) (duty >> 2);
}

// using tmr 6
void initPWM4(void) {
        // see data sheet page 209 for config instructions
        TRISA |= 1 << 4;
        CCP4CON = 0b00001100;                   // enter pwm mode
        setDutyPWM3(0);                         // set the duty
        CCPTMRS |= 0x02 << 6;                   // choose the timer; 0x00 -> tmr2, 0x01 -> tmr4, 0x02 -> tmr6
        TMR6IF = 0;                             // clear int flag for full pwm cycle
        while(!TMR6IF);                         // wait until reset
        TRISA &= ~(1 << 4);
}

void initInt(void) {
        RCIE = 1;
        
        PEIE = 1;
        GIE = 1;
}

void putc(char c) {
        TXREG = c;
        while(!TRMT);
}

void putstr(const char *c) {
        while (*c) {
                putc(*c);
                c++;
        }
}