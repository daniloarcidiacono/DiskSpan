#ifndef ITEMDETAILSMODEL_H
#define ITEMDETAILSMODEL_H
#include <QAbstractTableModel>
#include "Item.h"

class ItemDetailsModel : public QAbstractTableModel
{
	Q_OBJECT

private:
	// Reference to Item (NOT owned by this class)
	Item *item;

protected slots:
	void onEntryChanged();
	void onItemChanged();
	
public:
	ItemDetailsModel(Item *item, QObject *parent = 0);

	// Returns the item entry associated with the given index
	Item::ItemEntry *getItemEntry(const QModelIndex &index) const;

	//////////////////////////////////
	// QAbstractTableModel interface
	//////////////////////////////////
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

	// Get methods
	Item *getItem() const { return item; }

	// Set methods
    void setItem(Item *item);
};

#endif