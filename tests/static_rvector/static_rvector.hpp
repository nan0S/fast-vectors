#pragma once

#include <iostream>

using cap_type = size_t;

template<typename T, cap_type C>
class static_rvector {
public:
	using value_type = T;
	using size_type = cap_type;
	using reference = T&;
	using const_reference = const T&;
	using iterator = T*;
	using const_iterator = const T*;

	static constexpr cap_type Capacity = C;

	static_rvector();

	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	iterator end() noexcept;
	const_iterator end() const noexcept;

	template<typename... Args>
	void emplace_back(Args&&... args);
	template<typename... Args>
	void fast_emplace_back(Args&&... args);

	void push_back(const_reference value) noexcept;
	void fast_push_back(const_reference value) noexcept;
	void push_back(value_type&& value);
	void fast_push_back(value_type&& value) noexcept;

private:
	T data_[C];
	size_type length_;
};

template<typename T, cap_type C>
static_rvector<T, C>::static_rvector() :
	length_(0) {
}

template<typename T, cap_type C>
typename static_rvector<T, C>::iterator
static_rvector<T, C>::begin() noexcept {
	return data_;
}

template<typename T, cap_type C>
typename static_rvector<T, C>::const_iterator
static_rvector<T, C>::begin() const noexcept {
	return data_;
}

template<typename T, cap_type C>
typename static_rvector<T, C>::iterator
static_rvector<T, C>::end() noexcept {
	return data_ + length_;
}

template<typename T, cap_type C>
typename static_rvector<T, C>::const_iterator
static_rvector<T, C>::end() const noexcept {
	return data_ + length_;
}

template<typename T, cap_type C>
template<typename... Args>
void static_rvector<T, C>::emplace_back(Args&&... args) {
	if (length_ == Capacity)
		throw std::out_of_range("msg");
	data_[length_++] = value_type(std::forward<Args>(args)...);
}

template<typename T, cap_type C>
template<typename... Args>
void static_rvector<T, C>::fast_emplace_back(Args&&... args) {
	data_[length_++] = value_type(std::forward<Args>(args)...);
}

template<typename T, cap_type C>
void static_rvector<T, C>::push_back(const_reference value) noexcept {
	if (length_ == Capacity)
		throw std::out_of_range("msg");
	data_[length_++] = value;
}

template<typename T, cap_type C>
void static_rvector<T, C>::fast_push_back(const_reference value) noexcept {
	data_[length_++] = value;
}

template<typename T, cap_type C>
void static_rvector<T, C>::push_back(value_type&& value) {
	if (length_ == Capacity)
		throw std::out_of_range("msg");
	data_[length_++] = std::forward<value_type>(value);
}

template<typename T, cap_type C>
void static_rvector<T, C>::fast_push_back(value_type&& value) noexcept {
	data_[length_++] = std::forward<value_type>(value);
	// new (&data_[length_++]) value_type(value);
}