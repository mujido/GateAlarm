#include <atmel_start.h>
#include <util/delay.h>
#include <avr/sleep.h>

extern volatile bool gate_open;

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

static void setup_wait_for_gate_open()
{
    // interrupt when gate open for threshold time
}

static void setup_wait_for_gate_close()
{
    // interrupt when gate closed for threshold time
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

    //sleepctrl_set_powerdown_mode();
    //GATE_set_isc(PORT_ISC_enum)
	
	/* Replace with your application code */
	while (1) {
		ESPPWR_set_level(!gate_open);
        sleep_cpu();
	}
}
