#ifndef PAYLOAD_H
#define PAYLOAD_H

#include "blockType.h"
#include "drawing.h"

struct Payload{
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

	void increment(BlockType type){
		switch(type){
			case ROCK:
				rock_qty++;
				break;
			case COPPER:
				copper_qty++;
				break;
			case IRON:
				iron_qty++;
				break;
			case COAL:
				coal_qty++;
				break;
			case SILVER:
				silver_qty++;
				break;
			case GOLD:
				gold_qty++;
				break;
			case PLATINUM:
				platinum_qty++;
				break;
			case SAPPHIRE:
				sapphire_qty++;
				break;
			case EMERALD:
				emerald_qty++;
				break;
			case RUBY:
				ruby_qty++;
				break;
			case DIAMOND:
				diamond_qty++;
				break;
		}
	}

	int getTotalValue(){
		int sum = 0;
		sum += (rock_qty * rock_value);
		sum += (copper_qty * copper_value);
		sum += (iron_qty * iron_value);
		sum += (coal_qty * coal_value);
		sum += (silver_qty * silver_value);
		sum += (gold_qty * gold_value);
		sum += (platinum_qty * platinum_value);
		sum += (sapphire_qty * sapphire_value);
		sum += (emerald_qty * emerald_value);
		sum += (ruby_qty * ruby_value);
		sum += (diamond_qty * diamand_value);
		return sum;
	}

	void print(){
		if(this->copper_qty){ fillText(0, "Copper: " + std::to_string(this->copper_qty), 3, 82);}
		if(this->iron_qty){ fillText(0, "Iron: " + std::to_string(this->iron_qty), 3, 94);}
		if(this->coal_qty){ fillText(0, "Coal: " + std::to_string(this->coal_qty), 3, 106);}
		if(this->silver_qty){ fillText(0, "Silver: " + std::to_string(this->silver_qty), 3, 118);}
		if(this->gold_qty){ fillText(0, "Gold: " + std::to_string(this->gold_qty), 3, 130);}
		if(this->platinum_qty){ fillText(0, "Platinum: " + std::to_string(this->platinum_qty), 3, 142);}
		if(this->sapphire_qty){ fillText(0, "Sapphire: " + std::to_string(this->sapphire_qty), 3, 154);}
		if(this->emerald_qty){ fillText(0, "Emerald: " + std::to_string(this->emerald_qty), 3, 166);}
		if(this->ruby_qty){ fillText(0, "Ruby: " + std::to_string(this->ruby_qty), 3, 178);}
		if(this->diamond_qty){ fillText(0, "Diamond: " + std::to_string(this->diamond_qty), 3, 190);}
	}

	void reset(){
		rock_qty = 0, copper_qty = 0, iron_qty = 0, coal_qty = 0, silver_qty = 0, gold_qty = 0, platinum_qty = 0;
		sapphire_qty = 0, emerald_qty = 0, ruby_qty = 0, diamond_qty = 0;
	}
};

#endif