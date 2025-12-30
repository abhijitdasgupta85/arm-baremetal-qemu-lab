#ifndef RINGBUF_H
#define RINGBUF_H

#include "types.h"

#define RINGBUF_SIZE 128

typedef struct {
    char buf[RINGBUF_SIZE];
    uint32_t head;
    uint32_t tail;
} ringbuf_t;

void ringbuf_init(ringbuf_t *rb);
void ringbuf_put(ringbuf_t *rb, char c);
int32_t ringbuf_get(ringbuf_t *rb, char *c);

#endif
