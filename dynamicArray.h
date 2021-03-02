#pragma once
#include "saveNload.h"
#include "ProgramExceptions.h"
#include <iostream>

namespace elc {

	template<typename T>
	class dynamicArray {
	private:

		T* _array;
		int _length;
		int _size;

		void resize();

		class objFunc;

		template<class objFunc> static void merge_sort(T* arr, int len, const objFunc& compare)
		{
			if (len <= 1)
				return;
			else
			{
				T* sorted;
				try {
					sorted = new T[len];
				}
				catch (bad_alloc& ex)
				{
					throw allocation_failed(ex.what(), "Memory allocation for the merge sort failed.");
				}
				try {
					merge_sort(arr, len / 2, compare);
				}
				catch (allocation_failed& ex)
				{
					delete[] sorted;
					throw allocation_failed(ex.what(), ex.Message());
				}
				try
				{
					merge_sort(arr + (len / 2), len - (len / 2), compare);
				}
				catch (allocation_failed& ex)
				{
					delete[] sorted;
					throw allocation_failed(ex.what(), ex.Message());
				}
					
				merge(arr, len / 2, arr + (len / 2), len - (len / 2), sorted, compare);

				for (int i = 0; i < len; ++i)
					arr[i] = sorted[i];

				delete[] sorted;
			}
		}


		template<class objFunc> static void merge(T* arr1, int len1, T* arr2, int len2, T* res, const objFunc& comp)
		{
			int iOne = 0, iTwo = 0, iRes = 0;

			while (iOne < len1 && iTwo < len2)
			{
				if (comp(arr1[iOne], arr2[iTwo]))
					res[iRes++] = arr1[iOne++];
				else
					res[iRes++] = arr2[iTwo++];
			}

			while (iOne < len1)
				res[iRes++] = arr1[iOne++];
			while (iTwo < len2)
				res[iRes++] = arr2[iTwo++];
		}


	public:

		dynamicArray();
		explicit dynamicArray(int size);
		dynamicArray(const dynamicArray& src);
		~dynamicArray();

		dynamicArray& operator=(const dynamicArray& src);

		T& operator[](int ind) { return _array[ind]; }
		const T& operator[](int ind) const { return _array[ind]; }
		T& at(int ind);
		const T& at(int ind) const;

		void push_back(const T& val);
		T& pop_back();
		void shrink_to_fit();
		void print() const;


		template <class objFunc> void sort(const objFunc& compare)
		{
			merge_sort(_array, _length, compare);
		}


		const T& front()    const;
		int      size()     const { return _length; }
		int      capacity() const { return _size; }
		bool     empty()    const { return (_length == 0); }
		void     clear() { _length = 0; }

		void save(std::ostream& os) const
		{
			saveNload::save_basic_type<int>(os, _length);
			saveNload::save_basic_type<int>(os, _size);
		}


		void load(std::istream& is)
		{
			saveNload::load_basic_type<int>(is, _length);
			saveNload::load_basic_type<int>(is, _size);

			if (_size <= 0)
			{
				_size = 0;
				_array = nullptr;
			}
			else
			{
				try {
					_array = new T[_size];
				}
				catch (bad_alloc& ex)
				{
					throw allocation_failed(ex.what(), "Memory allocation for the dynamic array load failed.");
				}
			}
		}

		class iterator {
		private:

			dynamicArray* _pointer;
			int _index;

		public:

			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			friend class const_iterator;

			iterator(dynamicArray& arr, int index) : _pointer(&arr), _index(index) {}
			iterator(const iterator& src) : _pointer(src._pointer), _index(src._index) {}

			iterator& operator=(const iterator& src)
			{
				_pointer = src._pointer;
				_index = src._index;

				return *this;
			}


			bool operator==(const iterator& cmp) const { return (_pointer == cmp._pointer && _index == cmp._index); }
			bool operator!=(const iterator& cmp) const { return !(*this == cmp); }
			bool operator<(const iterator& cmp) const { return _index < cmp._index; }
			bool operator<=(const iterator& cmp) const { return _index < cmp._index || _index == cmp._index; }
			bool operator>(const iterator& cmp) const { return _index > cmp._index; }
			bool operator>=(const iterator& cmp) const { return _index > cmp._index || _index == cmp._index; }

			T& operator*() { return _pointer->_array[_index]; }
			T* operator->() { return &_pointer->_array[_index]; }

			iterator& operator++() { ++_index; return *this; }
			iterator operator++(int) { iterator temp(*this); ++_index; return temp; }
			iterator& operator--() { --_index; return *this; }
			iterator operator--(int) { iterator temp(*this); --_index; return temp; }
			iterator operator+(int numOfSteps) const { iterator iter = *this; iter._index += numOfSteps; return iter; }
			iterator operator-(int numOfSteps) const { iterator iter = *this; iter._index -= numOfSteps; return iter; }
			std::ptrdiff_t operator-(const iterator& other) { return _index - other._index; }
			std::ptrdiff_t operator-(const iterator& other) const { return _index - other._index; }

		};


		class const_iterator {
		private:

