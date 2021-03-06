/*  Copyright 2016 Danilo Arcidiacono. All Rights Reserved.
 *  This file is part of DiskSpan.
 *
 *  DiskSpan is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  DiskSpan is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with DiskSpan.  If not, see <http://www.gnu.org/licenses/>.
 */
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

void Disk::addItems(const QList<Item *> &newItems)
{
	foreach (Item *item, newItems)
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

quint64 Disk::getUsedCapacity() const
{
	quint64 ret = 0;
	foreach (Item *item, items)
		ret += item->getTotalSize();

	return ret;
}

QDataStream &operator <<(QDataStream &out, const Disk &foo)
{
	out << foo.capacity;
	out << foo.items.size();
	foreach (Item *item, foo.items)
		out << *item;

	return out;
}

QDataStream &operator >>(QDataStream &in, Disk &foo)
{
	// Reset
	foo.clear();
	
	// Read the capacity
	quint64 capacity;
	in >> capacity;
	foo.setCapacity(capacity);

	// Read the items
	int nItems;
	in >> nItems;
	QList<Item *> items;
	for (int i = 0; i < nItems; i++)
	{
		Item *item = new Item();
		in >> *item;
		items.push_back(item);
	}

	// Add them to the disk
	foo.addItems(items);

	return in;
}