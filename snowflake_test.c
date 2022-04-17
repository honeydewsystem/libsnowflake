#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "snowflake.h"

int main(void);
void test_extractions(
		uint_fast64_t timestamp,
		uint_fast8_t worker_id,
		uint_fast8_t process_id,
		uint_fast16_t* increment);

void test_extractions(
		uint_fast64_t timestamp,
		uint_fast8_t worker_id,
		uint_fast8_t process_id,
		uint_fast16_t* increment) {

	uint_fast64_t test64;
	uint_fast16_t test16;
	uint_fast8_t test8;

	uint_fast16_t increment_original = *increment;
	uint_fast64_t s = snowflake_create_full(timestamp, worker_id, process_id, increment);

	test64 = snowflake_extract_timestamp(s);
	if (test64 != timestamp) {
		printf("Timestamp not consistent, %lx -> %lx\n", timestamp, test64);
		exit(1);
	}

	test8 = snowflake_extract_worker_id(s);
	if (test8 != worker_id) {
		printf("Worker ID not consistent, %x -> %x\n", worker_id, test8);
		exit(1);
	}

	test8 = snowflake_extract_process_id(s);
	if (test8 != process_id) {
		printf("Process ID not consistent, %x -> %x\n", process_id, test8);
		exit(1);
	}

	test16 = snowflake_extract_increment(s);
	if (test16 != increment_original) {
		printf("Increment not consistent, %lx -> %lx\n", increment_original, test16);
		exit(1);
	}
}

int main(void) {
	uint_fast16_t increment = 0;
	test_extractions(0, 0, 0, &increment);
	test_extractions(1, 0, 0, &increment);
	test_extractions(0, 1, 0, &increment);
	test_extractions(0, 0, 1, &increment);
	test_extractions(1, 1, 1, &increment);
	test_extractions(1, 2, 3, &increment);
	test_extractions(2199023255551, 31, 31, &increment);
	test_extractions(snowflake_create_timestamp(), 31, 31, &increment);

	increment = 0xFFE;
	test_extractions(snowflake_create_timestamp(), 31, 31, &increment);

	if (increment != 0xFFF) {
		printf("Increment does not reach 0xFFF\n");
		exit(1);
	}

	test_extractions(snowflake_create_timestamp(), 31, 31, &increment);

	if (increment != 0) {
		printf("Increment does not return to 0x000\n");
		exit(1);
	}

	printf("Tests OK\n");
	return 0;
}
