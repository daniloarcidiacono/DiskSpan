#ifndef BFDSOLVER_H
#define BFDSOLVER_H
#include "BinSolver.h"
#include <QList>
#include <QMap>

class BFDSolver : public BinSolver
{
private:
	class BFDDisk
	{
	private:
		quint64 remainingCapacity;
		QList<int> itemIndexes;

	public:
		BFDDisk(const quint64 _remainingCapacity)
		{
			remainingCapacity = _remainingCapacity;
		}

		void addItem(const int itemIndex, const quint64 capacity)
		{
			itemIndexes.push_back(itemIndex);
			remainingCapacity -= capacity;
		}

		bool canFit(const quint64 capacity) const
		{
			return remainingCapacity >= capacity;
		}

		// Get methods
		quint64 getRemainingCapacity() const { return remainingCapacity; }
		int getItemIndexesCount() const { return itemIndexes.size(); }
		int getItemIndex(const int index) const { return itemIndexes.at(index); }
	};

	// Capacity of the disks
	quint64 diskCapacity;

	// Disks
	QList<BFDDisk> disks;

	// Items capacities
	QList<quint64> items;

	// itemIndex -> diskIndex
	QMap<int, int> assignments;

protected:
	// Given a item capacity, returns the index of the (existing) fullest bin that can fit the item
	// , or -1 if it doesn't exist
	int findFullestFittingBin(const quint64 capacity) const;

public:
	BFDSolver();
	virtual ~BFDSolver();

	/////////////////////////
	// BinSolver interface
	/////////////////////////
	virtual void reset();
	virtual void setDiskCapacity(const quint64 capacity);
	virtual int addItem(const quint64 size);
	virtual void solve();
	virtual int getDiskCount();
	virtual int getItemAssignment(const int itemIndex);
};

#endif