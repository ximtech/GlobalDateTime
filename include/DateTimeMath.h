#pragma once

#include <stdint.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

static inline int64_t minInt64(int64_t x, int64_t y) {
    return ((x < y) ? x : y);
}

static inline int64_t absInt64(int64_t x) {
    return ((x < 0) ? (-x) : x);
}

static inline int8_t int64Compare(int64_t x, int64_t y) {
    if (x < y) return -1;
    return x == y ? 0 : 1;
}

static inline int8_t int32Compare(int32_t x, int32_t y) {
    return (int8_t) ((x > y) - (x < y));
}

static inline int64_t floorDiv(int64_t x, int64_t y) {
    int64_t result = x / y;
    // if the signs are different and modulo not zero, round down
    if ((x ^ y) < 0 && (result * y != x)) {
        result--;
    }
    return result;
}

static inline int64_t floorMod(int64_t x, int64_t y) {
    int64_t mod = x % y;
    // if the signs are different and modulo not zero, adjust result
    if ((x ^ y) < 0 && mod != 0) {
        mod += y;
    }
    return mod;
}

static inline int64_t addExact(int64_t x, int64_t y) {
    int64_t result = x + y;
    // HD 2-12 Overflow if both arguments have the opposite sign of the result
    if (((x ^ result) & (y ^ result)) < 0) {
        return 0;
    }
    return result;
}

static inline int64_t multiplyExact(int64_t x, int64_t y) {
    int64_t result = x * y;
    int64_t ax = absInt64(x);
    int64_t ay = absInt64(y);
    if (((ax | ay) >> 31 != 0)) {
        // Some bits greater than 2^31 that might cause overflow
        // Check the result using the divide operator
        // and check for the special case of INT64_MIN * -1
        if (((y != 0) && (result / y != x)) || (x == INT64_MAX && y == -1)) {
            return 0;
        }
    }
    return result;
}

