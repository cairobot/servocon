/* 
 * file:        stdfc.c
 * author:      redxef
 * date:        October 4, 2016, 10:56 AM
 * version:     0.0.0
 * 
 * desc:
 * changes:
 */

#include "stdfc.h"

void memset(uint8_t *str, char val, uint8_t len) {
        uint8_t i;
        for (i = 0; i < len; i++)
                str[i] = val;
}

uint8_t ccat(char *str, uint8_t len, char append) {
        uint8_t i;

        for (i = 0; i < len - 1; i++) {
                if (str[i] == '\0') {
                        str[i] = append;
                        str[i + 1] = '\0';
                        return 0;
                }
        }
        return 1;
}

uint8_t strcmp(const char *str0, char *str1) {
        while (*str0 && *str1) {
                if (*str0 != *str1)
                        return 0;
                str0++;
                str1++;
        }

        if (*str0 != *str1)
                return 0;

        return 1;
}

uint8_t strlen(char *str) {
        uint8_t i = 0;
        while (*str) {
                i++;
                str++;
        }
        return i;
}

void rev(char *str) {
        uint8_t i, k, len, temp;
        len = strlen(str);
        for (i = 0, k = len - 1; i < (len / 2); i++, k--) {
                temp = str[k];
                str[k] = str[i];
                str[i] = temp;

        }
}

uint32_t str2_to_int(char *str) {
        uint32_t res = 0;
        while (*str) {
                res <<= 1;
                res |= (*str == '1') ? 1 : 0;
                str++;
        }
        return res;
}

int32_t str8_to_int(char *str) {
        uint8_t i = 0;
        int32_t res = 0;
        while (str[i]) {
                if (str[i] >= '0' && str[i] <= '7') {
                        res <<= 3;
                        res += str[i] - '0';
                }
                i++;
        }
        return res;
}

int32_t str10_to_int(char *str) {
        uint8_t i = 0;
        int32_t res = 0;
        while (str[i]) {
                if (str[i] >= '0' && str[i] <= '9') {
                        res *= 10;
                        res += str[i] - '0';
                }
                i++;
        }
        return res;
}

int32_t str16_to_int(char *str) {
        uint8_t i = 0;
        int32_t res = 0;
        while (str[i]) {
                res <<= 4;
                if (str[i] >= '0' && str[i] <= '9') {
                        res += str[i] - '0';
                } else if (str[i] >= 'a' && str[i] <= 'f') {
                        res += str[i] - 'a';
                } else if (str[i] >= 'A' && str[i] <= 'F') {
                        res += str[i] - 'A';
                }
                i++;
        }
        return res;
}

void int_to_str2(char *str, uint32_t i, uint8_t until) {
        uint8_t j = 0;
        while (until--) {
                str[j++] = '0' + (i & 0x00000001);
                i >>= 1;
        }
        str[j] = 0;
        rev(str);
}

void int_to_str8(char *str, int32_t i) {
        uint8_t j = 0;
        uint8_t neg = (i < 0) ? 1 : 0;
        if (i < 0) i = -i;
        while (i > 0) {
                str[j++] = '0' + (i % 8);
                i >>= 3;
        }
        if (neg)
                str[j++] = '-';
        str[j] = 0;
        rev(str);
}

void int_to_str10(char *str, int32_t i) {
        uint8_t j = 0;
        uint8_t neg = (i < 0) ? 1 : 0;
        if (i < 0) i = -i;
        while (i != 0) {
                str[j++] = '0' + (i % 10);
                i /= 10;
        }
        if (neg)
                str[j++] = '-';
        str[j] = 0;
        rev(str);
}

void int_to_str16(char *str, int32_t i) {
        uint8_t j = 0;
        uint8_t neg = (i < 0) ? 1 : 0;
        uint8_t num;
        if (i < 0) i = -i;
        while (i != 0) {
                num = i % 16;
                if (num >= 10)
                        str[j++] = 'a' + num - 10;
                else
                        str[j++] = '0' + num;
                i >>= 4;
        }
        if (neg)
                str[j++] = '-';
        str[j] = 0;
        rev(str);
}