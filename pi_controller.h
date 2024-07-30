/**
 * @file pi_controller.h
 * @brief Header file for the PI controller.
 */

#ifndef _PI_CONTROLLER_
#define _PI_CONTROLLER_

#include <avr/io.h>

/**
 * @brief Structure to hold the state of the PI controller.
 */
typedef struct {
    uint8_t kp;       /**< Proportional gain */
    uint8_t ki;       /**< Integral gain */
    int16_t esum;     /**< Sum of errors */
    int16_t e;        /**< Current error */
    int32_t p;        /**< Proportional term */
    int32_t i;        /**< Integral term */
    int16_t output;   /**< Controller output */
} ControllerState;

/**
 * @brief Creates a new PI controller state.
 * 
 * @param kp Proportional gain
 * @param ki Integral gain
 * @return ControllerState Initialized controller state
 */
ControllerState createNew(uint8_t kp, uint8_t ki);

/**
 * @brief Runs the PI controller.
 * 
 * @param state Current controller state
 * @param actual Actual value
 * @param target Target value
 * @return ControllerState Updated controller state
 */
ControllerState pi_controller_run(ControllerState state, uint16_t actual, uint16_t target);

#endif // _PI_CONTROLLER_