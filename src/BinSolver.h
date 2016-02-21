#ifndef BINSOLVER_H
#define BINSOLVER_H
#include <QtGlobal>

class BinSolver
{
public:
	virtual ~BinSolver() { };

	// Resets the parameters
	virtual void reset() = 0;

	// Sets the capacity of the bins
	virtual void setDiskCapacity(const quint64 capacity) = 0;

	// Adds a new item with given size
	// Returns the zero-based index assigned to the item
	virtual int addItem(const quint64 size) = 0;

	// Solves the bin-packing instance
	virtual void solve() = 0;

	// Returns the computed number of bins
	virtual int getDiskCount() = 0;

	// Returns the zero-based index of the disk assigned to item having the given index 
	virtual int getItemAssignment(const int itemIndex) = 0;
};

#endif