#ifndef PAYLOAD_H
#define PAYLOAD_H

#include "block.h"

class Payload{
private:
	int rock_qty = 0;
	int copper_qty = 0;
	int iron_qty = 0;
	int coal_qty = 0;
	int silver_qty = 0;
	int gold_qty = 0;
	int platinum_qty = 0;
	int sapphire_qty = 0;
	int emerald_qty = 0;
	int ruby_qty = 0;
	int diamond_qty = 0;

	const int rock_value = 0;
	const int copper_value = 10;
	const int iron_value = 20;
	const int coal_value = 30;
	const int silver_value = 60;
	const int gold_value = 100;
	const int platinum_value = 250;
	const int sapphire_value = 1000;
	const int emerald_value = 2000;
	const int ruby_value = 5000;
	const int diamand_value = 10000;

public:
	void increment(Block::Type type);
	int getTotalValue();
	void print();
	void reset();
};

#endif