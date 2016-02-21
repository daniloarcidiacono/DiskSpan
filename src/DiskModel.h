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
#ifndef DISKMODEL_H
#define DISKMODEL_H
#include <QAbstractListModel>

// Forward declarations
class Disk;
class Model;

class DiskModel : public QAbstractListModel
{
	Q_OBJECT

private:
	// Reference to the model (NOT owned by this class)
	Model *model;

protected slots:
	void onDiskChanged();

public:
	DiskModel(Model *model, QObject *parent = 0);

	// Returns the disk associated with the given index
	Disk *getDisk(const QModelIndex &index) const;

	//////////////////////////////////
	// QAbstractListModel interface
	//////////////////////////////////
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
};

#endif