#include "Item.h"

Item::Item(QObject *parent)
	: QObject(parent)
{	
}

Item::~Item()
{
	qDeleteAll(paths);
}

void Item::addEntry(ItemEntry *entry)
{
	paths.push_back(entry);
	emit onEntryChanged();
}

void Item::removeEntry(ItemEntry *entry)
{
	paths.removeAll(entry);
	delete entry;

	emit onEntryChanged();
}

void Item::removeEntries(QList<ItemEntry *> entries)
{	
	foreach (ItemEntry *entry, entries)
	{
		paths.removeAll(entry);
		delete entry;
	}

	emit onEntryChanged();
}

QString Item::getDescriptor() const
{
	QString ret = "";

	foreach (ItemEntry *entry, paths)
		ret += entry->path.baseName() + ", ";

	return ret;
}

quint64 Item::getTotalSize() const
{
	quint64 ret = 0;
	foreach (ItemEntry *entry, paths)
		ret += entry->size;

	return ret;
}
