///////////////////////////////////////////////////////////////////
//    Copyright (c) 2006 Rohit Sharma. All rights reserved.
//    This program is free software; you can redistribute it and/or
//    modify it under the terms as GNU General Public License.
//
// Description: This is a simple heap container missing in STL.
//              Two simple classes have (MinHeap and MaxHeap) have
//              been provided. Interface is simple- push and pop.
//              this container can be inherited to create more
//              complex container like Treap.
// This container support all vector<T> interface except push_back
// pop_back, insert, erase and resize, which (if allowed) destroy
// the heap property.
// Example:
//       MinHeap<double> test_heap;
//       test_heap.push(2.0);
//       std::cout << test_heap.pop();
//
// NOTE:It requires assignment operator and comparision operator
// (used in functional header) for more comples classes.
///////////////////////////////////////////////////////////////////

#ifndef _HEAP_H
#define _HEAP_H

#include <vector>
#include <functional>
#include <algorithm>
#include <assert.h>

//using namespace std;

template<class T, class COMPARE>
class Heap : public std::vector<T> {
#define STD_ITER std::iterator<T, COMPARE>
	typedef typename std::vector<T>::size_type size_type;
protected:
	COMPARE        _compare;
private:
	inline void swap(size_t i, size_t j);
	void heapify_up(size_t index);
	void heapify_down(size_t index);

	// invalid vector public functions, which can destroy heap property.
	void push_back(const T&) { assert("method push_back is not supported.\n"); }
	void pop_back() { assert("method pop_back is not supported.\n"); }
	STD_ITER insert(STD_ITER, const T&) { assert("method insert is not supported.\n"); }
	template <class InputIterator> void
		insert(STD_ITER, InputIterator, InputIterator) { assert("method insert is not supported.\n"); }
	void insert(STD_ITER, size_type, const T&) { assert("method insert is not supported.\n"); }
	STD_ITER erase(STD_ITER) { assert("method erase is not supported.\n"); }
	STD_ITER erase(STD_ITER, STD_ITER) { assert("method erase is not supported.\n"); }
	void resize(size_type) { assert("method resize is not supported.\n"); }
	void resize(size_type, const T&) { assert("method resize is not supported.\n"); }
public:
	Heap() {}
	void push(const T& element);
	bool pop(T&);
	bool erase(T&);
	bool exists(T&);
	bool index(T&, size_t&);
	inline size_t parent_index(size_t i) { return (i - 1) >> 1; }
	inline size_t left_index(size_t i) { return (i << 1) + 1; }
	inline size_t right_index(size_t i) { return (i << 1) + 2; }
	void debug();
};

template<typename T, class COMPARE>
inline void
Heap<T, COMPARE>::swap(size_t i, size_t j)
{
	assert(i != j && i < this->size() && j < this->size());
	T tmp = (*this)[i]; //requires assignement operator.
	(*this)[i] = (*this)[j];
	(*this)[j] = tmp;
}

template<typename T, class COMPARE>
inline void
Heap<T, COMPARE>::heapify_up(size_t child)
{
	size_t prnt = parent_index(child);
	while (child && _compare((*this)[child], (*this)[prnt])) {
		swap(child, prnt);
		child = prnt;
		prnt = parent_index(child);
	}
}

template<typename T, class COMPARE>
inline void
Heap<T, COMPARE>::heapify_down(size_t prnt)
{
	if (!this->size())
		return;
	size_t lchild = left_index(prnt);
	size_t rchild = right_index(prnt);

	size_t new_prnt = prnt;
	if (lchild < this->size() && _compare((*this)[lchild], (*this)[prnt]))
		new_prnt = lchild;
	if (rchild < this->size() && _compare((*this)[rchild], (*this)[new_prnt]))
		new_prnt = rchild;
	if (new_prnt != prnt) {
		swap(new_prnt, prnt);
		heapify_down(new_prnt);
	}
}

template<typename T, class COMPARE>
void
Heap<T, COMPARE>::push(const T& element) {
	size_t n = this->size();
	((std::vector<T>*)this)->push_back(element);
	heapify_up(n);
}

template<typename T, class COMPARE>
bool
Heap<T, COMPARE>::pop(T& popped) {
	if (!this->size()) return false;

	popped = (*this)[0];
	(*this)[0] = (*this)[this->size() - 1];
	((std::vector<T>*)this)->pop_back();
	heapify_down(0);
	return true;
}

template<typename T, class COMPARE>
bool
Heap<T, COMPARE>::index(T& element, size_t& idx) {

	for (size_t i = 0; i < this->size(); i++)
	{
		if ((*this)[i] == element)
		{
			idx = i;
			return true;
		}
	}
	return false;

}

template<typename T, class COMPARE>
bool
Heap<T, COMPARE>::erase(T& element) {
	if (!this->size()) return false;

	size_t idx;
	if (index(element, idx))
	{
		(*this)[idx] = (*this)[this->size() - 1];
		((std::vector<T>*)this)->pop_back();
		heapify_down(idx);
		return true;
	}
	return false;
}

template<typename T, class COMPARE>
bool
Heap<T, COMPARE>::exists(T& element) {
	if (!this->size()) return false;

	return (std::find(this->begin(), this->end(), element) != this->end());
}

template <typename T>
class MinHeap : public Heap<T, std::less<T> > {};

template <typename T>
class MaxHeap : public Heap<T, std::greater<T> > {};

#endif