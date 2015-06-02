#include "c_types.h"
#include "user_interface.h"
#include "espconn.h"
#include "mem.h"
#include "osapi.h"
#include "gpio.h"
#include "espmissingincludes.h"

#include "relay.h"

#define RELAYN_TO_RELAYID(relay) (relay == RELAY0 ? 0 : 1)

relayState_t relayState[MAX_RELAYS];

// this needs to be reworked to have a run time configuration
void relayInit(void) {
    uint8_t i;

    // setup our relay pins as outputs
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
    gpio_output_set(0, 0, (1 << RELAY0_GPIO), (1 << RELAY1_GPIO));

    for (i = 0; i < MAX_RELAYS; ++i) {
        relayState[i].configured = true;
        relayState[i].on = false;
    }
    // todo, implement latching
}

void ICACHE_FLASH_ATTR relaySet(relay_t relay, bool on) {
    if (on) {
        gpio_output_set((1 << relay), 0, (1 << relay), 0);
        relayState[RELAYN_TO_RELAYID(relay)].on = true;
    } else {
        gpio_output_set(0, (1 << relay), (1 << relay), 0);
        relayState[RELAYN_TO_RELAYID(relay)].on = false;
    }
}

bool ICACHE_FLASH_ATTR relayGet(relay_t relay) {
    return relayState[RELAYN_TO_RELAYID(relay)].on;
}

bool ICACHE_FLASH_ATTR relayIsConfigured(relay_t relay) {
    return relayState[RELAYN_TO_RELAYID(relay)].configured;
}
