#ifndef _gif_h
#define _gif_h

#include <stdint.h>
#include <stdio.h>

void gif_init(void);
void gif_gen(uint8_t* image, uint8_t** out, size_t* size);

#endif
