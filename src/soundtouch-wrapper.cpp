/*
 * Copyright 2016 jayay
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */


#undef SOUNDTOUCH_FLOAT_SAMPLES
#define SOUNDTOUCH_INTEGER_SAMPLES 1

#include "BPMDetect.h"
using namespace soundtouch;

#include "xmalloc.h"
#include "soundtouch-wrapper.h"

BPMDetect* constructBPMDetect(
			      int numChannels,  ///< Number of channels in sample data.
			      int sampleRate    ///< Sample rate in Hz.
			      )
{
	BPMDetect* ptr = new ::soundtouch::BPMDetect(numChannels, sampleRate);
	return ptr;
}


void inputSamples(BPMDetect* pInstance,
		  const SAMPLETYPE *samples, ///< Pointer to input/working data buffer
		  int numSamples             ///< Number of samples in buffer
		  )
{
	pInstance->inputSamples(samples, numSamples);
}

float getBpm(BPMDetect* pInstance)
{
	return pInstance->getBpm();
}

void freeBPMDetect(BPMDetect* pInstance)
{
	delete pInstance;
}
