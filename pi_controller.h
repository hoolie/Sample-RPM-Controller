#ifndef _PI_CONTROLLER_
#define _PI_CONTROLLER_
#include <avr/io.h>

typedef struct {
    uint8_t kp;
    uint8_t ki;
    int16_t esum;
    int16_t e;
    int32_t p;
    int32_t i;
    int16_t output;
} ControllerState;

ControllerState createNew(uint8_t kp, uint8_t ki);
ControllerState pi_controller_run(ControllerState state, uint16_t actual, uint16_t target);
#endif