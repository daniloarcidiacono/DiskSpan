#ifndef MODEL_H
#define MODEL_H
#include <QObject>
#include <QList>

// Forward declaration
class Item;
class Disk;

class Model : public QObject
{
	Q_OBJECT

private:
	// Items (owned by this class)
	QList<Item *> items;

	// Disks (owned by this class)
	QList<Disk *> disks;

signals:
	void onItemChanged();
	void onDiskChanged();

public:
	Model();
	virtual ~Model();

	// Resets the model
	void reset();

	// Returns true if the model contains an item referencing the given absolute path.
	bool hasItemContainingPath(const QString &absPath) const;
	
	// Add methods
	void addItem(Item *item);
	void addDisk(Disk *disk);

	// Remove methods
	void removeItem(Item *item);
	void removeItems(const QList<Item *> itemsToRemove);
	void removeDisk(Disk *disk);
	void removeDisks();
	
	// Gets an item
	Item *getItem(const int index) const { return items.at(index); }

	// Gets a disk
	Disk *getDisk(const int index) const { return disks.at(index); }

	// Get methods
	int getItemCount() const { return items.size(); }
	int getDiskCount() const { return disks.size(); }
};

#endif