#ifndef DISK_H
#define DISK_H
#include <QList>
#include <QObject>

// Forward declarations
class Item;

class Disk : public QObject
{
	Q_OBJECT

private:
	// Items (NOT owned by this class)
	QList<Item *> items;
	quint64 capacity;

signals:
	void onItemChanged();

public:
	Disk(const quint64 capacity = 0);
	virtual ~Disk();

	// Add methods
	void addItem(Item *item);

	// Remove methods
	void removeItem(Item *item);

	// Removes all the items
	void clear();

	// Gets an item
	Item *getItem(const int index) const { return items.at(index); }

	// Get methods
	int getItemCount() const { return items.size(); }
	quint64 getCapacity() const { return capacity; }

	// Set methods
	void setCapacity(const quint64 value) { capacity = value; }
};

#endif