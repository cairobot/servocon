/* 
 * file:        main_v_1_0.c
 * author:      redxef
 * date:        November 12, 2016, 11:08 AM
 * version:     0.0.0
 * 
 * desc:
 * changes:
 */

#include "main_base.h"

uint16_t dat0 = -1;
uint16_t dat1 = -1;

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

void interrupt ISR() {
        if (RCIE && RCIF) {
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
}

int main(int argc, char **argv) {
        initFrequency();
        initRegisters();
        initVariables();
        initUART();
        initTMR6();
        initPWM1();
        initPWM2();
        initPWM3();
        initPWM4();
        initInt();
        setDutyPWM1(PWM_MID);
        setDutyPWM2(PWM_MID);
        setDutyPWM3(PWM_MID);
        setDutyPWM4(PWM_MID);

        while (1) {}
}