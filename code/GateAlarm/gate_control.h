/*
 * gate_control.h
 *
 * Created: 10/27/2019 2:34:56 PM
 *  Author: kurt
 */ 


#ifndef GATE_CONTROL_H_
#define GATE_CONTROL_H_

#include <atmel_start_pins.h>

typedef enum
{
    GATE_INIT,
    GATE_OPEN_TIME_WAIT,
    GATE_OPEN,
    GATE_CLOSED
} GateState;

extern volatile GateState gate_current_state;

static inline void gate_disable_open_timer(void)
{
    TCB0.CTRLA &= ~TCB_ENABLE_bm;
}

static inline void gate_enable_open_timer(void)
{
    TCB0.CNT = 0;
    TCB0.CTRLA |= TCB_ENABLE_bm;
}

static inline GateState gate_get_actual_state(void)
{
    return GATE_get_level() ? GATE_OPEN : GATE_CLOSED;
}

void gate_ctrl_switch_state(GateState state);

#endif /* GATE_CONTROL_H_ */