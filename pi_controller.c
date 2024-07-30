/**
 * @file pi_controller.c
 * @brief PI controller implementation.
 */

#include "pi_controller.h"
#include "utils.h"

/**
 * @brief Creates a new PI controller state.
 * 
 * @param kp Proportional gain
 * @param ki Integral gain
 * @return ControllerState Initialized controller state
 */
ControllerState createNew(uint8_t kp, uint8_t ki){
    ControllerState newState;
    newState.kp = kp;
    newState.ki = ki;
    return newState;
}

/**
 * @brief Runs the PI controller.
 * 
 * @param state Current controller state
 * @param actual Actual value
 * @param target Target value
 * @return ControllerState Updated controller state
 */
ControllerState pi_controller_run(ControllerState state, uint16_t actual, uint16_t target)
{
    state.e = target - actual;
    state.esum += state.e;
    state.esum = constrain_int16_t(state.esum, -5000, 5000);

    state.p = ((int32_t)state.e * state.kp) >> 6;
    state.i = ((int32_t)state.esum * state.ki) >> 6;

    state.output = state.p + state.i;
    return state;
}