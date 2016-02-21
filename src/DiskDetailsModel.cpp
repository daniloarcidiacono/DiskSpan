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
#include "DiskDetailsModel.h"
#include "Model.h"
#include "Item.h"
#include "FSUtils.h"
#include "Disk.h"
#include <QDebug>

DiskDetailsModel::DiskDetailsModel(Disk *disk, QObject *parent)
	: QAbstractTableModel(parent)
{
	this->disk = disk;

	// Signals
    if (this->disk != NULL)
	   QObject::connect(this->disk, SIGNAL(onItemChanged()), this, SLOT(onDiskChanged()));
}

Item *DiskDetailsModel::getItem(const QModelIndex &index) const
{
	if (disk != NULL && index.isValid())
		return disk->getItem(index.row());

	return 0;
}

void DiskDetailsModel::onDiskChanged()
{
	// Update the whole view
	beginResetModel();
	endResetModel();
}

int DiskDetailsModel::rowCount(const QModelIndex &parent) const
{	
	return disk != NULL ? disk->getItemCount() : 0;
}

int DiskDetailsModel::columnCount(const QModelIndex &parent) const
{
	return 2;
}

QVariant DiskDetailsModel::data(const QModelIndex &index, int role) const
{
	const Item *item = getItem(index);
	
 	if (role == Qt::DisplayRole)
    {    	
    	if (index.column() == 0)
    		return item->getDescriptor();

    	if (index.column() == 1)
    		return FSUtils::formatSize(item->getTotalSize());
    }

    return QVariant();	
}

QVariant DiskDetailsModel::headerData(int section, Qt::Orientation orientation, int role) const
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

void DiskDetailsModel::setDisk(Disk *disk)
{
    if (this->disk != NULL)
        QObject::disconnect(this->disk, SIGNAL(onItemChanged()), this, SLOT(onDiskChanged()));

    this->disk = disk;

    if (this->disk != NULL)
        QObject::connect(this->disk, SIGNAL(onItemChanged()), this, SLOT(onDiskChanged()));
    
    onDiskChanged();
}