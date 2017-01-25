/* 
 * file:        ringbuffer.h
 * author:      redxef
 * date:        September 29, 2016, 3:21 PM
 * version:     0.1.1
 * 
 * desc:
 * The ringbuffer is used to buffer the data received over an serial interface.
 */

#include <stdint.h>
#include <stdlib.h>
#include <xc.h>

#ifndef __RINGBUFFER_H_
#define __RINGBUFFER_H_

/******************************************************************************/
/* MACROS *********************************************************************/
/******************************************************************************/
#define RB_BUFFER_SIZE          48

/******************************************************************************/
/* PUBLIC VARS ****************************************************************/
/******************************************************************************/

/******************************************************************************/
/* FUNCTION PROTOTYPES ********************************************************/
/******************************************************************************/
/**
 * A struct for storing data in a buffer.
 * The ringbuffer is crudely written with
 * ease of use and simplicity in mind.
 * It can either have a fixed size of data
 * predefined at compile time for every instance,
 * or dynamically when constructing a new one.
 * If the Macro FIXED is defined, then the size
 * is predefined, otherwise not.
 */
struct ringbuffer {
        uint8_t pp;     //put pos
        uint8_t gp;     //get pos
        uint8_t dat[RB_BUFFER_SIZE];
};

/**
 * Initialises a given ringbuffer struct.
 * The buffersize is set via the Macro RB_BUFFER_SIZE.
 *
 * @param rb            The ringbuffer to initialize.
 * @returns             A pointer to the newly initialized struct.
 */
struct ringbuffer *ringbufferInit(struct ringbuffer *rb);

/**
 * Deletes the given ringbuffer struct. The poiunter to pointer
 * style has been used, to set the struct to NULL after deletion
 * for security reasons. Thus, if the struct is deleted, it also
 * says so by being NULL.
 *
 * @param rb            The ringbuffer to be deleted.
 */
void ringbufferDelete(struct ringbuffer **rb);

/**
 * Returns the Size of the ringbuffer (RB_BUFFER_SIZE).
 *
 * @param rb            The ringbuffer from which to get the
 *                      size.
 * @returns             The length of this ringbuffer, where
 *                      this number represents the number of items
 *                      the buffer can hold - 1.
 */
uint8_t ringbufferSize(struct ringbuffer *rb);

/**
 * Puts a new byte of data into the ringbuffer and returns 0 on
 * success. Otherwise the byte will be dropped and 1 returned.
 *
 * @param rb            The ringbuffer, which should receive the
 *                      byte of data.
 * @param dat           The data to feed the ringbuffer.
 * @returns             0 or 1 on success or failure respectively.
 */
uint8_t ringbufferPut(struct ringbuffer *rb, uint8_t dat);

/**
 * Returns the next byte of data from the ringbuffer or -1 if no
 * data is available.
 *
 * @param rb            The buffer from which to get the next byte
 *                      of data.
 * @returns             The next byte in the lower byte of the returned
 *                      value or -1.
 */
int16_t ringbufferGet(struct ringbuffer *rb);

#endif