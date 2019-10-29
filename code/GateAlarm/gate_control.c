/*
 * gate_control.c
 *
 * Created: 10/27/2019 2:35:27 PM
 *  Author: kurt
 */ 

#include "gate_control.h"

void gate_ctrl_switch_state(GateState state)
{
    if (state == gate_current_state)
        return;

    switch (state)
    {
        case GATE_OPEN_TIME_WAIT:
            gate_enable_open_timer();
            GATE_set_isc(PORT_ISC_INTDISABLE_gc);
            break;

        case GATE_OPEN:
            gate_current_state = GATE_OPEN;
            gate_disable_open_timer();
            GATE_set_isc(PORT_ISC_INTDISABLE_gc);
            break;            

        case GATE_CLOSED:
            gate_current_state = GATE_CLOSED;
            gate_disable_open_timer();
            GATE_set_isc(PORT_ISC_BOTHEDGES_gc);

        default:
            break;
    }
}

