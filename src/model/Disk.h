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
#ifndef DISK_H
#define DISK_H
#include <QList>
#include <QObject>
#include <QDataStream>

// Forward declarations
class Item;

class Disk : public QObject
{
	Q_OBJECT

friend QDataStream &operator <<(QDataStream &out, const Disk &foo);
friend QDataStream &operator >>(QDataStream &in, Disk &foo);

private:
	// Items (NOT owned by this class)
	QList<Item *> items;
	quint64 capacity;

signals:
	void onItemChanged();

public:
	Disk(const quint64 capacity = 0);
	virtual ~Disk();

	// Removes all the items
	void clear();

	// Add methods
	void addItem(Item *item);
	void addItems(const QList<Item *> &newItems);
	
	// Remove methods
	void removeItem(Item *item);

	// Returns the used capacity of the disk
	quint64 getUsedCapacity() const;

	// Gets an item
	Item *getItem(const int index) const { return items.at(index); }

	// Get methods
	int getItemCount() const { return items.size(); }
	quint64 getCapacity() const { return capacity; }

	// Set methods
	void setCapacity(const quint64 value) { capacity = value; }
};

QDataStream &operator <<(QDataStream &out, const Disk &foo);
QDataStream &operator >>(QDataStream &in, Disk &foo);

#endif