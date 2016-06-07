#include <inttypes.h>
#include <sys/types.h>
#include <aubio/aubio.h>

#ifndef SAMPLECONVERT_H
#define SAMPLECONVERT_H

float convert_sample(int16_t sample_int);

void convert_buffer(fvec_t* vec, size_t size, char* wpos);

#endif