			const dynamicArray* _pointer;
			int _index;

		public:

			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			friend class iterator;

			const_iterator(const dynamicArray& arr, int index) : _pointer(&arr), _index(index) {}
			const_iterator(const const_iterator& src) : _pointer(src._pointer), _index(src._index) {}
			const_iterator(const iterator& src) : _pointer(src._pointer), _index(src._index) {}

			const_iterator& operator=(const const_iterator& src)
			{
				_pointer = src._pointer;
				_index = src._index;

				return *this;
			}


			bool operator==(const const_iterator& cmp) const { return (_pointer == cmp._pointer && _index == cmp._index); }
			bool operator!=(const const_iterator& cmp) const { return !(*this == cmp); }
			bool operator<(const const_iterator& cmp) const { return _index < cmp._index; }
			bool operator<=(const const_iterator& cmp) const { return _index < cmp._index || _index == cmp._index; }
			bool operator>(const const_iterator& cmp) const { return _index > cmp._index; }
			bool operator>=(const const_iterator& cmp) const { return _index > cmp._index || _index == cmp._index; }

			const T& operator*() const { return _pointer->_array[_index]; }
			const T* operator->() const { return &_pointer->_array[_index]; }

			const_iterator& operator++() { ++_index; return *this; }
			const_iterator operator++(int) { const_iterator temp(*this); ++_index; return temp; }
			const_iterator& operator--() { --_index; return *this; }
			const_iterator operator--(int) { const_iterator temp(*this); --_index; return temp; }
			const_iterator operator+(int numOfSteps) const { const_iterator iter = *this; iter._index += numOfSteps; return iter; }
			const_iterator operator-(int numOfSteps) const { const_iterator iter = *this; iter._index -= numOfSteps; return iter; }
			std::ptrdiff_t operator-(const const_iterator& other) { return _index - other._index; }
			std::ptrdiff_t operator-(const const_iterator& other) const { return _index - other._index; }

		};


		class reverse_iterator {
		private:

			dynamicArray* _pointer;
			int _index;

		public:

			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			friend class const_reverse_iterator;

			reverse_iterator(dynamicArray& arr, int index) : _pointer(&arr), _index(index) {}
			reverse_iterator(const reverse_iterator& src) : _pointer(src._pointer), _index(src._index) {}

			reverse_iterator& operator=(const reverse_iterator& src)
			{
				_pointer = src._pointer;
				_index = src._index;

				return *this;
			}


			bool operator==(const reverse_iterator& cmp) const { return (_pointer == cmp._pointer && _index == cmp._index); }
			bool operator!=(const reverse_iterator& cmp) const { return !(*this == cmp); }

			T& operator*() { return _pointer->_array[_index]; }
			T* operator->() { return &_pointer->_array[_index]; }

			reverse_iterator& operator++() { --_index; return *this; }
			reverse_iterator operator++(int) { reverse_iterator temp(*this); --_index; return temp; }
			reverse_iterator& operator--() { ++_index; return *this; }
			reverse_iterator operator--(int) { reverse_iterator temp(*this); ++_index; return temp; }

		};


		class const_reverse_iterator {
		private:

			const dynamicArray* _pointer;
			int _index;

		public:

			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			friend class reverse_iterator;

			const_reverse_iterator(const dynamicArray& arr, int index) : _pointer(&arr), _index(index) {}
			const_reverse_iterator(const const_reverse_iterator& src) : _pointer(src._pointer), _index(src._index) {}
			const_reverse_iterator(const reverse_iterator& src) : _pointer(src._pointer), _index(src._index) {}

			const_reverse_iterator& operator=(const const_reverse_iterator& src)
			{
				_pointer = src._pointer;
				_index = src._index;

				return *this;
			}


			bool operator==(const const_reverse_iterator& cmp) const { return (_pointer == cmp._pointer && _index == cmp._index); }
			bool operator!=(const const_reverse_iterator& cmp) const { return !(*this == cmp); }

			const T& operator*() const { return _pointer->_array[_index]; }
			const T* operator->() const { return &_pointer->_array[_index]; }

			const_reverse_iterator& operator++() { --_index; return *this; }
			const_reverse_iterator operator++(int) { const_reverse_iterator temp(*this); --_index; return temp; }
			const_reverse_iterator& operator--() { ++_index; return *this; }
			const_reverse_iterator operator--(int) { const_reverse_iterator temp(*this); ++_index; return temp; }

		};

		iterator begin() { return iterator(*this, 0); }
		iterator end() { return iterator(*this, _length); }
		const iterator cbegin() const { return iterator(*this, 0); }
		const iterator cend() const { return iterator(*this, _length); }
		const_iterator begin() const { return const_iterator(*this, 0); }
		const_iterator end() const { return const_iterator(*this, _length); }
		reverse_iterator rbegin() { return reverse_iterator(*this, 0); }
		reverse_iterator rend() { return reverse_iterator(*this, _length); }
		const reverse_iterator crbegin() const { return reverse_iterator(*this, 0); }
		const reverse_iterator crend() const { return reverse_iterator(*this, _length); }
		const_reverse_iterator rbegin() const { return const_reverse_iterator(*this, 0); }
		const_reverse_iterator rend() const { return const_reverse_iterator(*this, _length); }

