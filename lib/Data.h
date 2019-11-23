
#include "stdint.h"
#include "Settings.h"
#include "ReferenceTable.h"
#include "Pins.h"

#ifndef DATA_H
#define DATA_H

typedef struct{
	int32_t resistance[120];
	uint32_t voltage[14][16];
} Data;

void clearAll(Data& data);

void clearVoltage(Data& data);
void clearResistance(Data& data);

void printVoltage(const Data& data);
void printResistance(const Data& data);

void switchMuxState(const Pins& pins, const uint8_t& mux_no, const ReferenceTable& table, const uint8_t& binary_index);

void readVoltage(Data& data, const Settings& settings, const Pins& pins, const ReferenceTable& table);
void readResistance(Data& data, const Settings& settings, const Pins& pins, const ReferenceTable& table);

#endif
