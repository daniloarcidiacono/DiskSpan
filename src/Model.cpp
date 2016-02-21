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
	QObject::connect(disk, SIGNAL(onItemChanged()), this, SIGNAL(onDiskChanged()));
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
	QObject::disconnect(disk, SIGNAL(onItemChanged()), this, SIGNAL(onDiskChanged()));
	disks.removeAll(disk);
	delete disk;
	emit onDiskChanged();
}

void Model::removeDisks()
{	
	foreach (Disk *disk, disks)
		QObject::disconnect(disk, SIGNAL(onItemChanged()), this, SIGNAL(onDiskChanged()));

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
