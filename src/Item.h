#ifndef ITEM_H
#define ITEM_H
#include <QList>
#include <QFileInfo>

class Item : public QObject
{
	Q_OBJECT

public:
	class ItemEntry
	{
	public:
		// Path of the file/directory
		QFileInfo path;

		// Size in bytes
		quint64 size;

		ItemEntry()
		{
		}
	};

private:
	// Items (owned by this class)
	QList<ItemEntry *> paths;

signals:
	void onEntryChanged();

public:
	Item(QObject *parent = 0);
	virtual ~Item();

	// Returns true if the item contains an entry referring to the given absolute path.
	bool hasEntryContainingPath(const QString &absPath) const;

	// Add methods
	void addEntry(ItemEntry *entry);

	// Remove methods
	void removeEntry(ItemEntry *entry);
	void removeEntries(QList<ItemEntry *> entries);

	// Returns a string describing this entry
	QString getDescriptor() const;

	// Computes the total size in bytes of this entry
	quint64 getTotalSize() const;

	// Get methods
	ItemEntry *getEntry(const int index) const { return paths.at(index); }
	int getEntryCount() const { return paths.size(); }
};

#endif