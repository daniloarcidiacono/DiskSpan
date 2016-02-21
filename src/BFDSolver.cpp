#include "BFDSolver.h"
#include "SortUtils.h"

BFDSolver::BFDSolver()
{
	reset();
}

BFDSolver::~BFDSolver()
{	
}

void BFDSolver::reset()
{
	diskCapacity = 0;
	disks.clear();	
	items.clear();
	assignments.clear();
}

void BFDSolver::setDiskCapacity(const quint64 capacity)
{
	diskCapacity = capacity;
}

int BFDSolver::addItem(const quint64 size)
{	
	const int itemIndex = items.size();
	items.push_back(size);
	assignments.insert(itemIndex, -1);

	return itemIndex;
}

void BFDSolver::solve()
{
	// Order the items in decreasing capacity
	const QList<int> sortedItemsIndexes = sort_indexes(items);

	foreach (int itemIndex, sortedItemsIndexes)
	{
		const quint64 itemCapacity = items.at(itemIndex);
		int diskIdx = findFullestFittingBin(itemCapacity);
		if (diskIdx == -1)
		{
			diskIdx = disks.size();
			disks.push_back(BFDDisk(diskCapacity));
		}

		disks[diskIdx].addItem(itemIndex, itemCapacity);
		assignments[itemIndex] = diskIdx;
	}
}

int BFDSolver::findFullestFittingBin(const quint64 capacity) const
{
	int diskIdx = -1;

	// For each disk
	for (int i = 0; i < disks.size(); i++)
	{
		const BFDDisk &disk = disks.at(i);

		// If it fits
		if (disk.canFit(capacity))
		{
			if (diskIdx == -1 || disk.getRemainingCapacity() < disks.at(diskIdx).getRemainingCapacity())
				diskIdx = i;
		}
	}

	return diskIdx;
}

int BFDSolver::getDiskCount()
{
	return disks.size();
}

int BFDSolver::getItemAssignment(const int itemIndex)
{	
	return assignments.value(itemIndex);
}

