/* 
 * file:        stdfc.h
 * author:      redxef
 * date:        October 4, 2016, 10:57 AM
 * version:     0.0.0
 * 
 * desc:
 * changes:
 */

#ifndef __STDFC_H_
#define	__STDFC_H_

/******************************************************************************/
/* INCLUDES *******************************************************************/
/******************************************************************************/
#include <stdint.h>

/******************************************************************************/
/* PUBLIC VARS ****************************************************************/
/******************************************************************************/

/******************************************************************************/
/* FUNCTION PROTOTYPES ********************************************************/
/******************************************************************************/
void memset(uint8_t *str, char val, uint8_t len);
uint8_t ccat(char *str, uint8_t len, char append);
uint8_t strcmp(const char *str0, char *str1);
uint8_t strlen(char *str);
uint32_t str2_to_int(char *str);
int32_t str8_to_int(char *str);
int32_t str10_to_int(char *str);
int32_t str16_to_int(char *str);
void int_to_str2(char *str, uint32_t i, uint8_t until);
void int_to_str8(char *str, int32_t i);
void int_to_str10(char *str, int32_t i);
void int_to_str16(char *str, int32_t i);
#endif

