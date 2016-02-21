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
#ifndef FSTREEPROXYFILTER_H
#define FSTREEPROXYFILTER_H
 
#include <QSortFilterProxyModel>
 
// Forward declarations
class Model;

class FSTreeProxyFilter : public QSortFilterProxyModel
{
	Q_OBJECT

private:
	// Reference to the model (NOT owned by this class)
	Model *model;

protected slots:
	void onItemChanged();

public:
    FSTreeProxyFilter(Model *model, QObject *parent = NULL);
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
};
 
#endif