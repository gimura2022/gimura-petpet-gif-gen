#ifndef _http_h
#define _http_h

#include <stdint.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024*16

typedef void (*http_hendler_t)(char* buf, int client_fd);

void http_start_server(const char* ip, const uint16_t port, http_hendler_t hendler);
void http_send_data(const int client_fd, const char* type, uint8_t* data, size_t data_size);
void http_send_error(const int client_fd);

#endif
