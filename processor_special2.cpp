#include "processor.h"
#include "processor_utils.h"

void processor::special2(uint32_t instruction)
{
	uint8_t clo = instruction & MASK_6B;
	uint8_t rd = (instruction >> 11) & MASK_5B;
	uint8_t rt = (instruction >> 16) & MASK_5B;
	uint8_t rs = (instruction >> 21) & MASK_5B;

	int64_t temp_64b = -1;

	switch(clo)
	{
		case 0x02:		// MUL
			temp_64b = get_register_32b_signed(rs) * get_register_32b_signed(rt);
			set_register_32b(rd, temp_64b & MASK_32B);
			// LO/HI are said to be unpredictable after this command
			break;

		case 0x1C:		// CLZ
			set_register_32b(rd, count_leading_zeros(32, registers[rs]));
			// FIXME also in rt?
			break;

		case 0x21:		// CLO
			set_register_32b(rd, count_leading_ones(32, registers[rs]));
			// FIXME also in rt?
			break;

		default:
			pdc -> log("special2 clo %02x not supported", clo);
			break;
	}
}
