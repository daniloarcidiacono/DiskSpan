#include "ItemsModel.h"
#include "Model.h"
#include "Item.h"
#include "FSUtils.h"
#include <QDebug>

ItemsModel::ItemsModel(Model *model, QObject *parent)
	: QAbstractTableModel(parent)
{
	this->model = model;

	// Signals
	QObject::connect(model, SIGNAL(onItemChanged()), this, SLOT(onItemChanged()));
}

Item *ItemsModel::getItem(const QModelIndex &index) const
{
	if (index.isValid())
		return model->getItem(index.row());

	return 0;
}

void ItemsModel::onItemChanged()
{
	// Update the whole view
	beginResetModel();
	endResetModel();

	// Update the whole view
	//emit dataChanged(QModelIndex(), QModelIndex());
}

int ItemsModel::rowCount(const QModelIndex &parent) const
{	
	return model->getItemCount();
}

int ItemsModel::columnCount(const QModelIndex &parent) const
{
	return 2;
}

QVariant ItemsModel::data(const QModelIndex &index, int role) const
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

QVariant ItemsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) 
        {
            switch (section)
            {
            	case 0:
                	return tr("Entries");

            	case 1:
                	return tr("Size");
            }
        }
    }
    return QVariant();
}