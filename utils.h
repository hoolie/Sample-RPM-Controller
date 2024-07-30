
// utils.h - Utility macros and inline functions for the ATmega328P
// This header file contains macros and inline functions to constrain values 
// within specified bounds for various data types.

// Include guard to prevent multiple inclusions of this header file
#ifndef _UTILS_
#define _UTILS_

// Include the I/O definitions for the AVR microcontroller
#include <avr/io.h>

// Macro to constrain a value within a specified range [lower, upper].
// If the value is greater than the upper bound, it returns the upper bound.
// If the value is less than the lower bound, it returns the lower bound.
// Otherwise, it returns the value itself.
#define CONSTRAIN(val, lower, upper) ((val) > (upper) ? (upper) : ((val) < (lower) ? (lower) : (val)))

// Inline function to constrain an 8-bit unsigned integer (uint8_t)
// within a specified range [lower_bound, upper_bound].
static inline uint8_t constrain_uint8_t(uint8_t value, uint8_t lower_bound, uint8_t upper_bound) {
    return CONSTRAIN(value, lower_bound, upper_bound);
}

// Inline function to constrain a 16-bit signed integer (int16_t)
// within a specified range [lower_bound, upper_bound].
static inline int16_t constrain_int16_t(int16_t value, int16_t lower_bound, int16_t upper_bound) {
    return CONSTRAIN(value, lower_bound, upper_bound);
}

// Inline function to constrain a 16-bit unsigned integer (uint16_t)
// within a specified range [lower_bound, upper_bound].
static inline uint16_t constrain_uint16_t(uint16_t value, uint16_t lower_bound, uint16_t upper_bound) {
    return CONSTRAIN(value, lower_bound, upper_bound);
}

// Inline function to constrain a 32-bit signed integer (int32_t)
// within a specified range [lower_bound, upper_bound].
static inline int32_t constrain_int32_t(int32_t value, int32_t lower_bound, int32_t upper_bound) {
    return CONSTRAIN(value, lower_bound, upper_bound);
}

// End of the include guard
#endif
