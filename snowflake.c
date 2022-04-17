/*
Copyright © 2022 Honeydew

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
	_POSIX_C_SOURCE
   clock_gettime   199309L
   lround          200112L
*/
#if _POSIX_C_SOURCE < 200112L
#error "snowflake.c requires _POSIX_C_SOURCE >= 200112L"
#endif

#ifndef SNOWFLAKE_EPOCH
/* first second of 2015 */
#define SNOWFLAKE_EPOCH 1420070400000
#endif

#include <stdint.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include "snowflake.h"

uint_fast16_t snowflake_increment = 0;

uint_fast64_t snowflake_create_timestamp(void) {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return (ts.tv_sec * 1000) + lround(ts.tv_nsec / 1000000.0) - SNOWFLAKE_EPOCH;
}

uint_fast64_t snowflake_create_full(
		const uint_fast16_t timestamp,
		const uint_fast8_t worker_id,
		const uint_fast8_t process_id,
		uint_fast16_t* const increment_ptr) {

	uint64_t ret = 0;

	ret = ((timestamp & 0x1FFFFFFFFFF) << 22)
		| ((worker_id & 0x1F) << 17)
		| ((process_id & 0x1F) << 12)
		| ((*increment_ptr) & 0xFFF);

	if (*increment_ptr >= 0xFFF) {
		*increment_ptr = 0;
	} else {
		*increment_ptr += 1;
	}

	return ret;
}

