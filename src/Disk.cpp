#include "Disk.h"
#include "Item.h"

Disk::Disk(const quint64 capacity)
	: QObject()
{
	this->capacity = capacity;
}

Disk::~Disk()
{
}

void Disk::addItem(Item *item)
{
	items.push_back(item);
	emit onItemChanged();
}

void Disk::removeItem(Item *item)
{
	items.removeAll(item);
	emit onItemChanged();
}

void Disk::clear()
{
	items.clear();
	emit onItemChanged();
}