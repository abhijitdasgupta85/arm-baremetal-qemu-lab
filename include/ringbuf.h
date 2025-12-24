#ifndef RINGBUF_H
#define RINGBUF_H

#include "types.h"

/*2.1 Why ring buffers are mandatory
Without ring buffers:
    RX data is lost
    UART blocks execution
    Simulation becomes impossible

With ring buffers:
    ISR/poller writes fast
    Application reads later
    No blocking
    Deterministic behavior
*/

#define RINGBUF_SIZE 128

/*Buffer structure*/
typedef struct {
    volatile uint32_t head;
    volatile uint32_t tail;
    char buf[RINGBUF_SIZE];
}ringbuf_t;


/* ================= Ring Buffer Helpers ================= */
static inline int32_t ringbuf_is_empty(ringbuf_t *rb)
{
    return rb->head == rb->tail;
}

static inline int32_t ringbuf_is_full(ringbuf_t *rb)
{
    return ((rb->head + 1) % RINGBUF_SIZE) == rb->tail;
}

static inline void ringbuf_put(ringbuf_t *rb, char c)
{
    if (!ringbuf_is_full(rb)) {
        rb->buf[rb->head] = c;
        rb->head = (rb->head + 1) % RINGBUF_SIZE;
    }
}

static inline int32_t ringbuf_get(ringbuf_t *rb, char *c)
{
    if (ringbuf_is_empty(rb))
        return 0;

    *c = rb->buf[rb->tail];
    rb->tail = (rb->tail + 1) % RINGBUF_SIZE;
    return 1;
}
#endif

