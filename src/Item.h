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
#ifndef ITEM_H
#define ITEM_H
#include <QList>
#include <QFileInfo>

class Item : public QObject
{
	Q_OBJECT

public:
	class ItemEntry
	{
	public:
		// Path of the file/directory
		QFileInfo path;

		// Size in bytes
		quint64 size;

		ItemEntry()
		{
		}
	};

private:
	// Items (owned by this class)
	QList<ItemEntry *> paths;

signals:
	void onEntryChanged();

public:
	Item(QObject *parent = 0);
	virtual ~Item();

	// Returns true if the item contains an entry referring to the given absolute path.
	bool hasEntryContainingPath(const QString &absPath) const;

	// Add methods
	void addEntry(ItemEntry *entry);

	// Remove methods
	void removeEntry(ItemEntry *entry);
	void removeEntries(QList<ItemEntry *> entries);

	// Returns a string describing this entry
	QString getDescriptor() const;

	// Computes the total size in bytes of this entry
	quint64 getTotalSize() const;

	// Get methods
	ItemEntry *getEntry(const int index) const { return paths.at(index); }
	int getEntryCount() const { return paths.size(); }
};

#endif