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
#ifndef ITEMDETAILSMODEL_H
#define ITEMDETAILSMODEL_H
#include <QAbstractTableModel>
#include "model/Item.h"

class ItemDetailsModel : public QAbstractTableModel
{
	Q_OBJECT

private:
	// Reference to Item (NOT owned by this class)
	Item *item;

protected slots:
	void onEntryChanged();
	void onItemChanged();
	void onBeforeRemovingItem(const Item *it);

public:
	ItemDetailsModel(Item *item, QObject *parent = 0);

	// Returns the item entry associated with the given index
	Item::ItemEntry *getItemEntry(const QModelIndex &index) const;

	//////////////////////////////////
	// QAbstractTableModel interface
	//////////////////////////////////
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

	// Get methods
	Item *getItem() const { return item; }

	// Set methods
    void setItem(Item *item);
};

#endif