// c++20 required
#pragma once
#include <stdexcept>

template<class _Type>
class ListItem {
	template<class _Type>
	friend class List;
private:
	_Type value_{ _Type };
	ListItem<_Type>* previous_{ nullptr };
	ListItem<_Type>* next_{ nullptr };

	_Type operator*() { return value_; }

	ListItem* operator+(int distance) {
		if (distance == 0) return this;
		if (distance < 0) return this->operator-(distance * -1);
		ListItem* ptr = this;
		for (int i = 1; i <= distance; i++)
			ptr = ptr->next_;
		return ptr;
	}

	ListItem* operator-(int distance) {
		if (distance == 0) return this;
		if (distance < 0) return this->operator+(distance * -1);
		ListItem* ptr = this;
		for (int i = 1; i < distance; i++)
			ptr = ptr->previous_;
		return ptr;
	}
public:
	ListItem() {
		value_ = _Type();
		next_ = nullptr;
		previous_ = nullptr;
	}
	ListItem(_Type value, ListItem<_Type>* previous, ListItem<_Type>* next) :
		value_(value), previous_(previous), next_(next) {};

	_Type getValue() {
		return value_;
	}

};

template<class _Type>
class List {
private:
	ListItem<_Type>* first_;
	int length_;

	int index_to_real_index(int index) {
		int len = getLength();
		while (index < 0) index += len;
		while (index > len) index -= len;
		return index;
	}
public:
	List() {
		first_ = nullptr;
	}

	_Type* operator[](int index) {
		index = index_to_real_index(index);
		return &((*first_ + index)->value_);
	}

				// returns true only if all items are the same
	bool operator==(List<_Type>& r_value) 
		requires requires(_Type t) { t == t; t != t; } {
		if (length_ != r_value.length_)
			return false;
		ListItem<_Type>* ptr = first_;
		ListItem<_Type>* rptr = r_value.first_;
		while (ptr != nullptr && rptr != nullptr) {
			if (ptr->value_ != rptr->value_)
				return false;
			ptr = ptr + 1;
			rptr = rptr + 1;
		}
		return true;
	}

	ListItem<_Type>* insert(_Type value, int position = -1) {
		if (getLength() == 0) {
			first_ = new ListItem<_Type>(value, nullptr, nullptr);
			length_++;
			return first_;
		}
		if (position < 0)
			position = index_to_real_index(position) + 1;
		if (position > length_)
			position = index_to_real_index(position) - 1;

		ListItem<_Type>* ptr;

		if (position == length_) {
			ptr = *first_ + (position - 1);
			ptr->next_ = new ListItem<_Type>(value, ptr, nullptr);
		}
		else if (position == 0) {
			ptr = first_;
			ptr->previous_ = new ListItem<_Type>(value, nullptr, ptr);
		}
		else {
			ptr = *first_ + position;
			ListItem<_Type>* prev = ptr->previous_;
			prev->next_ = new ListItem<_Type>(value, prev, ptr);
			ptr->previous_ = prev->next_;
		}

		length_++;
		return ptr->previous_;
	}

	_Type remove(int index) {
		ListItem<_Type>* ptr = *first_ + index;
		_Type value = ptr->value_;;
		if (ptr->previous_ == nullptr) {
			ptr->next_->previous_ = nullptr;
			delete ptr;
		}
		else if (ptr->next_ == nullptr) {
			ptr->previous_->next_ == nullptr;
			delete ptr;
		}
		else {
			ptr->previous_->next_ = ptr->next_;
			ptr->next_->previous_ = ptr->previous_;
			delete ptr;
		}
		length_--;
		return value;
	}

	int find(_Type value) requires requires(_Type t) { t == t; } {
		int res = 0;
		ListItem<_Type>* ptr = first_;
		while (true) {
			if (ptr == nullptr) break;
			if (ptr->value_ == value) return res;
			ptr = *ptr + 1;
			res++;
		}
		return -1;
	}

	int getLength() { return length_; }

};
