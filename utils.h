#ifndef _UTILS_
#define _UTILS_
#include <avr/io.h>

// Macro for constraining values of any comparable type
#define CONSTRAIN(val, lower, upper) ((val) > (upper) ? (upper) : ((val) < (lower) ? (lower) : (val)))

// Inline function declarations
static inline uint8_t constrain_uint8_t(uint8_t value, uint8_t lower_bound, uint8_t upper_bound) {
    return CONSTRAIN(value, lower_bound, upper_bound);
}

static inline int16_t constrain_int16_t(int16_t value, int16_t lower_bound, int16_t upper_bound) {
    return CONSTRAIN(value, lower_bound, upper_bound);
}

static inline uint16_t constrain_uint16_t(uint16_t value, uint16_t lower_bound, uint16_t upper_bound) {
    return CONSTRAIN(value, lower_bound, upper_bound);
}

static inline int32_t constrain_int32_t(int32_t value, int32_t lower_bound, int32_t upper_bound) {
    return CONSTRAIN(value, lower_bound, upper_bound);
}

#endif
