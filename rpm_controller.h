/**
 * @file rpm_controller.h
 * @brief PI Controller for RPM running on TC0 Overflow interupt.
 */
#ifndef _RPM_CONTROLLER_
#define _RPM_CONTROLLER_
#include "pi_controller.h"
/**
 * @brief Initializes the rpm_controller with proportional and integral gain.
 * 
 * @param kp proportional gain
 * @param ki integral gain
 */
void rpm_controller_init(uint8_t kp, uint8_t ki);

/**
 * @brief returns a copy of the current controller state
 */
ControllerState rpm_controller_get_state();

/**
 * @brief set the target rpm
 * @param rpm target rpm 
 */
void rpm_controller_set_target_rpm(int rpm);

/**
 * @brief get the current target rpm
 * @return the current target rpm
 */
uint16_t rpm_controller_get_target_rpm();
/**
 * @brief increase the target rpm by 1
 */
void rpm_controller_inc();

/**
 * @brief decrease the target rpm by 1
 */
void rpm_controller_dec();
#endif // _RPM_CONTROLLER_