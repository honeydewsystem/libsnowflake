/*
Copyright © 2022 Honeydew

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef INCLUDED_SNOWFLAKE_H
#define INCLUDED_SNOWFLAKE_H

#ifdef __cpluslus
extern "C" {
#endif

/**
	Generates a timestamp relative to the snowflake epoch, which is
	configured at compile time.  (By default it is the first second of 2015)
*/
uint_fast64_t snowflake_create_timestamp(void);

/**
	Constructs a snowflake from the given components.

	timestamp must fit within 42 bits (be less than 2^42). It is recommended
	that you call snowflake_create_timestamp() to get a timestamp value.

	worker_id must be less than 32, and process_id must be less than 32.

	increment_ptr must point to an already-initialized value which is
	less than or equal to 0xFFF. After the snowflake is generated, the
	value will be incremented by 1. If it exceeds 0xFFF, then it will be
	reset to 0. It may never be NULL (or nullptr).

	If any of the prior preconditions are not met, then it is not
	guaranteed that the returned snowflake is distinct from all other
	returned snowflakes, and it is not guaranteed that the extraction
	macros will return the correct result.

	For a less verbose alternative, use
	snowflake_create(worker_id, process_id, increment_ptr)
	which will automatically call snowflake_create_timestamp().

	Data may be extracted from a snowflake using:
		snowflake_extract_timestamp()
		snowflake_extract_worker_id()
		snowflake_extract_process_id()
		snowflake_extract_increment()
*/
uint_fast64_t snowflake_create_full(
		const uint_fast16_t timestamp,
		const uint_fast8_t worker_id,
		const uint_fast8_t process_id,
		uint_fast16_t* const increment_ptr);

#define snowflake_create(worker_id, process_id, increment_ptr) \
	snowflake_create_full(snowflake_create_timestamp(), worker_id, process_id, increment_ptr)

#define snowflake_extract_timestamp(s)  ((s) >> 22)
#define snowflake_extract_worker_id(s)  (((s) & 0x3E0000) >> 17)
#define snowflake_extract_process_id(s) (((s) & 0x1F000) >> 12)
#define snowflake_extract_increment(s)  ((s) & 0xFFF)

#ifdef __cplusplus
}
#endif

#endif
