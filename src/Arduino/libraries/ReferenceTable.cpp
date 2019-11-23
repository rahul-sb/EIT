
#include "ReferenceTable.h"

void set(ReferenceTable& table){
	
	for (uint8_t i = 0; i < 16; i++) {
		for (uint8_t j = 0; j < 4; j++)	{
		  table.binary_table[j][i] = bitRead(i,3-j);
		}
	}

	for (uint8_t i = 0; i < 32; i++) {
		if (i < 16)
			table.mux_vector[i] = i;
		else
			table.mux_vector[i] = i - 16;
	}
}