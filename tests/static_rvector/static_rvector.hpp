#pragma once

#include <iostream>

template<typename T, uint C>
class static_rvector {
public:
	using value_type = T;
	using size_type = uint_fast32_t;
	using reference = T&;
	using const_reference = const T&;
	using iterator = T*;
	using const_iterator = const T*;

	static_rvector();

	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	iterator end() noexcept;
	const_iterator end() const noexcept;

	template<typename... Args>
	void emplace_back(Args&&... args);
	template<typename... Args>
	void fast_emplace_back(Args&&... args) noexcept;

	void push_back(const T& value);
	void fast_push_back(const_reference value) noexcept;
	void push_back(T&& value);
	void fast_push_back(value_type&& value) noexcept;

private:
	T data_[C];
	size_type length_;
};

template<typename T, uint C>
static_rvector<T, C>::static_rvector() :
	length_(0) {
}

template<typename T, uint C>
typename static_rvector<T, C>::iterator
static_rvector<T, C>::begin() noexcept {
	return data_;
}

template<typename T, uint C>
typename static_rvector<T, C>::const_iterator
static_rvector<T, C>::begin() const noexcept {
	return data_;
}

template<typename T, uint C>
typename static_rvector<T, C>::iterator
static_rvector<T, C>::end() noexcept {
	return data_ + length_;
}

template<typename T, uint C>
typename static_rvector<T, C>::const_iterator
static_rvector<T, C>::end() const noexcept {
	return data_ + length_;
}

template<typename T, uint C>
template<typename... Args>
void static_rvector<T, C>::emplace_back(Args&&... args) {
	// if (length_ == C)
		// throw std::out_of_range("msg");
	data_[length_++] = value_type(std::forward<Args>(args)...);
}

template<typename T, uint C>
template<typename... Args>
void static_rvector<T, C>::fast_emplace_back(Args&&... args) noexcept {
	data_[length_++] = value_type(std::forward<Args>(args)...);
}

template<typename T, uint C>
void static_rvector<T, C>::push_back(const_reference value) {
	// if (length_ == C)
		// throw std::out_of_range("msg");
	data_[length_++] = value;
}

template<typename T, uint C>
void static_rvector<T, C>::fast_push_back(const_reference value) noexcept {
	data_[length_++] = value;
}

template<typename T, uint C>
void static_rvector<T, C>::push_back(value_type&& value) {
	// if (length_ == C)
		// throw std::out_of_range("msg");
	data_[length_++] = std::move(value);
}

template<typename T, uint C>
void static_rvector<T, C>::fast_push_back(value_type&& value) noexcept {
	data_[length_++] = std::move(value);
}