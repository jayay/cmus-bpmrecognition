/*
 * Copyright 2016 jayay
 * Copyright 2008-2013 Various Authors
 * Copyright 2004 Timo Hirvonen
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

#include "config.h"
#include "xmalloc.h"
#include "bpmread.h"
#include "cache.h"
#include "utils.h"
#include "ui_curses.h"
#include "misc.h"
#include "ip.h"
#include "input.h"
#include "buffer.h"
#include "soundtouch-wrapper.h"
#include "sf.h"

#include <stdio.h>

char *program_name = "cmus_bpmread";
struct track_info *hash_table[HASH_SIZE];
int using_utf8 = 1;
char *charset = "UTF-8";
char *icecast_default_charset = "UTF-8";

int channels = 0;

struct input_plugin {
	const struct input_plugin_ops *ops;
	struct input_plugin_data data;
	unsigned int open : 1;
	unsigned int eof : 1;
	int http_code;
	char *http_reason;

	/* cached duration, -1 = unset */
	int duration;
	/* cached bitrate, -1 = unset */
	long bitrate;
	/* cached codec, NULL = unset */
	char *codec;
	/* cached codec_profile, NULL = unset */
	char *codec_profile;

	/*
	 * pcm is converted to 16-bit signed little-endian stereo
	 * NOTE: no conversion is done if channels > 2 or bits > 16
	 */
	void (*pcm_convert)(void *, const void *, int);
	void (*pcm_convert_in_place)(void *, int);
	/*
	 * 4  if 8-bit mono
	 * 2  if 8-bit stereo or 16-bit mono
	 * 1  otherwise
	 */
	int pcm_convert_scale;
};


/**
 * Stolen from Mixxx
 *
 * @param bpm The BPM value calculated by libSoundTouch
 *
 * The value of @param bpm should
 * become between @param min and @param max
 *
 * @return the new calculated bpm
 *
 */

static float correctBpm(float bpm, int min, int max)
{
	if (bpm * 2 < max) {
		bpm *= 2;
	}

	if (bpm > max) {
		bpm /= 8;
	}

	while (bpm < min) {
		bpm *= 2;
	}

	return bpm;
}


/**
 *
 */

int writeBpm(const char* filename, char force)
{
	int rc;
	int size;
	int nr_read;
	char *wpos;

	struct input_plugin *ip = ip_new(filename);
	rc = ip_open(ip);

	if (rc) {
		fprintf(stderr,"Error opening file `%s'\n", filename);
		ip_delete(ip);
		return 1;
	} else {
		ip_setup(ip);
	}

	unsigned int hash = hash_str(filename);
	struct track_info* ti = lookup_cache_entry(filename, hash);
	if (ti != NULL) {
		if ((ti->bpm > 0) && !force) {
			fprintf(stderr, "BPM: %i\n", ti->bpm);
			return STATE_HAS_BPM;
		}
	} else {
		return STATE_ERROR;
	}

	channels = sf_get_sample_size(ip->data.sf);

	int rate = sf_get_rate(ip->data.sf);
	buffer_nr_chunks = 10 * rate * sizeof(SAMPLETYPE) * channels / CHUNK_SIZE;
	buffer_init();

	BPMDetect* SBpmDetect = constructBPMDetect(channels, rate);

	while ((size = buffer_get_wpos(&wpos))) {
		nr_read = ip_read(ip, wpos, size);
		if (nr_read == 0) {
			/* EOF */
			break;
		}

		inputSamples(SBpmDetect, (SAMPLETYPE*) wpos, nr_read/channels);

		/* For debugging: it's easy to write the stream to stdout and pipe it into
		 * aplay -c 2 -r 44100  -f S16_LE
		 * fwrite(wpos, nr_read, 1, stdout);
		 */

		// mark buffer as used
		buffer_fill(nr_read);
	}

	float bpmCalculated = getBpm(SBpmDetect);
	float bpm = correctBpm(bpmCalculated, 50, 150);

	// printf("%s: found %f BPM\n", filename, bpm);
	int res = (set_bpm_for_file(filename, (int)bpm) == 0 ? STATE_SUCCESS : STATE_ERROR);

	freeBPMDetect(SBpmDetect);

	ip_delete(ip);
	return res;
}

