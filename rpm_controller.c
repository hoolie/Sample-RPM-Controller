/**
 * @file rpm_controller.c
 * @brief PI Controller for RPM running on TC0 Overflow interupt.
 */

#include <avr/interrupt.h>
#include "pi_controller.h"
#include "rpm_measurement.h"
#include "pwm.h"
#include "utils.h"

volatile uint16_t target_rpm = 0; /**< Target RPM value */
ControllerState rpmState; /**< PI controller state */

#define MAX_OUTPUT 255 /**< Maximum controller output */
#define MIN_OUTPUT 0   /**< Minimum controller output */

/**
 * @brief Initializes the rpm_controller with proportional and integral gain.
 * 
 * @param kp proportional gain
 * @param ki integral gain
 */
void rpm_controller_init(uint8_t kp, uint8_t ki) {
    rpmState = createNew(64, 4);
}

/**
 * @brief returns a copy of the current controller state
 */
ControllerState rpm_controller_get_state() {
    return rpmState;
}

/**
 * @brief set the target rpm
 * @param rpm target rpm 
 */
void rpm_controller_set_target_rpm(int rpm) {
    target_rpm = constrain_int16_t(rpm, 0, 400);
}

/**
 * @brief get the current target rpm
 * @return the current target rpm
 */
uint16_t rpm_controller_get_target_rpm() {
    return target_rpm;
}

/**
 * @brief increase the target rpm by 1
 */
void rpm_controller_inc() {
    rpm_controller_set_target_rpm(target_rpm+1);
}

/**
 * @brief decrease the target rpm by 1
 */
void rpm_controller_dec() {
    rpm_controller_set_target_rpm((int)(target_rpm)-1);
}
/**
 * @brief Runs the RPM controller.
 * 
 * @param target_rpm Target RPM value
 * @param current_rpm Current RPM value
 * @return uint8_t Controller output
 */
uint8_t rpm_controller(uint16_t target_rpm, uint16_t current_rpm)
{
    rpmState = pi_controller_run(rpmState, current_rpm, target_rpm);
    return (uint8_t)constrain_int16_t(rpmState.output, MIN_OUTPUT, MAX_OUTPUT);
}

/**
 * @brief Timer0 overflow interrupt service routine.
 */
ISR(TIMER0_OVF_vect)
{
    uint16_t rpm = rpm_measurement_get();
    uint8_t duty_cycle = rpm_controller(target_rpm, rpm);
    pwm_set(duty_cycle);
}
