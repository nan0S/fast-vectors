#pragma once

#include <cinttypes>
#include <iterator>

typedef unsigned int uint;

template<typename T, uint C>
class static_rvector {
public:
	using value_type = T;
	using size_type = uint_fast32_t;
	using reference = T&;
	using const_reference = const T&;
	using iterator = T*;
	using const_iterator = const T*;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	static_rvector();

	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	iterator end() noexcept;
	const_iterator end() const noexcept;

	reverse_iterator rbegin() noexcept;
	const_reverse_iterator rbegin() const noexcept;
	reverse_iterator rend() noexcept;
	const_reverse_iterator rend() const noexcept;

	const_iterator cbegin() const noexcept;
	const_iterator cend() const noexcept;
	const_reverse_iterator crbegin() const noexcept;
	const_reverse_iterator crend() const noexcept;

	template<typename... Args>
	void emplace_back(Args&&... args);
	template<typename... Args>
	void fast_emplace_back(Args&&... args) noexcept;

	void push_back(const T& value);
	void fast_push_back(const_reference value) noexcept;
	void push_back(T&& value);
	void fast_push_back(value_type&& value) noexcept;

private:
	T m_data[C];
	size_type m_length;
};

template<typename T, uint C>
static_rvector<T, C>::static_rvector() :
	m_length(0) {
}

template<typename T, uint C>
typename static_rvector<T, C>::iterator
static_rvector<T, C>::begin() noexcept {
	return m_data;
}

template<typename T, uint C>
typename static_rvector<T, C>::const_iterator
static_rvector<T, C>::begin() const noexcept {
	return m_data;
}

template<typename T, uint C>
typename static_rvector<T, C>::iterator
static_rvector<T, C>::end() noexcept {
	return m_data + m_length;
}

template<typename T, uint C>
typename static_rvector<T, C>::const_iterator
static_rvector<T, C>::end() const noexcept {
	return m_data + m_length;
}

template<typename T, uint C>
typename static_rvector<T, C>::reverse_iterator
static_rvector<T, C>::rbegin() noexcept {
	return m_data + m_length;
}

template<typename T, uint C>
typename static_rvector<T, C>::const_reverse_iterator
static_rvector<T, C>::rbegin() const noexcept {
	return m_data + m_length;
}

template<typename T, uint C>
typename static_rvector<T, C>::reverse_iterator
static_rvector<T, C>::rend() noexcept {
	return m_data;
}

template<typename T, uint C>
typename static_rvector<T, C>::const_reverse_iterator
static_rvector<T, C>::rend() const noexcept {
	return m_data;
}

template<typename T, uint C>
typename static_rvector<T, C>::const_iterator
static_rvector<T, C>::cbegin() const noexcept {
	return m_data;
}

template<typename T, uint C>
typename static_rvector<T, C>::const_iterator
static_rvector<T, C>::cend() const noexcept {
	return m_data + m_length;
}

template<typename T, uint C>
typename static_rvector<T, C>::const_reverse_iterator
static_rvector<T, C>::crbegin() const noexcept {
	return m_data + m_length;
}

template<typename T, uint C>
typename static_rvector<T, C>::const_reverse_iterator
static_rvector<T, C>::crend() const noexcept {
	return m_data;
}

template<typename T, uint C>
template<typename... Args>
void static_rvector<T, C>::emplace_back(Args&&... args) {
	// if (m_length == C)
		// throw std::out_of_range("msg");
	m_data[m_length++] = value_type(std::forward<Args>(args)...);
}

template<typename T, uint C>
template<typename... Args>
void static_rvector<T, C>::fast_emplace_back(Args&&... args) noexcept {
	m_data[m_length++] = value_type(std::forward<Args>(args)...);
}

template<typename T, uint C>
void static_rvector<T, C>::push_back(const_reference value) {
	// if (m_length == C)
		// throw std::out_of_range("msg");
	m_data[m_length++] = value;
}

template<typename T, uint C>
void static_rvector<T, C>::fast_push_back(const_reference value) noexcept {
	m_data[m_length++] = value;
}

template<typename T, uint C>
void static_rvector<T, C>::push_back(value_type&& value) {
	// if (m_length == C)
		// throw std::out_of_range("msg");
	m_data[m_length++] = std::move(value);
}

template<typename T, uint C>
void static_rvector<T, C>::fast_push_back(value_type&& value) noexcept {
	m_data[m_length++] = std::move(value);
}