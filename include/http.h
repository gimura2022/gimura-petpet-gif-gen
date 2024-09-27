#ifndef _http_h
#define _http_h

#include <stdint.h>

typedef void (*http_hendler_t)(char* buf);

void http_start_server(const char* ip, const uint16_t port, http_hendler_t hendler);

#endif
