#ifndef ITEMSMODEL_H
#define ITEMSMODEL_H
#include <QAbstractTableModel>

// Forward declarations
class Model;
class Item;

class ItemsModel : public QAbstractTableModel
{
	Q_OBJECT

private:
	// Reference to the model (NOT owned by this class)
	Model *model;

protected slots:
	void onItemChanged();

public:
	ItemsModel(Model *model, QObject *parent = 0);

	// Returns the item associated with the given index
	Item *getItem(const QModelIndex &index) const;

	//////////////////////////////////
	// QAbstractTableModel interface
	//////////////////////////////////
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
};

#endif