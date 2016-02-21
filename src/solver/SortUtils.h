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
#ifndef SORTUTILS_H
#define SORTUTILS_H
#include <QtAlgorithms>
#include <QList>

template<typename T> class CompareIndicesByAnotherVectorValues
{ 
    const QList<T> &values;

public: 
    CompareIndicesByAnotherVectorValues(const QList<T> &_values)
        : values(_values)
    {
    } 

    bool operator()(const int &a, const int &b) const
    { 
        return values.at(a) > values.at(b); 
    } 
}; 

template <typename T> QList<int> sort_indexes(const QList<T> &v)
{
    // initialize original index locations
    QList<int> idx;

    for (int i = 0; i < v.size(); i++)
    	idx.push_back(i);

    // sort indexes based on comparing values in v
    qSort(idx.begin(), idx.end(), CompareIndicesByAnotherVectorValues<T>(v));

    return idx;
}

#endif