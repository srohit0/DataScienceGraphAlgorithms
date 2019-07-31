///////////////////////////////////////////////////////////////////
//    Copyright (c) 2019 Rohit Sharma. All rights reserved.
//    This program is free software; you can redistribute it and/or
//    modify it under the terms as GNU General Public License.
///////////////////////////////////////////////////////////////////
//
// Generic dvector class for efficiency.

#include <assert.h>
#include <stdlib.h>     /* malloc, free, rand */

#pragma once

#define VECTOR_INIT_SIZE 12
// Ref: https://stackoverflow.com/questions/5232198/about-vectors-growth
#define VECTOR_GROWTH_FACTOR 1.5

template <class T>
class dvector {
	typedef size_t Index;
protected:
	T* _m_data;
	size_t  _sz;
	size_t  _capacity;
	static T NULL_DATA;

	void stretch_capacity()
	{
		_capacity = static_cast<size_t>(VECTOR_GROWTH_FACTOR * _capacity);
		_m_data = static_cast<T*>(realloc(_m_data, _capacity));
		assert(_m_data);
	}
	// move data values from 'index' to end by n away from start.
	void move_right(Index index, size_t n)
	{
		if (index < 0 || index >= _sz) return; // bad index
		while (_sz + n >= _capacity)
			stretch_capacity();
		for (Index i = _sz + n - 1; i > index + n - 1; i--)
			_m_data[i] = _m_data[i - 1];
		_sz = _sz + n;
	}
	// move data values from 'index' to end by n towards start.
	void move_left(Index index, size_t n)
	{
		if (index < 0 || index >= _sz) return; // bad index
		if (index + n >= _sz) {
			_sz = index;
			return;
		}
		for (Index i = index; i < _sz; i++)
			_m_data[i] = _m_data[i + n];
		_sz = _sz - n;
	}
public:
	// These selected vector methods are inspired from STL C++ vector template
	// default constructor
	dvector(size_t max_sz = VECTOR_INIT_SIZE)
	{
		_sz = 0;
		_capacity = max_sz;
		if (_capacity > 0) {
			_m_data = static_cast<T*>(malloc(_capacity * sizeof(T)));
			assert(_m_data);
		}
	}

	// copy constructor
	dvector(const dvector& other)
	{
		_sz = other._sz;
		_capacity = other._capacity;
		_m_data = static_cast<T*>(malloc(_capacity * sizeof(T)));
		assert(_m_data);
		for (Index i = 0; i < _sz; i++)
			_m_data[i] = other._m_data[i];
	}

	// assignment operator
	dvector& operator=(const dvector& other)
	{
		if (this != other) {
			if (_capacity < other._capacity) {
				_m_data = static_cast<T*>(realloc(_m_data, other._capacity));
				assert(_m_data);
				_capacity = other._capacity;
			}
			for (Index i = 0; i < other._sz; i++)
				_m_data[i] = other._m_data[i];
			_sz = other._sz;
		}
		return *this;
	}

	// destructor
	~dvector()
	{
		if (_m_data)
			free(_m_data);
	}

	short dimension()
	{
		return 1;
	}

	// unsafe
	T& operator[](const Index& index) const
	{
		return _m_data[index];
	}

	void clear()
	{
		_sz = 0;
	}

	size_t size()
	{
		return _sz;
	}

	bool empty()
	{
		return _sz == 0;
	}

	void push_back(const T& data)
	{
		if (_sz == _capacity)
			stretch_capacity();
		_m_data[_sz++] = data;
	}

	void pop_back()
	{
		_sz = (_sz > 0) ? --_sz : _sz;
	}

	T& back()
	{
		if (_sz > 0)
			return _m_data[_sz - 1];
		assert(false);
		return NULL_DATA; // return reference on stack? unsafe TODOs: use smart_ptrs
	}


	void insert(size_t index, const T& data)
	{
		move_right(index, 1);
		_m_data[index] = data;
	}

	// erase 'n' element starting from 'index'
	void erase(size_t index, size_t n)
	{
		move_left(index, n);
	}

	// linear search
	int find(const T& data)
	{
		for (Index i = 0; i < _sz; i++)
			if (_m_data[i] == data)
				return i;
		return -1;
	}

	bool exists(const T& data)
	{
		return (find(data) > 0);
	}
};
