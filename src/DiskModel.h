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