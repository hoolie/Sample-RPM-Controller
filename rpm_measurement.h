/**
 * @file rpm_measurement.h
 * @brief Header file for RPM measurement functions.
 */

#ifndef _RPM_MEASUREMENT_
#define _RPM_MEASUREMENT_

#include "constants.h"
#include <avr/io.h>

/**
 * @brief Initializes RPM measurement.
 */
void rpm_measurement_init(void);

/**
 * @brief Gets the current RPM.
 * 
 * @return uint16_t Current RPM value
 */
uint16_t rpm_measurement_get(void);

#endif // _RPM_MEASUREMENT_