#include "DiskModel.h"
#include "Disk.h"
#include "FSUtils.h"
#include "Model.h"

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
    		return QString("Disk %1 (%2)").arg(index.row() + 1).arg(FSUtils::formatSize(disk->getCapacity()));
    }

    return QVariant();	
}
