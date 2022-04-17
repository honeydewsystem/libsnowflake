/*
Copyright © 2022 Honeydew

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
   clock_gettime - 199309L
   lround - 200112L
*/
#if _POSIX_C_SOURCE < 200112L
#error "snowflake.c requires _POSIX_C_SOURCE >= 200112L"
#endif

#include <stdint.h>
#include "snowflake.h"

#include <time.h>
#include <math.h>
#include <assert.h>

#ifndef SNOWFLAKE_EPOCH
#define SNOWFLAKE_EPOCH 1420070400000
#endif

uint_fast16_t snowflake_increment = 0;

uint_fast64_t snowflake_create_timestamp(void) {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return (ts.tv_sec * 1000) + lround(ts.tv_nsec / 1000000.0) - SNOWFLAKE_EPOCH;
}

uint_fast64_t snowflake_create(const uint_fast8_t worker_id, const uint_fast8_t process_id) {
	uint64_t ret = 0;

	ret = ((snowflake_create_timestamp() & 0x1FFFFFFFFFF) << 22)
		| ((worker_id & 0x1FF) << 17)
		| ((process_id & 0x1FF) << 12)
		| (snowflake_increment & 0xFFF);

	snowflake_increment += 1;

	if (snowflake_increment > 0xFFF) {
		snowflake_increment = 0;
	}

	return ret;
}

uint_fast64_t snowflake_extract_timestamp(const uint_fast64_t s) {
	return s >> 22;
}

uint_fast8_t snowflake_extract_worker_id(const uint_fast64_t s) {
	return (s & 0x3E0000) >> 17;
}

uint_fast8_t snowflake_extract_process_id(const uint_fast64_t s) {
	return (s & 0x1F000) >> 12;
}

uint_fast16_t snowflake_extract_increment(const uint_fast64_t s) {
	return s & 0xFFF;
}

void snowflake_to_string(uint_fast64_t s, char* buf) {
	size_t i;
	size_t len;
	char temp_buf[SNOWFLAKE_STR_LEN];

	assert(buf != NULL);

	if (s == 0) {
		buf[0] = '0';
		buf[1] = 0;
		return;
	}

	len = 0;
	while (s > 0) {
		temp_buf[len] = '0' + (s % 10);
		s /= 10;
		len += 1;
	}

	i = 0;
	for (; i < len; i++) {
		buf[i] = temp_buf[len - i - 1];
	}
	buf[i] = 0;
}

int snowflake_from_string(uint_fast64_t* result_ptr, const char* str) {
	size_t i;
	uint_fast64_t result = 0;
	uint_fast64_t overflow_checker;

	assert(str != NULL);
	assert(result_ptr != NULL);

	if (str[0] == 0) {
		/* empty string fails instantly */
		return -1;
	}

	i = 0;
	for (; str[i] != 0; i++) {
		if (str[i] < '0' || str[i] > '9') {
			return -1;
		}
		overflow_checker = (result * 10) + (str[i] - '0');
		if (overflow_checker < result) {
			return -1;
		}
		result = overflow_checker;
	}

	*result_ptr = result;
	return 0;
}
