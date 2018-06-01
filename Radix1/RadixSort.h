//
// Created by tanmay on 29/5/18.
//

#ifndef UNTITLED_RADIXSORT_H
#define UNTITLED_RADIXSORT_H
//
// Created by tanmay on 29/5/18.
//

#include "Points.h"
#include <vector>
#include <iostream>
#include <omp.h>
#include <time.h>
#include <fstream>
#include <string>
#include <chrono>
using namespace std;
template <class T>
class RadixSort{
    Points<T> *arr;
    int n;
public:
    RadixSort(int n)
    {
        this->n=n;
        ifstream in;
        string line;
        string filename = "exampleOutput2.csv";
        in.open(filename);
        arr=new Points<T>[n];
        for (int i = 0; i < n; i++) {
            unsigned int x,y,z;
            getline( in, line,',');
            x=stoi(line);
            getline( in, line,',');
            y=stoi(line);
            getline( in, line,'\n');
            z=stoi(line);
            arr[i]=Points<T>(x,y,z);
        }
        in.close();
        /*cout<<"Enter Points";
        arr=new Points<T>[n];
        for (int i = 0; i < n; i++) {
            unsigned int x,y,z;
            cin>>x>>y>>z;
            arr[i]=Points<T>(x,y,z);
        }*/
    }

    void printinp()
    {
        for (int i = 0; i < this->n; i++)
        {
            cout<<"("<<arr[i].getX()<<","<<arr[i].getY()<<","<<arr[i].getZ()<<"),";
        }
    }
    vector<Points<T>> Sort(vector<Points<T>> arr1,int level)
    {
        if(arr1.size()<=1 || level<1)
        {
            return arr1;
        }
        vector<Points<T>> buckets[8];
        omp_set_num_threads(1);

        #pragma omp parallel for
        for (int i = 0; i < arr1.size(); i++)
        {
            T x=arr1[i].getX();
            T y=arr1[i].getY();
            T z=arr1[i].getZ();
            x=x>>(level-1);
            y=y>>(level-1);
            z=z>>(level-1);
            x=x&1;
            y=y&1;
            z=z&1;
            y=y<<1;
            x=x<<2;
            #pragma omp critical
            {
                buckets[x+y+z].push_back(arr1[i]);
            }
        }
        vector<Points<T>> sortedarr;
        omp_set_num_threads(1);

        #pragma omp parallel for
        for (int i = 0; i < 8; i++)  {
            for (Points<T> point : Sort(buckets[i], level - 1)) {
                #pragma omp critical
                {
                    sortedarr.push_back(point);
                }
            }
        }
        return sortedarr;
    }
    void sorting()
    {
        vector<Points<T>> sortedarr;
        vector<Points<T>> unsortedarr;
        int lvls= sizeof(arr->getX())*8;
        omp_set_num_threads(1);

#pragma omp parallelfor
        for (int i = 0; i < this->n; i++)
        {
            #pragma omp critical
            {
            unsortedarr.push_back(arr[i]);
            }
        }
        sortedarr=Sort(unsortedarr, lvls);
        for (Points<T> point : sortedarr)
        {
            cout<<"("<<point.getX()<<","<<point.getY()<<","<<point.getZ()<<"),";
        }
    }
};

#endif //UNTITLED_RADIXSORT_H

/*
 * Algorithm Sort(pointslist,level)
 * base case: If number of elements in pointslist is less than or equal to 1,
 *            If  level<=-1
 *                  return .
 * Make 8 buckets
 *
 * for each element in pointslist
 *        Right shift x,y,z by level-1
 *        x,y,z&1
 *        y<<1
 *        z<<2
 *        bucketnum=x+y+z
 *
 *        Place point in bucket.
 * for each bucket
 *        Sort(pointslist, level-1)
 *
 */