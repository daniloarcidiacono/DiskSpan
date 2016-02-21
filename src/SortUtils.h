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