#ifndef __RELAY_H__
#define __RELAY_H__

#define MAX_RELAYS  2

#define RELAY0_GPIO 0
#define RELAY1_GPIO 2

#define RELAYN_TO_RELAYID(relay) (relay == RELAY0 ? 0 : 1)

typedef struct relayState {
    bool configured;
    bool on;
} relayState_t;

// these correspond to pin numbers in the defines above
typedef enum relay { RELAY0 = RELAY0_GPIO, RELAY1 = RELAY1_GPIO } relay_t;

void relayInit(void);
void ICACHE_FLASH_ATTR relaySet(relay_t relay, bool on);
bool ICACHE_FLASH_ATTR relayGet(relay_t relay);
bool ICACHE_FLASH_ATTR relayIsConfigured(relay_t relay);

#endif /* __RELAY_H__ */
