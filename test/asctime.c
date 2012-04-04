#include <assert.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
	char		*buf;
	struct tm const	tm = {
		.tm_sec = 20,
		.tm_min = 21,
		.tm_hour = 22,
		.tm_mday = 10,
		.tm_mon = 11,
		.tm_year = 112,
	};
	char		*res;

	buf = malloc(4096);
	assert(buf != NULL);
	memset(buf, 23, 4096);

	res = asctime_r(&tm, buf);
	assert(strcmp(res, asctime(&tm)) == 0);
	assert(strcmp(res, "Sun Dec 10 22:21:20 2012\n") == 0);

	return EXIT_SUCCESS;
}
