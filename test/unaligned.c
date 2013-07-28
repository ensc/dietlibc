#include "../dietunaligned.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

struct unaligned0 {
	uint8_t		u8;
	uint16_t	u16;
	uint32_t	u32;
	uint64_t	u64;
};

int main()
{
	struct unaligned0	data0 = {
		.u8 = 8,
		.u16 = 16,
		.u32 = 32,
		.u64 = 64
	};
	struct unaligned0	data1;

	unsigned char		*buf0 = malloc(sizeof data0 + 1);
	unsigned char		*buf1 = malloc(sizeof data0 + 1);

	assert(buf0 != NULL);
	assert(buf1 != NULL);

	memcpy(buf0+1, &data0, sizeof data0);

	assert(read_unaligned_s(buf0+1, struct unaligned0, u8)  == data0.u8);
	assert(read_unaligned_s(buf0+1, struct unaligned0, u16) == data0.u16);
	assert(read_unaligned_s(buf0+1, struct unaligned0, u32) == data0.u32);
	assert(read_unaligned_s(buf0+1, struct unaligned0, u64) == data0.u64);


	memset(buf1, 0, sizeof data0 + 1);
	write_unaligned_s(data0.u8,  buf1+1, struct unaligned0, u8);
	write_unaligned_s(data0.u16, buf1+1, struct unaligned0, u16);
	write_unaligned_s(data0.u32, buf1+1, struct unaligned0, u32);
	write_unaligned_s(data0.u64, buf1+1, struct unaligned0, u64);

	memcpy(&data1, buf1+1, sizeof data1);
	assert(data0.u8  == data1.u8);
	assert(data0.u16 == data1.u16);
	assert(data0.u32 == data1.u32);
	assert(data0.u64 == data1.u64);

	free(buf1);
	free(buf0);

	return 0;
}
