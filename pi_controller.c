#include "pi_controller.h"
#include "utils.h"

ControllerState createNew(uint8_t kp, uint8_t ki){
 ControllerState newState;
 newState.kp = kp;
 newState.ki = ki;
 //newState.esum = 0;
 //newState.e = 0;
 //newState.p = 0;
 //newState.i = 0;
 //newState.output = 0;
 return newState;
}

ControllerState pi_controller_run(ControllerState state, uint16_t actual, uint16_t target)
{

    state.e = target-actual;
    state.esum +=state.e;
    // todo: extract esum_boundary to state
    state.esum = constrain_int16_t(state.esum,-5000,+5000);

    state.p = ((int32_t)state.e* state.kp) >> 6;
    state.i = ((int32_t)state.esum *state.ki) >> 6;

    // Combine proportional and integral terms
    state.output = state.p + state.i;
    return state;


}