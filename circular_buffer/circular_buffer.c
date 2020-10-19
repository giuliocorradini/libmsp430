/*
 * circular_buffer.c
 *
 *  Created on: 17/ago/2015
 *      Author: Paolo Santinelli
 */


//--------------------------------------------------------------------------------------
#include <stdlib.h>
#include <stdbool.h>
#include "circular_buffer.h"

/* Circular buffer */
//typedef struct {                                        /* Circular buffer struct */
//    int                 size;   /* maximum number of elements         */
//    int                 w_pos;  /* write index                        */
//    int                 r_pos;  /* read index			        */
//    int                 count;  /* number of items in the buffer      */
//    unsigned char       *buff;  /* vector of elements                 */
//} circular_buffer_t;

/** 
 * Inizialize circular buffer 
 */
void cbInit(circular_buffer_t *cb, uint16_t size) 
{             
    cb->size  = size;
    cb->w_pos = 0;
    cb->r_pos = 0;
    cb->count = 0;
    cb->buff  = (uint8_t*)malloc(cb->size * sizeof(uint8_t));
}


/**
 * Free circular buffer 
 */
inline void cbFree(circular_buffer_t *cb)
{                       
    free(cb->buff); /* OK if null */
}


/** 
 * Check circular buffer full condition 
 */
inline int cbIsFull(circular_buffer_t *cb) 
{                      
    return cb->count == cb->size;
}


/** 
 * Check circular buffer empty condition 
 */
inline int cbIsEmpty(circular_buffer_t *cb) 
{                     
    return cb->count == 0;
}


/** 
 * Write data in to circular buffer 
 */
bool cbWrite(circular_buffer_t *cb, uint8_t elem) 
{   
    if (cb->count >= cb->size)
        return false;

    cb->count++;
    cb->buff[cb->w_pos] = elem;

    // cb->w_pos = (cb->w_pos+1) % cb->size;
    cb->w_pos = 
        (++cb->w_pos < cb->size) 
        ? cb->w_pos 
        : 0; 
 
    return true;
}


/** 
 * Read data from circular buffer 
 */
bool cbRead(circular_buffer_t *cb, uint8_t *elem) 
{   
    if (cb->count <= 0)
        return false;
    
    cb->count--;
    *elem = cb->buff[cb->r_pos];

    // cb->r_pos = (cb->r_pos+1) % cb->size;
    cb->r_pos = 
        (++cb->r_pos < cb->size) 
        ? cb->r_pos
        : 0; 

    return true; 
}
//------------------------------------------------------------------------------------
