#include "Model.h"
#include "Item.h"
#include "Disk.h"

Model::Model()
	: QObject()
{
	reset();
}

Model::~Model()
{
	reset();
}

void Model::reset()
{
	qDeleteAll(items);
	qDeleteAll(disks);

	emit onItemChanged();
	emit onDiskChanged();
}

bool Model::hasItemContainingPath(const QString &absPath) const
{
	foreach (Item *item, items)
	{
		if (item->hasEntryContainingPath(absPath))
			return true;
	}

	return false;
}

void Model::addItem(Item *item)
{
	QObject::connect(item, SIGNAL(onEntryChanged()), this, SIGNAL(onItemChanged()));
	items.push_back(item);
	emit onItemChanged();
}

void Model::addDisk(Disk *disk)
{
	disks.push_back(disk);
	emit onDiskChanged();
}

void Model::removeItem(Item *item)
{
	QObject::disconnect(item, SIGNAL(onEntryChanged()), this, SIGNAL(onItemChanged()));
	items.removeAll(item);
	delete item;
	emit onItemChanged();
}

void Model::removeItems(const QList<Item *> itemsToRemove)
{
	foreach (Item *item, itemsToRemove)
	{
		QObject::disconnect(item, SIGNAL(onEntryChanged()), this, SIGNAL(onItemChanged()));
		items.removeAll(item);
		delete item;
	}

	emit onItemChanged();
}

void Model::removeDisk(Disk *disk)
{
	disks.removeAll(disk);
	delete disk;
	emit onDiskChanged();
}