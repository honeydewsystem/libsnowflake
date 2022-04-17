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

#define SNOWFLAKE_STR_LEN 40

uint_fast64_t snowflake_create_timestamp(void);

uint_fast64_t snowflake_create(const uint_fast8_t worker_id, const uint_fast8_t process_id);

uint_fast64_t snowflake_extract_timestamp(const uint_fast64_t s);
uint_fast8_t snowflake_extract_worker_id(const uint_fast64_t s);
uint_fast8_t snowflake_extract_process_id(const uint_fast64_t s);
uint_fast16_t snowflake_extract_increment(const uint_fast64_t s);

void snowflake_to_string(uint_fast64_t s, char* buf);

int snowflake_from_string(uint_fast64_t *s, const char* buf);

#ifdef __cplusplus
}
#endif

#endif
