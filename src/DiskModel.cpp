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
