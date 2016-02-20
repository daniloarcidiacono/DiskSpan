#include "Disk.h"
#include "Item.h"

Disk::Disk()
{
	capacity = 0;
}

Disk::~Disk()
{
	items.clear();
}

void Disk::addItem(Item *item)
{
	items.push_back(item);
}

void Disk::removeItem(Item *item)
{
	items.removeAll(item);
}
