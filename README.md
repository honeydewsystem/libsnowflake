# libsnowflake

Generates 'snowflakes' (unique identifiers) based on the specification provided by Discord (available [here](https://discord.com/developers/docs/reference#snowflakes))

By default, Discord's epoch is used, though this may be changed by defining `SNOWFLAKE_EPOCH` in `snowflake.c`.


## Requirements

Requires `_POSIX_C_SOURCE >= 200112L` (for `clock_gettime` and `lround`) and you may need to add `-lm` to gcc's flags to link the math library (for `lround`).


## Testing

See `snowflake_test.c` for tests, and use `make test`.


## Installation

`make install` (root privileges may be necessary)


## Usage

For documentation, see the header file's comments. For more in-depth examples, see `snowflake_test.c`.

```c
#include <stdint.h>
#include "snowflake.h"

/* ... */

/* arbitrary values, must be less than 32 */
uint_fast8_t worker_id = 1;
uint_fast8_t process_id = 1;

/* this will be incremented for every call to snowflake_create_full */
/* (it will also be reset to 0 when it gets too large) */
uint_fast16_t increment = 0;

/* create a timestamp for this moment */
uint_fast64_t timestamp = snowflake_create_timestamp();

/* put all the pieces together */
uint_fast64_t snowflake1 = snowflake_create_full(timestamp, worker_id, process_id, &increment);

/* less verbose option - calls snowflake_create_timestamp() for you */
uint_fast64_t snowflake2 = snowflake_create(worker_id, process_id, &increment);
```


