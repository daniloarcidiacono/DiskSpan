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