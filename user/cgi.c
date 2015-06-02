/*
Some random cgi routines. Used in the LED example and the page that returns the entire
flash as a binary. Also handles the hit counter on the main page.
*/

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */


#include <string.h>
#include <osapi.h>
#include "user_interface.h"
#include "mem.h"
#include "httpd.h"
#include "cgi.h"
#include "relay.h"
#include <ip_addr.h>
#include "espmissingincludes.h"
#include "../include/httpdconfig.h"

// cgi code for the relay
int ICACHE_FLASH_ATTR cgiRelay(HttpdConnData *connData) {
    int len;
    char buff[1024];
    
    if (connData->conn == NULL) {
        //Connection aborted. Clean up.
        return HTTPD_CGI_DONE;
    }

    len = httpdFindArg(connData->post->buff, "relay0", buff, sizeof(buff));
    if (len != 0) {
        if (relayIsConfigured(RELAY0))
            relaySet(RELAY0, atoi(buff));
    }
    len = httpdFindArg(connData->post->buff, "relay1", buff, sizeof(buff));
    if (len != 0) {
        if (relayIsConfigured(RELAY1))
            relaySet(RELAY1, atoi(buff));
    }

    httpdRedirect(connData, "relay.tpl");
    return HTTPD_CGI_DONE;
}

// template code for the relay page
int ICACHE_FLASH_ATTR tplRelay(HttpdConnData *connData, char *token, void **arg) {
    char buff[32];
    bool doSub = false;
    relay_t relay = RELAY0;

    if (token==NULL) return HTTPD_CGI_DONE;

    os_strcpy(buff, "internal error");
    if (os_strcmp(token, "relay0") == 0) {
        doSub = true;
        relay = RELAY0;
    } else if (os_strcmp(token, "relay1") == 0) {
        doSub = true;
        relay = RELAY1;
    }
    if (doSub && relayIsConfigured(relay)) {
        if (relayGet(relay)) {
            os_strcpy(buff, "on");
        } else {
            os_strcpy(buff, "off");
        }
    }

    httpdSend(connData, buff, -1);

    return HTTPD_CGI_DONE;
}

static long hitCounter=0;

//Template code for the counter on the index page.
int ICACHE_FLASH_ATTR tplMain(HttpdConnData *connData, char *token, void **arg) {
    char buff[128];
    if (token==NULL) return HTTPD_CGI_DONE;
    
    // Hit Counter
    if (os_strcmp(token, "counter")==0) {
        hitCounter++;
        os_sprintf(buff, "%ld", hitCounter);
    // SDK Version
    }else if (os_strcmp(token, "sdk")==0) {
        os_sprintf(buff, "%s", SDK_VERSION);
    // Boot Partition
    }else if (os_strcmp(token, "boot")==0) {
        os_sprintf(buff, "%ld", system_upgrade_userbin_check());
    // App Version
    }else if (os_strcmp(token, "version")==0) {
        os_sprintf(buff, "%s", APP_VERSION);
    // App Name
    }else if (os_strcmp(token, "app")==0) {
        os_sprintf(buff, "%s", APP_NAME);
    }
    httpdSend(connData, buff, -1);
    return HTTPD_CGI_DONE;
}
