#include "ringbuf.h"

void ringbuf_init(ringbuf_t *rb)
{
    rb->head = 0;
    rb->tail = 0;
}

void ringbuf_put(ringbuf_t *rb, char c)
{
    uint32_t next = (rb->head + 1) % RINGBUF_SIZE;

    if (next != rb->tail) {  // drop if full
        rb->buf[rb->head] = c;
        rb->head = next;
    }
}

int32_t ringbuf_get(ringbuf_t *rb, char *c)
{
    if (rb->head == rb->tail)
        return 0;   // empty

    *c = rb->buf[rb->tail];
    rb->tail = (rb->tail + 1) % RINGBUF_SIZE;
    return 1;
}
