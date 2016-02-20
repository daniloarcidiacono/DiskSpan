#ifndef DISK_H
#define DISK_H
#include <QList>

// Forward declarations
class Item;

class Disk
{
private:
	// Items (NOT owned by this class)
	QList<Item *> items;
	quint64 capacity;

public:
	Disk();
	virtual ~Disk();

	// Add methods
	void addItem(Item *item);

	// Remove methods
	void removeItem(Item *item);

	// Gets an item
	Item *getItem(const int index) const { return items.at(index); }

	// Get methods
	int getItemCount() const { return items.size(); }
	quint64 getCapacity() const { return capacity; }

	// Set methods
	void setCapacity(const quint64 value) { capacity = value; }
};

#endif