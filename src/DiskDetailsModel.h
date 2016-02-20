#ifndef DISKDETAILSMODEL_H
#define DISKDETAILSMODEL_H
#include <QAbstractTableModel>

// Forward declarations
class Disk;
class Item;

class DiskDetailsModel : public QAbstractTableModel
{
	Q_OBJECT

private:
	// Reference to the disk (NOT owned by this class)
	Disk *disk;

protected slots:
	void onDiskChanged();

public:
	DiskDetailsModel(Disk *disk, QObject *parent = 0);

	// Returns the item associated with the given index
	Item *getItem(const QModelIndex &index) const;

	//////////////////////////////////
	// QAbstractTableModel interface
	//////////////////////////////////
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    // Get methods
	Disk *getDisk() const { return disk; }

	// Set methods
    void setDisk(Disk *disk);
};

#endif