#include <string.h>

#include "processor.h"

processor::processor(memory_bus *pmb_in) : pmb(pmb_in)
{
	memset(registers, 0x00, sizeof registers);

	pc = 0;
}

processor::~processor()
{
}

void processor::tick()
{
	int instruction = -1;

	if (!pmb -> read_32b(pc, &instruction))
	{
		// processor exception FIXME
	}

	pc += 4;

	int opcode = (instruction >> 26) & 0x3f;

	switch(opcode)
	{
		case 0x02:		// J-type
		case 0x03:
			j_type(opcode, instruction);
			break;

		case 0x01:		// I-type
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
		case 0x08:
		case 0x09:
		case 0x09:
		case 0x0b:
		case 0x0c:
		case 0x0d:
		case 0x0e:
		case 0x0f:
		case 0x20:
		case 0x21:
		case 0x23:
		case 0x24:
		case 0x25:
		case 0x28:
		case 0x29:
		case 0x2b:
		case 0x31:
		case 0x39:
			i_type(opcode, instruction);
			break;

		case 0x10:		// co processor instructions
		case 0x11:
		case 0x12:
		case 0x13:
			ipco(opcode, instruction);
			break;

		default:
			fprintf(stderr, "unsupported opcode %02x\n", opcode);
			// throw invalid
			break;
	}
}

void processor::j_type(int opcode, int instruction)
{
	if (opcode == 3)	// JAL
		registers[31] = pc;

	pc = instruction & 0x3FFFFFF | (pc & 0x3c);
}

void processor::ipco(int opcode, int instruction)
{
	int co_processor = opcode & 0x03;
	int format = (instruction >> 21) & 0x1f;
	int function = instruction 0x3f;

	fprintf(stderr, "IPCO(%d) format %d function %d\n", co_processor, format, function);
}

void processor::r_type(int opcode, int instruction)
{
	int function = instruction & 0x3f;
	int sa = (instruction >> 6) & 0x1f;
	int rd = (instruction >> 11) & 0x1f;
	int rt = (instruction >> 16) & 0x1f;
	int rs = (instruction >> 21) & 0x1f;

	switch(function)
	{
		case 0x0d;		// BREAK for debugging
			fprintf(stderr, "BREAK\n");
			break;

		default:
			// throw invalid
			fprintf(stderr, "r-type unsupported function %02x\n", function);
			break;
	}
}

void processor::i_type(int opcode, int instruction)
{
	int immediate = instruction & 0xffff;

	switch(opcode)
	{
		case 0x04:		// BEQ
			offset = immediate << 2;
			break; // FIXME

		default:
			// throw invalid
			fprintf(stderr, "i-type unsupported function %02x\n", opcode);
			break;
	}
}