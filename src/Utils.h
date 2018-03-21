/*
 * Utils.h
 *
 *  Created on: Jan 20, 2018
 *      Author: henry
 */

#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

static const bool CMD_DEBUG=false;

template <class K>
void shuffle(vector<K> &vec)
{
	for(int i = (int)vec.size() - 2; i>0; i--)
	{
		int target=rand()%((int)i);
		K temp=vec[(int)i+1];
		vec[(int)i+1]=vec[target];
		vec[target]=temp;
	}
}

template <class K>
void shuffle(vector<K> &vec, int start, int end)
{
	for(int i = (int)end - 2-start; i>0; i--)
	{
		int target=rand()%((int)i);
		K temp=vec[(int)i+1+start];
		vec[(int)i+1+start]=vec[target+start];
		vec[target+start]=temp;
	}
}

template <class K>
void shuffle(K arr[], int start, int end)
{
	for(int i = (int)end - 2-start; i>0; i--)
	{
		int target=rand()%((int)i);
		K temp=arr[(int)i+1+start];
		arr[(int)i+1+start]=arr[target+start];
		arr[target+start]=temp;
	}
}
template <class K>
void shuffle(K arr[], int end)
{
	for(int i = (int)end - 2; i>0; i--)
	{
		int target=rand()%((int)i);
		K temp=arr[(int)i+1];
		arr[(int)i+1]=arr[target];
		arr[target]=temp;
	}
}

inline vector<string> split(const string &tosplit, char reg)
{
	stringstream test(tosplit);
	string segment;
	vector<string> seglist;

	while(std::getline(test, segment, reg))
	{
	   seglist.push_back(segment);
	}

	return seglist;
}

inline int toInt(string intAsString)
{
	int asInt = 0;
	stringstream ss;
	ss<<intAsString;
	ss>>asInt;
	return asInt;
}
template <class K> string toString(K obj)
{
	string re;
	stringstream ss;
	ss<<obj;
	ss>>re;
	return re;
}
inline void process_mem_usage(double& vm_usage, double& resident_set)
{
   using std::ios_base;
   using std::ifstream;
   using std::string;

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}
inline void print_mem_usage()
{
	double vm, rss;
	process_mem_usage(vm, rss);
	cout << "VM: " << vm << "; RSS: " << rss << endl;
}

#endif /* SRC_UTILS_H_ */
