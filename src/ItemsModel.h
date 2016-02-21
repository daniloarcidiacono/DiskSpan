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
#ifndef ITEMSMODEL_H
#define ITEMSMODEL_H
#include <QAbstractTableModel>

// Forward declarations
class Model;
class Item;

class ItemsModel : public QAbstractTableModel
{
	Q_OBJECT

private:
	// Reference to the model (NOT owned by this class)
	Model *model;

protected slots:
	void onItemChanged();

public:
	ItemsModel(Model *model, QObject *parent = 0);

	// Returns the item associated with the given index
	Item *getItem(const QModelIndex &index) const;

	//////////////////////////////////
	// QAbstractTableModel interface
	//////////////////////////////////
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
};

#endif