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
#ifndef MODEL_H
#define MODEL_H
#include <QObject>
#include <QList>
#include <QDataStream>

// Forward declaration
class Item;
class Disk;

class Model : public QObject
{
	Q_OBJECT

friend QDataStream &operator <<(QDataStream &out, const Model &foo);
friend QDataStream &operator >>(QDataStream &in, Model &foo);

private:
	// Items (owned by this class)
	QList<Item *> items;

	// Disks (owned by this class)
	QList<Disk *> disks;

signals:
	void onItemChanged();
	void onDiskChanged();
	void beforeRemovingItem(const Item *it);
	void beforeRemovingDisk(const Disk *ds);
	
public:
	Model(QObject *parent = 0);
	virtual ~Model();

	// Resets the model
	void reset();

	// Returns true if the model contains an item referencing the given absolute path.
	bool hasItemContainingPath(const QString &absPath) const;
	
	// Add methods
	void addItem(Item *item);
	void addItems(const QList<Item *> &newItems);
	void addDisk(Disk *disk);
	void addDisks(const QList<Disk *> &newItems);

	// Remove methods
	void removeItem(Item *item);
	void removeItems(QList<Item *> &itemsToRemove);
	void removeDisk(Disk *disk);
	void removeDisks();

	// Gets an item
	Item *getItem(const int index) const { return items.at(index); }

	// Gets a disk
	Disk *getDisk(const int index) const { return disks.at(index); }

	// Returns the sum of the sizes of every item
	quint64 getTotalItemSize() const;
	
	// Get methods
	int getItemCount() const { return items.size(); }
	int getDiskCount() const { return disks.size(); }
};

QDataStream &operator <<(QDataStream &out, const Model &foo);
QDataStream &operator >>(QDataStream &in, Model &foo);

#endif