#include <atmel_start.h>
#include <util/delay.h>
#include <avr/sleep.h>

#include "rtctime.h"
#include "gate_control.h"

volatile GateState gate_current_state = GATE_INIT;

static inline void sleepctrl_set_standby_mode()
{
	SLPCTRL.CTRLA = 1 << SLPCTRL_SEN_bp       /* Sleep enable: enabled */
			        | SLPCTRL_SMODE_STDBY_gc; /* Standby Mode */
}

static inline void sleepctrl_set_powerdown_mode()
{
	SLPCTRL.CTRLA = 1 << SLPCTRL_SEN_bp       /* Sleep enable: enabled */
 	                | SLPCTRL_SMODE_PDOWN_gc; /* Power Down Mode */
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

    //sleepctrl_set_powerdown_mode();
    //GATE_set_isc(PORT_ISC_enum)

    PA1_set_dir(PORT_DIR_OUT);
    PA1_set_level(true);

    SHUTDOWN_set_dir(PORT_DIR_IN);
    while (!SHUTDOWN_get_level())
        ;

    SHUTDOWN_set_isc(PORT_ISC_FALLING_gc);

    gate_ctrl_switch_state(gate_get_actual_state());

	while (1) {
		ESPPWR_set_level(gate_current_state == GATE_OPEN);
        sleep_cpu();
	}
}
