/*  Copyright 2016 Danilo Arcidiacono. All Rights Reserved.
 *  This file is part of DiskSpan.
 *
 *  DiskSpan is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  DiskSpan is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with DiskSpan.  If not, see <http://www.gnu.org/licenses/>.
 */
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