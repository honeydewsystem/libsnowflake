#include <stdint.h>
#include "snowflake.h"

#include <stdlib.h>
#include <stdio.h>

void string_smoke_test(uint_fast64_t snowflake);
int main(void);

void string_smoke_test(uint_fast64_t snowflake) {
	char buf[SNOWFLAKE_STR_LEN];
	uint_fast64_t from_string = 123;

	snowflake_to_string(snowflake, buf);

	if (snowflake_from_string(&from_string, buf)) {
		printf("from_string failed, %lu -> %s\n -> (failed)", snowflake, buf);
		exit(1);
	}

	if (snowflake != from_string) {
		printf("string conversions inconsistent, %lu -> %s -> %lu\n", snowflake, buf, from_string);
		exit(1);
	}
}

int main(void) {
	string_smoke_test(0);
	string_smoke_test(0xFFFFFFFFFFFFFFFF);
	string_smoke_test(123);
	string_smoke_test(snowflake_create(1, 2));

	return 0;
}
