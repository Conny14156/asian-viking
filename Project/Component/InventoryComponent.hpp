#ifndef InventoryComponent_HPP
#define InventoryComponent_HPP
#include "IBaseComponent.hpp"
#include <vector>
class Item;
class Bag;
class InventoryComponent : public IBaseComponent < InventoryComponent >
{
public:
	double maxWeight;
	double currentWeight;
	int usedBags;
	int maxBags;

	int coins;

	std::vector<Bag*> bags;

	InventoryComponent();
	~InventoryComponent();
	
	bool addItem(Item& item, int numberOfItems);
	bool swapItem(Bag* first, int firstPair, Bag* second,int secondPair);
	bool equipBag(Bag* first, int firstPair);
	bool unequipBag(Bag* bag, Bag* second, int emptySlot);
	bool UpdateFromEditor();

	void attachOn(GameObject* attachTo);
	void addGold(unsigned int goldToAdd);
	void removeGold(unsigned int goldToRemove);
private:

	template<class Archive>
	friend void save(Archive&Ar, const InventoryComponent& Icp);
	template<class Archive>
	friend void load(Archive&Ar, InventoryComponent& Icp);

};


#endif