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

void Item::reset()
{
	qDeleteAll(paths);
	emit onEntryChanged();
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

void Item::addEntries(const QList<ItemEntry *> &newEntries)
{
	foreach (ItemEntry *entry, newEntries)
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

QDataStream &operator <<(QDataStream &out, const Item &foo)
{
	out << foo.paths.size();
	foreach (Item::ItemEntry *entry, foo.paths)
	{
		out << entry->path.absoluteFilePath();
		out << entry->size;
	}

	return out;
}

QDataStream &operator >>(QDataStream &in, Item &foo)
{
	foo.reset();

	// Read the entries
	int nPaths;
	in >> nPaths;
	QList<Item::ItemEntry *> entries;
	for (int i = 0; i < nPaths; i++)
	{		
		// Read the raw dat
		QString absPath;
		quint64 size;
		in >> absPath;
		in >> size;

		// Create the entry
		Item::ItemEntry *entry = new Item::ItemEntry();
		entry->path = QFileInfo(absPath);
		entry->size = size;

		// Add it
		entries.push_back(entry);
	}

	// Add them to the item
	foo.addEntries(entries);
	return in;
}