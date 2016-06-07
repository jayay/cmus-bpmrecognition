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
#include "sampleconvert.h"
#include "sf.h"

#include <aubio/aubio.h>
#include <assert.h>
#include <stdio.h>

const char * const program_name = "cmus_bpmread";
struct track_info *hash_table[HASH_SIZE];
int using_utf8 = 1;
char *charset = "UTF-8";
char *icecast_default_charset = "UTF-8";
int hop_size = 1024;

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
		return -1;
	} else {
		ip_setup(ip);
	}

	unsigned int hash = hash_str(filename);
	struct track_info* ti = lookup_cache_entry(filename, hash);
	if (ti != NULL) {
		if ((ti->bpm > 0) && !force) {
			fprintf(stderr, "BPM: %i\n", ti->bpm);
			return 0;
		}
	} else {
		return -1;
	}

	channels = sf_get_sample_size(ip->data.sf);

	int samplerate = sf_get_rate(ip->data.sf);
	buffer_nr_chunks = 1024;
	buffer_init();

	fvec_t * in = new_fvec(sf_get_frame_size(ip->data.sf) * 6720);
	fvec_t * out = new_fvec (2); // output position

       	aubio_tempo_t *a_tempo = new_aubio_tempo(
						 "default",
						 buffer_nr_chunks,
						 (buffer_nr_chunks / 4),
						 samplerate);

	if (aubio_tempo_set_threshold(a_tempo, (smpl_t)20.))
		fprintf(stderr, "Couldn't set threshold\n");

	assert(a_tempo != NULL);
	int bpm = -1;
	float most_prop = 0;

	while ((size = buffer_get_wpos(&wpos))) {
		nr_read = ip_read(ip, wpos, size);
		if (nr_read <= 0) {
			/* EOF */
		       	break;
		}

		convert_buffer(in, nr_read * sf_get_sample_size(ip->data.sf), wpos);

		/* For debugging: it's easy to write the stream to stdout and pipe it into
		 * aplay -c 2 -r 44100  -f FLOAT_LE
		 */

#ifdef BINTOSTDOUT
	       	write(1, in->data, in->length);
#endif

		aubio_tempo_do(a_tempo, in, out);

		if (out->data[0] != 0) {
#ifndef BINTOSTDOUT
#ifdef DEBUG
			printf("beat at %.3fms, %.3fs, frame %d, %.2fbpm with confidence %.2f\n",
				  aubio_tempo_get_last_ms(a_tempo),
				  aubio_tempo_get_last_s(a_tempo),
				  aubio_tempo_get_last(a_tempo),
				  aubio_tempo_get_bpm(a_tempo),
				  aubio_tempo_get_confidence(a_tempo));
#endif
#endif
			float confidence = aubio_tempo_get_confidence(a_tempo);

			if (confidence > 0 && confidence >= most_prop)
				bpm = aubio_tempo_get_bpm(a_tempo);
		}

		buffer_reset();
	}

	printf("... found %d BPM\n", (int)bpm);
	int res = (set_bpm_for_file(filename, (int)bpm) == 0 ? STATE_SUCCESS : STATE_ERROR);

	del_aubio_tempo(a_tempo);
	del_fvec(in);
	del_fvec(out);
	aubio_cleanup();
	ip_delete(ip);
	return res;
}
