#ifndef RINGBUF_H
#define RINGBUF_H

#include "types.h"

#define RINGBUF_SIZE 128

/*Buffer structure*/
typedef struct {
    volatile uint32_t head;
    volatile uint32_t tail;
    char buf[RINGBUF_SIZE];
}ringbuf_t;

void ringbuf_init(ringbuf_t *rb);
void ringbuf_put(ringbuf_t *rb, char c);
int32_t ringbuf_get(ringbuf_t *rb, char *c);

#endif

