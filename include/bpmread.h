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

#ifndef BPMREAD_H
#define BPMREAD_H

#define HASH_SIZE 1023

enum {
	STATE_SUCCESS,
	STATE_HAS_BPM,
	STATE_ERROR
};

extern struct track_info *hash_table[HASH_SIZE];
extern int total;

int writeBpm(const char*, char);

#endif
