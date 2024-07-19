#ifndef _RPM_MEASUREMENT_
#define _RPM_MEASUREMENT_
#include "constants.h"
#include <avr/io.h>

void rpm_measurement_init();
uint16_t rpm_measurement_get();
#endif