		void insert(const iterator& itr, const T& val);

		const iterator erase(const iterator& itr)
		{
			iterator temp = itr;
			++temp;
			return erase(itr, temp);
		}


		const iterator erase(const iterator& first, const iterator& last)
		{
			iterator tempFirst = first, tempLast = last;

			int count = 0;

			while (tempFirst != tempLast)
			{
				tempFirst++;
				count++;
			}

			tempFirst = first;

			while (tempLast != end())
			{
				*tempFirst = *tempLast;
				tempFirst++;
				tempLast++;
			}

			_length -= count;

			iterator returnItr = first;
			returnItr--;

			return returnItr;

		}



	};


	template<typename T>
	void dynamicArray<T>::resize()
	{
		if (_size == 0)
		{
			try {
				_array = new T[1];
			}
			catch (bad_alloc& ex)
			{
				throw  allocation_failed(ex.what(),"Memory allocation for the dynamic array resize failed.");
			}
			_size = 1;
		}
		else
		{
			T* newArray;
			try {
				newArray = new T[_size * 2];
			}
			catch (bad_alloc& ex)
			{
				throw  allocation_failed(ex.what(), "Memory allocation for the dynamic array resize failed.");
			}
			
			for (int i = 0; i < _size; ++i)
				newArray[i] = _array[i];

			_size *= 2;

			delete[] _array;
			_array = newArray;
		}
	}

	template<typename T>
	dynamicArray<T>::dynamicArray() : dynamicArray(0)
	{
	}


	template<typename T>
	dynamicArray<T>::dynamicArray(int size) : _array(nullptr), _length(0), _size(size)
	{
		if (size <= 0)
			_size = 0;
		else
		{
			try {
				_array = new T[size];
			}
			catch (bad_alloc& ex)
			{
				throw  allocation_failed(ex.what(),"Memory allocation for the dynamic array constructor failed.");
			}
		}
	}


	template<typename T>
	dynamicArray<T>::dynamicArray(const dynamicArray& src) : dynamicArray(src._size)
	{
		for (int i = 0; i < src._length; ++i)
		{
			_array[i] = src._array[i];
			_length += 1;
		}
	}


	template<typename T>
	dynamicArray<T>::~dynamicArray()
	{
		delete[] _array;
	}


	template<typename T>
	dynamicArray<T>& dynamicArray<T>::operator=(const dynamicArray<T>& src)
	{
		if (this != *src)
		{
			_length = src._length;
			_size = src._size;

			delete[] _array;

			if (src._size == 0)
				_array = nullptr;
			else
			{
				try {
					_array = new T[src._size];
				}
				catch (bad_alloc& ex)
				{
					throw allocation_failed(ex.what(), "Memory allocation for the dynamic array = operator failed.");
				}
				
				for (int i = 0; i < src._length; ++i)
					_array[i] = src._array[i];
			}
		}
		return *this;
	}


	template<typename T>
	T& dynamicArray<T>::at(int ind)
	{
		if (ind < 0 || ind > _length - 1)
			throw "out of bounds";
		else
			return _array[ind];
	}


	template<typename T>
	const T& dynamicArray<T>::at(int ind) const
	{
		if (ind < 0 || ind > _length - 1)
			throw "out of bounds";
		else
			return _array[ind];
	}


	template<typename T>
	void dynamicArray<T>::push_back(const T& val)
	{
		if (_length >= _size)
			resize();

		_array[_length++] = val;
	}


	template<typename T>
	T& dynamicArray<T>::pop_back()
	{
		if (_length == 0)
			throw "tried refrencing non refrenceble element";
		else
		{
			_length -= 1;
			return _array[_length];
		}
	}


	template<typename T>
	void dynamicArray<T>::shrink_to_fit()
	{
		if (_length == 0)
		{
			delete[] _array;
			_size = 0;
		}
		else if (_length != _size)
		{
			T* newArray;
			try {
				newArray = new T[_length];
			}
			catch (bad_alloc& ex)
			{
				cout << "Memory allocation for the dynamic array shrink to fit failed." << endl;
				exit(EXIT_FAILURE);
			}
			
			for (int i = 0; i < _length; ++i)
				newArray[i] = _array[i];

			delete[] _array;
			_array = newArray;

			_size = _length;
		}
		else
			return;
	}


	template<typename T>
	void dynamicArray<T>::print() const
	{
		for (int i = 0; i < _length; ++i)
			std::cout << _array[i] << " ";
	}


	template<typename T>
	const T& dynamicArray<T>::front() const
	{
		if (_length == 0)
			throw "out of bounds";
		else
			return _array[0];
	}


	template<typename T>
	void dynamicArray<T>::insert(const iterator& itr, const T& val)
	{
		iterator curr = end();
		iterator prev = end();

		while (curr != itr)
		{
			prev--;
			*curr = *prev;
			curr = prev;
		}

		iterator temp = itr;
		*temp = val;

		_length += 1;
	}

}