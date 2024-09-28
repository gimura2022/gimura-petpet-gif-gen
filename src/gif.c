#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

static uint8_t* gif_layout;
static size_t gif_size;

void gif_init(void) {
	FILE* file = fopen("gifs/std.gif", "rb");

	fseek(file, 0, SEEK_END);
	gif_size = ftell(file);
	rewind(file);

	gif_layout = (uint8_t*) malloc(gif_size);
	fread(gif_layout, gif_size, 1, file);

	fclose(file);
}

void gif_gen(uint8_t* image, uint8_t** out, size_t* size) {
	*out = (uint8_t*) malloc(gif_size);
	*size = gif_size;
	memcpy(*out, gif_layout, gif_size);

	// gif generation in hree
}
