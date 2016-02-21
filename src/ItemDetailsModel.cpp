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
#include "ItemDetailsModel.h"
#include "FSUtils.h"
#include <QDebug>

ItemDetailsModel::ItemDetailsModel(Item *item, QObject *parent)
	: QAbstractTableModel(parent)
{
	this->item = item;

	// Signals
    if (this->item != NULL)
	   QObject::connect(this->item, SIGNAL(onEntryChanged()), this, SLOT(onEntryChanged()));
}

Item::ItemEntry *ItemDetailsModel::getItemEntry(const QModelIndex &index) const
{
    if (item != NULL && index.isValid())
        return item->getEntry(index.row());

    return NULL;
}

void ItemDetailsModel::onItemChanged()
{
    // Update the whole view
    beginResetModel();
    endResetModel();
}

void ItemDetailsModel::onEntryChanged()
{
	// Update the whole view
	beginResetModel();
	endResetModel();
}

int ItemDetailsModel::rowCount(const QModelIndex &parent) const
{	
	return item != NULL ? item->getEntryCount() : 0;
}

int ItemDetailsModel::columnCount(const QModelIndex &parent) const
{
	return 2;
}

QVariant ItemDetailsModel::data(const QModelIndex &index, int role) const
{
	const Item::ItemEntry *entry = getItemEntry(index);
	
 	if (role == Qt::DisplayRole)
    {    	
    	if (index.column() == 0)
    		return entry->path.absoluteFilePath();

    	if (index.column() == 1)
    		return FSUtils::formatSize(entry->size);
    }

    return QVariant();	
}

QVariant ItemDetailsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) 
        {
            switch (section)
            {
            	case 0:
                	return tr("Path");

            	case 1:
                	return tr("Size");
            }
        }
    }

    return QVariant();
}

void ItemDetailsModel::setItem(Item *item)
{
    if (this->item != NULL)
        QObject::disconnect(this->item, SIGNAL(onEntryChanged()), this, SLOT(onEntryChanged()));

    this->item = item;

    if (this->item != NULL)
        QObject::connect(this->item, SIGNAL(onEntryChanged()), this, SLOT(onEntryChanged()));
    
    onItemChanged();
}