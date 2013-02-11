#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "memory_bus.h"
#include "processor_utils.h"

memory_bus::memory_bus() : list(NULL), n_elements(0)
{
}

memory_bus::~memory_bus()
{
	free(list);
}

void memory_bus::register_memory(uint64_t offset, uint64_t mask, memory *target)
{
	n_elements++;

	list = (memory_segment_t *)realloc(list, n_elements * sizeof(memory_segment_t)); 
	if (!list)
		error_exit("Memory allocation error (register_memory)");

	list[n_elements - 1].offset = offset;
	list[n_elements - 1].mask   = mask;
	list[n_elements - 1].target = target;
}

// r/w might overlap segments? FIXME
const memory_segment_t * memory_bus::find_segment(uint64_t offset) const
{
	for(int segment = 0; segment < n_elements; segment++)
	{
		const memory_segment_t *psegment = &list[segment];

		uint64_t seg_offset = psegment -> offset;
		uint64_t seg_mask   = ~psegment -> mask;

		if ((offset & seg_mask) == seg_offset)
			return psegment;
	}

	return NULL;
}

bool memory_bus::read_64b(uint64_t offset, uint64_t *data) const
{
	const memory_segment_t * segment = find_segment(offset);
	if (!segment)
		return false;	// throw and exception instead? try catch in tick() which converts the exception to an e.g. address exception(?)

	segment -> target -> read_64b(offset - segment -> offset, data);

	return true;
}

bool memory_bus::write_64b(uint64_t offset, uint64_t data)
{
	const memory_segment_t * segment = find_segment(offset);
	if (!segment)
		return false;

	segment -> target -> write_64b(offset - segment -> offset, data);

	return true;
}

bool memory_bus::read_32b(uint64_t offset, uint32_t *data) const
{
	const memory_segment_t * segment = find_segment(offset);
	if (!segment)
		return false;

	segment -> target -> read_32b(offset - segment -> offset, data);

	return true;
}

bool memory_bus::write_32b(uint64_t offset, uint32_t data)
{
	const memory_segment_t * segment = find_segment(offset);
	if (!segment)
		return false;

	segment -> target -> write_32b(offset - segment -> offset, data);

	return true;
}

bool memory_bus::read_16b(uint64_t offset, uint16_t *data) const
{
	const memory_segment_t * segment = find_segment(offset);
	if (!segment)
		return false;

	segment -> target -> read_16b(offset - segment -> offset, data);

	return true;
}

bool memory_bus::write_16b(uint64_t offset, uint16_t data)
{
	const memory_segment_t * segment = find_segment(offset);
	if (!segment)
		return false;

	segment -> target -> write_16b(offset - segment -> offset, data);

	return true;
}

bool memory_bus::read_8b(uint64_t offset, uint8_t *data) const
{
	const memory_segment_t * segment = find_segment(offset);
	if (!segment)
		return false;

	segment -> target -> read_8b(offset - segment -> offset, data);

	return true;
}

bool memory_bus::write_8b(uint64_t offset, uint8_t data)
{
	const memory_segment_t * segment = find_segment(offset);
	if (!segment)
		return false;

	segment -> target -> write_8b(offset - segment -> offset, data);

	return true;
}
