#include "sampleconvert.h"


float convert_sample(int16_t sample_int)
{
	float f;
	f = ((float) sample_int) / (float) 32768;
	if( f > 1 )
		f = 1;
	if( f < -1 )
		f = -1;
	return f;
}

void convert_buffer(fvec_t * vec, size_t size, char* wpos)
{
	int i = 0, j = 0;

	while (i < size) {
		uint8_t byte1 = wpos[i];
		uint8_t byte2 = wpos[i+1];

		int16_t sample = (byte2 << 8) | (byte1);

		vec->data[j++] = (smpl_t)convert_sample(sample);
		i+=2;
	}
	vec->length = j;
}
