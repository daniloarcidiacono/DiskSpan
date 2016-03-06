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
#include "Model.h"
#include "Item.h"
#include "Disk.h"
#include <QDebug>

Model::Model(QObject *parent)
	: QObject(parent)
{
	reset();
}

Model::~Model()
{
	qDeleteAll(items);
	qDeleteAll(disks);

	//reset();
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

void Model::addItems(const QList<Item *> &newItems)
{
	foreach (Item *item, newItems)
	{
		QObject::connect(item, SIGNAL(onEntryChanged()), this, SIGNAL(onItemChanged()));
		items.push_back(item);
	}

	emit onItemChanged();	
}

void Model::addDisk(Disk *disk)
{
	QObject::connect(disk, SIGNAL(onItemChanged()), this, SIGNAL(onDiskChanged()));
	disks.push_back(disk);
	emit onDiskChanged();
}

void Model::addDisks(const QList<Disk *> &newItems)
{	
	foreach (Disk *disk, newItems)
	{
		QObject::connect(disk, SIGNAL(onItemChanged()), this, SIGNAL(onDiskChanged()));
		disks.push_back(disk);
	}
	
	emit onDiskChanged();
}

void Model::removeItem(Item *item)
{
	QObject::disconnect(item, SIGNAL(onEntryChanged()), this, SIGNAL(onItemChanged()));
	emit beforeRemovingItem(item);
	items.removeAll(item);
	delete item;
	emit onItemChanged();
}

void Model::removeItems(QList<Item *> &itemsToRemove)
{	
	foreach (Item *item, itemsToRemove)
	{
		QObject::disconnect(item, SIGNAL(onEntryChanged()), this, SIGNAL(onItemChanged()));
		emit beforeRemovingItem(item);
		items.removeAll(item);
	}	

	qDeleteAll(itemsToRemove);
	itemsToRemove.clear();
	emit onItemChanged();
}

void Model::removeDisk(Disk *disk)
{
	QObject::disconnect(disk, SIGNAL(onItemChanged()), this, SIGNAL(onDiskChanged()));
	emit beforeRemovingDisk(disk);
	disks.removeAll(disk);
	delete disk;
	emit onDiskChanged();
}

void Model::removeDisks()
{	
	foreach (Disk *disk, disks)
	{
		QObject::disconnect(disk, SIGNAL(onItemChanged()), this, SIGNAL(onDiskChanged()));
		emit beforeRemovingDisk(disk);
	}

	qDeleteAll(disks);
	disks.clear();
	emit onDiskChanged();
}

quint64 Model::getTotalItemSize() const
{
	quint64 totalSize = 0;
	foreach (Item *item, items)
        totalSize += item->getTotalSize();

    return totalSize;
}

QDataStream &operator <<(QDataStream &out, const Model &foo)
{
	// Write a header with a "magic number" and a version
	out << (quint32)0xA0B0C0D0;
	out << (qint32)020;
	out.setVersion(QDataStream::Qt_5_5);

	// Write the data
	out << foo.items.size();
	foreach (Item *item, foo.items)
		out << *item;

	out << foo.disks.size();
	foreach (Disk *disk, foo.disks)
		out << *disk;

	return out;
}

QDataStream &operator >>(QDataStream &in, Model &foo)
{
	// Read and check the header
	quint32 magic;
	in >> magic;
	//if (magic != 0xA0B0C0D0)
	//    return XXX_BAD_FILE_FORMAT;

	// Read the version
	qint32 version;
	in >> version;
	/*if (version < 100)
	    return XXX_BAD_FILE_TOO_OLD;
	if (version > 123)
	    return XXX_BAD_FILE_TOO_NEW;*/

	/*if (version <= 110)
	    in.setVersion(QDataStream::Qt_3_2);
	else
	    in.setVersion(QDataStream::Qt_4_0);*/

	in.setVersion(QDataStream::Qt_5_5);
	
	foo.reset();
	
	// Read the items
	int nItems;
	in >> nItems;
	qDebug() << "modelread: nItems = " << nItems;
	QList<Item *> items;
	for (int i = 0; i < nItems; i++)
	{
		Item *item = new Item();
		in >> *item;
		items.push_back(item);
	}

	// Read the disks
	int nDisks;
	in >> nDisks;
	qDebug() << "modelread: nDisks = " << nDisks;
	QList<Disk *> disks;
	for (int i = 0; i < nDisks; i++)
	{
		Disk *disk = new Disk();
		in >> *disk;
		disks.push_back(disk);
	}

	// Setup the document
	foo.addItems(items);
	foo.addDisks(disks);

	return in;
}