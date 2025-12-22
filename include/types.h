#ifndef TYPES_H
#define TYPES_H

/*=========================================================
 * Fixed-width integer types
 * (Do NOT rely on stdint.h in early bare-metal code)
 *=========================================================
 */

typedef unsigned char       uint8_t;
typedef signed char         int8_t;

typedef unsigned short      uint16_t;
typedef signed short        int16_t;

typedef unsigned int        uint32_t;
typedef signed int          int32_t;

typedef unsigned long long  uint64_t;
typedef signed long long    int64_t;

/*=========================================================
 * Boolean type
 *=========================================================
 */
typedef enum
{
    FALSE = 0,
    TRUE  = 1
} bool_t;

/*=========================================================
 * Size / address types
 *=========================================================
 */
typedef uint32_t size_t;
typedef uint32_t addr_t;

/*=========================================================
 * NULL definition
 *=========================================================
 */
#ifndef NULL
#define NULL ((void *)0)
#endif

/*=========================================================
 * Compiler / architecture helpers
 *=========================================================
 */

/* Prevent unused variable warnings */
#define UNUSED(x) ((void)(x))

/* Align variable or structure */
#define ALIGN(x) __attribute__((aligned(x)))

/* Force inline (useful in scheduler / context switch paths) */
#define FORCE_INLINE __attribute__((always_inline)) inline

/* Memory barrier (ARM) */
static inline void memory_barrier(void)
{
    __asm__ volatile ("dmb sy" ::: "memory");
}

/* Disable IRQs */
static inline void irq_disable(void)
{
    __asm__ volatile ("cpsid i" ::: "memory");
}

/* Enable IRQs */
static inline void irq_enable(void)
{
    __asm__ volatile ("cpsie i" ::: "memory");
}

#endif /* TYPES_H */
