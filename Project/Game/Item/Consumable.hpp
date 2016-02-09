#ifndef CONSUMABLE_HPP
#define	CONSUMABLE_HPP
#include "item.hpp"

class Consumable : public Item
{

public:

	Consumable();
	Consumable(unsigned int ID, std::string name, std::string iconName, double weight, int price, std::string description);
	Consumable(const Consumable& copy);
	Consumable& operator=(const Consumable& copy);

	std::string toToolTipString();

	//Effect effectOnUse?

private:
	template<class Archive>
	friend void save(Archive&ar, const Consumable& consumable);
	template<class Archive>
	friend void load(Archive&ar, Consumable& consumable);
};


#endif