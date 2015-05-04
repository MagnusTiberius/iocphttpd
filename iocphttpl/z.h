#pragma once
#ifndef _Z_H
#define _Z_H

#include "stdafx.h"

// Z template class
template <class T>class Z
{
private:

	T* d;
	unsigned int ss;

public:

	Z(int s = 0)
	{
		if (!s)
			s = 1;
		d = new T[s];
		memset(d, 0, s*sizeof(T));
		ss = s;
	}
	~Z()
	{
		delete[] d;
	}

	operator T*()
	{
		return d;
	}

	void _clear()
	{
		ZeroMemory(d, ss*sizeof(T));
	}
	void clear()
	{
		ZeroMemory(d, ss*sizeof(T));
	}

	int bs()
	{
		return ss*sizeof(T);
	}

	int is()
	{
		return ss;
	}

	void Resize(unsigned int news)
	{
		if (news == ss)
			return; // same size

		// Create buffer to store existing data
		T* newd = new T[news];
		int newbs = news*sizeof(T);
		ZeroMemory((void*)newd, newbs);

		if (ss < news)
			// we created a larger data structure
			memcpy((void*)newd, d, ss*sizeof(T));
		else
			// we created a smaller data structure
			memcpy((void*)newd, d, news*sizeof(T));
		delete[] d;
		d = newd;
		ss = news;
	}

	void AddResize(int More)
	{
		Resize(ss + More);
	}

};

#endif

