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
#include "FSTreeProxyFilter.h"
#include "Model.h"
#include <QFileSystemModel>
#include <QtDebug>
 
FSTreeProxyFilter::FSTreeProxyFilter(Model *model, QObject *parent)
    : QSortFilterProxyModel(parent)
{
    this->model = model;

    QObject::connect(this->model, SIGNAL(onItemChanged()), this, SLOT(onItemChanged()));
}
 
void FSTreeProxyFilter::onItemChanged()
{
    invalidateFilter();
}

bool FSTreeProxyFilter::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const QFileSystemModel *fsModel = static_cast<const QFileSystemModel *>(sourceModel());
    const QModelIndex index = fsModel->index(sourceRow, 0, sourceParent);
    const QString absPath = fsModel->fileInfo(index).absoluteFilePath();

    return !model->hasItemContainingPath(absPath);
}