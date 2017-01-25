/* 
 * file:        ringbuffer.c
 * author:      redxef
 * date:        September 13, 2016, 2:38 PM
 * version:     0.1.0
 * 
 * desc:
 * Implementation for the ringbuffer.
 */

#include "ringbuffer.h"

struct ringbuffer *ringbufferInit(struct ringbuffer *rb) {
        if (rb == NULL)
                return NULL;
        rb->pp = 0;
        rb->gp = 0;
        
        return rb;
}

void ringbufferDelete(struct ringbuffer **rb) {
        //free(*rb);
        //*rb = NULL;
}

uint8_t ringbufferSize(struct ringbuffer *rb) {
        return RB_BUFFER_SIZE;
}

uint8_t ringbufferPut(struct ringbuffer *rb, uint8_t dat) {
        #ifdef DEBUG
        printf("pp: %d, gp: %d\n", rb->pp, rb->gp);
        printf("first: %d\n", (rb->pp >= rb->gp || rb->pp < rb->gp - 1));
        printf("second: %d\n", (rb->pp != ringbufferSize(rb) - 1 || rb->gp != 0));
        #endif
        if ((rb->pp >= rb->gp || rb->pp < rb->gp - 1) && (rb->pp != ringbufferSize(rb) - 1 || rb->gp != 0)) {
                rb->dat[rb->pp] = dat;
                rb->pp += 1;
                if (rb->pp == ringbufferSize(rb))
                        rb->pp = 0;
                return 0;
        }
        return 1;
}

int16_t ringbufferGet(struct ringbuffer *rb) {
        int16_t ret = -1;
        if (rb->gp < rb->pp || rb->gp > rb->pp) { // either the put-pointer is behind or ahead, if it is at the same pos, we have a problem
                ret = rb->dat[rb->gp];
                rb->gp += 1;
                if (rb->gp == ringbufferSize(rb))
                        rb->gp = 0;
        }

        return ret;

}