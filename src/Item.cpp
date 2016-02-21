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
#include "Item.h"

Item::Item(QObject *parent)
	: QObject(parent)
{	
}

Item::~Item()
{
	qDeleteAll(paths);
}

bool Item::hasEntryContainingPath(const QString &absPath) const
{
	foreach (Item::ItemEntry *entry, paths)
		if (entry->path.absoluteFilePath() == absPath)
			return true;

	return false;
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
