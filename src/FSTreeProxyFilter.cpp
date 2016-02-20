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