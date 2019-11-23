
#include "stdint.h"
#include "Arduino.h"

#ifndef REFERENCETABLE_H
#define REFERENCETABLE_H

typedef struct{
	uint8_t binary_table[4][16];
	uint8_t mux_vector[32];
} ReferenceTable;

void set(ReferenceTable& table);

#endif

/*
	mux_vector: [0 ,1 ,... , 15, 0, 1, ..., 15]'

	binary_table:
				0,0,0,0     1
				0,0,0,0     1
				0,0,1,1     1
				0,1,0,1 ... 1
 */ 
