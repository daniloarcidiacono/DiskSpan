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
#include "DiskModel.h"
#include "model/Disk.h"
#include "model/Model.h"
#include "FSUtils.h"

DiskModel::DiskModel(Model *model, QObject *parent)
	: QAbstractListModel(parent)
{
	this->model = model;

	QObject::connect(model, SIGNAL(onDiskChanged()), this, SLOT(onDiskChanged()));
}

void DiskModel::onDiskChanged()
{
	// Update the whole view
	beginResetModel();
	endResetModel();
}

Disk *DiskModel::getDisk(const QModelIndex &index) const
{
	if (index.isValid())
		return model->getDisk(index.row());

	return NULL;
}

int DiskModel::rowCount(const QModelIndex &parent) const
{
	return model->getDiskCount();	
}

QVariant DiskModel::data(const QModelIndex &index, int role) const
{
	const Disk *disk = getDisk(index);
	
 	if (role == Qt::DisplayRole)
    {    	
    	if (index.column() == 0)
    	{
    		const quint64 capacity = disk->getCapacity();
    		const quint64 usedCapacity = disk->getUsedCapacity();
    		const double usedPercentage = (double)usedCapacity / capacity * 100;
    		return QString("Disk %1 (%2, %3% used) ").arg(index.row() + 1)
    												.arg(FSUtils::formatSize(capacity))
    												.arg(QString::number(usedPercentage, 'f', 2));
    	}
    }

    return QVariant();	
}
