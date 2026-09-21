#ifndef ATOMICS_H
#define ATOMICS_H

#include <stdint.h>

typedef uint64_t atomic_u64;

static inline void atomic_store_relaxed_u64(atomic_u64 *p, atomic_u64 value) {
    __atomic_store_n(p, value, __ATOMIC_RELAXED);
}

static inline atomic_u64 atomic_load_relaxed_u64(const atomic_u64 *p) {
    return __atomic_load_n(p, __ATOMIC_RELAXED);
}

static inline void atomic_store_release_u64(atomic_u64 *p, atomic_u64 value) {
    __atomic_store_n(p, value, __ATOMIC_RELEASE);
}

static inline atomic_u64 atomic_load_acquire_u64(const atomic_u64 *p) {
    return __atomic_load_n(p, __ATOMIC_ACQUIRE);
}

#endif