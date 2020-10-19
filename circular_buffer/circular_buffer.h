/*
 * circular_buffer.h
 *
 *  Created on: 17/ago/2015
 *      Author: Paolo Santinelli
 */

#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#include <stdbool.h>

/** 
 * Circular buffer struct 
 */
typedef struct {
    uint16_t           size;    // maximum number of elements   
    uint16_t           w_pos;   // write index                  
    uint16_t           r_pos;   // read index				    
    uint16_t           count;   // number of items in the buffer
    unsigned char *buff;    // vector of elements       
} circular_buffer_t;

void cbFree   (circular_buffer_t *cb);
int  cbIsFull (circular_buffer_t *cb);
int  cbIsEmpty(circular_buffer_t *cb);
void cbInit   (circular_buffer_t *cb, unsigned int size);
bool cbWrite  (circular_buffer_t *cb, uint8_t  elem);
bool cbRead   (circular_buffer_t *cb, uint8_t *elem);

#endif /* CIRCULAR_BUFFER_H_ */