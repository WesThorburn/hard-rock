#ifndef PAYLOAD_H
#define PAYLOAD_H

#include "drawing.h"
#include "block.h"

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

	void increment(Block::Type type){
		switch(type){
			case Block::ROCK:
				rock_qty++;
				break;
			case Block::COPPER:
				copper_qty++;
				break;
			case Block::IRON:
				iron_qty++;
				break;
			case Block::COAL:
				coal_qty++;
				break;
			case Block::SILVER:
				silver_qty++;
				break;
			case Block::GOLD:
				gold_qty++;
				break;
			case Block::PLATINUM:
				platinum_qty++;
				break;
			case Block::SAPPHIRE:
				sapphire_qty++;
				break;
			case Block::EMERALD:
				emerald_qty++;
				break;
			case Block::RUBY:
				ruby_qty++;
				break;
			case Block::DIAMOND:
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
		int startingYPos = 82;
		int totalPayloadValue = getTotalValue();
		if(this->copper_qty){ fillText(0, "Copper: " + std::to_string(this->copper_qty), 3, startingYPos += 12);}
		if(this->iron_qty){ fillText(0, "Iron: " + std::to_string(this->iron_qty), 3, startingYPos += 12);}
		if(this->coal_qty){ fillText(0, "Coal: " + std::to_string(this->coal_qty), 3, startingYPos += 12);}
		if(this->silver_qty){ fillText(0, "Silver: " + std::to_string(this->silver_qty), 3, startingYPos += 12);}
		if(this->gold_qty){ fillText(0, "Gold: " + std::to_string(this->gold_qty), 3, startingYPos += 12);}
		if(this->platinum_qty){ fillText(0, "Platinum: " + std::to_string(this->platinum_qty), 3, startingYPos += 12);}
		if(this->sapphire_qty){ fillText(0, "Sapphire: " + std::to_string(this->sapphire_qty), 3, startingYPos += 12);}
		if(this->emerald_qty){ fillText(0, "Emerald: " + std::to_string(this->emerald_qty), 3, startingYPos += 12);}
		if(this->ruby_qty){ fillText(0, "Ruby: " + std::to_string(this->ruby_qty), 3, startingYPos += 12);}
		if(this->diamond_qty){ fillText(0, "Diamond: " + std::to_string(this->diamond_qty), 3, startingYPos += 12);}
		setFontSize(0, 14);
		setFillStyle(0, 0, 0, 0);
		if(totalPayloadValue){ fillText(0, "Cargo Value: $" + std::to_string(totalPayloadValue) + ".00", 3, startingYPos += 14);}
	}

	void reset(){
		rock_qty = 0, copper_qty = 0, iron_qty = 0, coal_qty = 0, silver_qty = 0, gold_qty = 0, platinum_qty = 0;
		sapphire_qty = 0, emerald_qty = 0, ruby_qty = 0, diamond_qty = 0;
	}
};

#endif