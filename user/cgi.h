#ifndef CGI_H
#define CGI_H

#include "httpd.h"

int cgiRelay(HttpdConnData *connData);
int tplRelay(HttpdConnData *connData, char *token, void **arg);
int tplMain(HttpdConnData *connData, char *token, void **arg);

#endif
