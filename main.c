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

#include "main.h"

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
        ANSELB = 0b00010000;
        TRISB  = 0b00010010;                    // rb1 is input, bc eusart rx, rb4 bc pushbutton
        
        WPUB = 0b00000000;                      // pull up for pushbutton on rb4
        nWPUEN = 0;
}

void initVariables(void) {
        dat0 = -1;
        dat1 = -1;
        cycle_adc = 0;
        cycle_timeout = 0;
}

void initUART(void) {
        RXDTSEL = 0; // select receive pin
        TXCKSEL = 0; // select transmit pin
        
        CREN = 1; //rc en
        TXEN = 1; //tm en
        SYNC = 0; //no clk
        
        // set baudrate to 9600 or 115200
#if BAUDRATE == 9600
        BRGH = 0;   // high frequency
        BRG16 = 1;  // 16 or 8 bit resolution?
        SPBRG = 25; // data sheet page 299
#elif BAUDRATE == 115200
        BRGH = 1;
        BRG16 = 1;
        SPBRG = 8;
#endif
        
        SPEN = 1; // enable uart
        
        while (RCIF)
                RCREG;
}

void initTMR4(void) {
        TMR4 = 0x00;
        PR4 = 0xff;
        T4CON = 0b01110111;                     // prescaler of 64, postscaler of 15; --> 246ms cycle = 1/4 sec
        TMR4IF = 0;
}

void initTMR6(void) {
        // these settings result in a 16ms duty cycle when FOSC is set to 4Mhz (pre/(FOSC/4))*1024 = duty time
        PR6 = 0xff;                             // for 1024 bit pwm
        T6CON = 0b111;                          // see data sheet page 191, turn on and prescaler to 64
        TMR6IF = 0;
}

void initADC(void) {
        ADCON0 = 0;
        ADCON1 = 0;
        
        ADCON0 |= 8 << 2;       // set source of conversion to AN8
        ADCON1 |= 0 << 7;       // left justify
        ADCON1 |= 000 << 4;     // conversion clock speed
        ADCON1 |= 0 << 2;       // positive reference
        ADCON1 |= 0;            // negative reference
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
        setDutyPWM1(0);                         // set the duty
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
        TMR4IE = 1;
        RCIE = 1;
        PEIE = 1;
        GIE = 1;
}

void putc(char c) {
        while(!TRMT);
        TXREG = c;
        NOP();
}

void putstr(const char *c) {
        while (*c) {
                putc(*c);
                c++;
        }
}

inline uint8_t getPicAddr(uint8_t d0, uint8_t d1) {
        return (d0 >> 5) & 0b00000011;
}

inline uint8_t getSAddr(uint8_t d0, uint8_t d1) {
        return (d0 >> 3) & 0b00000011;
}

inline uint8_t getMode(uint8_t d0, uint8_t d1) {
        return (d0 >> 1) & 0b00000011;
}

inline uint8_t getData(uint8_t d0, uint8_t d1) {
        uint8_t ret = 0;
        ret |= (d0 & 0b00000001) << 7;
        ret |= (d1 & 0b01111111);
        return ret;
}

inline void setServo(uint8_t id, uint8_t val) {
        switch (id) {
                case 0:
                        setDutyPWM1(val);
                        break;
                case 1:
                        setDutyPWM2(val);
                        break;
                case 2:
                        setDutyPWM3(val);
                        break;
                case 3:
                        setDutyPWM4(val);
                        break;
        }
}

void interrupt ISR(void) {
#if DEBUG != 0
        putstr("> interrupt: ");
#endif
        if (RCIE && RCIF) {
#if DEBUG != 0
                putc('u');
#endif
                if (dat0 == -1) {
                        dat0 = RCREG;
                        if (!(dat0 & 0b10000000)) {
                                dat0 = -1;
                        } else if (getPicAddr((uint8_t) (dat0 & 0xff), (uint8_t) (dat1 & 0xff)) != BDEVID) {
                                dat0 = -1;
                        }
                } else if (dat1 == -1) {
                        dat1 = RCREG;
                        setServo(getSAddr((uint8_t) (dat0 & 0xff), (uint8_t) (dat1 & 0xff)), getData((uint8_t) (dat0 & 0xff), (uint8_t) (dat1 & 0xff)));
                        dat0 = dat1 = -1;
                }
                RCIF = 0;
        }
        if (TMR4IF) {
#if DEBUG != 0
                putc('t');
#endif
                cycle_adc++;
                cycle_timeout++;
                TMR4IF = 0;
        }
#if DEBUG != 0
        putstr("\r\n");
#endif
}

int main(void) {
        uint8_t adc_val;
        uint8_t buf;
        initFrequency();
        initRegisters();
        initVariables();
        initUART();
        initTMR4();
        initTMR6();
        initADC();
        initPWM1();
        initPWM2();
        initPWM3();
        initPWM4();
        setDutyPWM1(0);
        setDutyPWM2(0);
        setDutyPWM3(0);
        setDutyPWM4(0);
        initInt();
        
        while (1) {
                if (cycle_adc == 0xff) {        // every 1 minute (62.7 seconds)
                        // read voltage and set it to main controller
                        ADON = 1;
                        __delay_ms(10);
                        GO_nDONE = 1;
                        while (GO_nDONE);
                        adc_val = ADRESH;
                        ADON = 0;
                        
                        putc('>');
                        buf = (adc_val >> 4) & 0x0f;
                        if (buf >= 0x0a)
                                buf += 'a' - 0x0a;
                        else
                                buf += '0';
                        putc(buf);
                        buf = adc_val & 0x0f;
                        if (buf >= 0x0a)
                                buf += 'a' - 0x0a;
                        else
                                buf += '0';
                        putc(buf);
                        putc('h');
                        putc('<');
                        
                        cycle_adc = 0;
                }
                if (cycle_timeout == 0xff) {    // every 1 minute (62.7 seconds)
                        // turn off servos
                        setDutyPWM1(0);
                        setDutyPWM2(0);
                        setDutyPWM3(0);
                        setDutyPWM4(0);
                        
                        cycle_timeout = 0;
                }
        }
}