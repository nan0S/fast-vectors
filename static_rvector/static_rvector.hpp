#pragma once

#include <cinttypes>
#include <iterator>
#include <algorithm>

typedef unsigned int uint;

template<typename T, uint C>
class static_rvector;

template<typename T, uint C>
std::ostream& operator<<(std::ostream& out, const static_rvector<T, C>& v);

template<typename T, uint C>
void swap(static_rvector<T, C>& x, static_rvector<T, C>& y);

template<typename T, uint C>
class static_rvector {
public:
	using value_type = T;
	using size_type = uint_fast32_t;
	using difference_type = ptrdiff_t;
	using reference = T&;
	using const_reference = const T&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using iterator = pointer;
	using const_iterator = const_pointer;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	constexpr static_rvector() noexcept;
	constexpr explicit static_rvector(size_type n);
	constexpr static_rvector(size_type n, const value_type& val);
	template<typename InputIterator,
			 typename = typename std::iterator_traits<InputIterator>::value_type>
	constexpr static_rvector(InputIterator first, InputIterator last);
	constexpr static_rvector(const static_rvector& x);
	constexpr static_rvector(static_rvector&& x) noexcept;
	constexpr static_rvector(std::initializer_list<T> il);
	~static_rvector();

	constexpr static_rvector& operator=(const static_rvector& other) noexcept;
	constexpr static_rvector& operator=(static_rvector&& other) noexcept;
	constexpr static_rvector& operator=(std::initializer_list<value_type> il) noexcept;

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

	size_type size() const noexcept;
	constexpr size_type max_size() const noexcept;
	constexpr void resize(size_type n);
	constexpr void resize(size_type n, const value_type& val);
	constexpr size_type capacity() const noexcept;
	bool empty() const noexcept;
	void reserve(size_type n) noexcept;
	void shrink_to_fit() noexcept;

	reference operator[](size_type n);
	const_reference operator[](size_type n) const;
	reference at(size_type n);
	const_reference at(size_type n) const;
	reference front();
	const_reference front() const;
	reference back();
	const_reference back() const;
	value_type* data() noexcept;
	const value_type* data() const noexcept;

	template<typename InputIterator,
		typename = typename std::iterator_traits<InputIterator>::value_type>
	void assign(InputIterator first, InputIterator last);
	void assign(size_type n, const value_type& val);
	void assign(std::initializer_list<value_type> il);

	void push_back(const_reference value);
	void fast_push_back(const_reference value) noexcept;
	void push_back(value_type&& value);
	void fast_push_back(value_type&& value) noexcept;

	void pop_back();
	void safe_pop_back() noexcept;
	// TODO: insert
	// TODO: erase
	void swap(static_rvector& other);
	void clear() noexcept;
	// TODO: emplace

	template<typename... Args>
	void emplace_back(Args&&... args);
	template<typename... Args>
	void fast_emplace_back(Args&&... args) noexcept;

	friend std::ostream& operator<<<T, C>(std::ostream& out, const static_rvector& v);

private:
	value_type* data_at(size_type n) noexcept;
	const value_type* data_at(size_type n) const noexcept;

private:
	typename std::aligned_storage<sizeof(T), alignof(T)>::type m_data[C];
	size_type m_length;
};

template<typename T, uint C>
constexpr static_rvector<T, C>::static_rvector() noexcept :
	m_length(0)
{
}

template<typename T, uint C>
constexpr static_rvector<T, C>::static_rvector(size_type n) :
	m_length(n) {
	std::uninitialized_default_construct_n(data(), n);
}

template<typename T, uint C>
constexpr static_rvector<T, C>::static_rvector(size_type n, const value_type& val) :
	m_length(n) {
	std::uninitialized_fill_n(data(), n, val);
}

template<typename T, uint C>
template<typename InputIterator, typename>
constexpr static_rvector<T, C>::static_rvector(InputIterator first, InputIterator last) :
	m_length(std::distance(first, last)) {
	std::uninitialized_copy(first, last, data());
}

template<typename T, uint C>
constexpr static_rvector<T, C>::static_rvector(const static_rvector& x) :
	m_length(x.m_length) {
	std::uninitialized_copy(x.begin(), x.end(), data());
}

template<typename T, uint C>
constexpr static_rvector<T, C>::static_rvector(static_rvector&& x) noexcept :
	m_length(x.m_length) {
	std::uninitialized_move(x.begin(), x.end(), data());
}
	
template<typename T, uint C>
constexpr static_rvector<T, C>::static_rvector(std::initializer_list<T> il) :
	m_length(il.size()) {
	std::uninitialized_copy(il.begin(), il.end(), begin());
}

template<typename T, uint C>
static_rvector<T, C>::~static_rvector() {
	std::destroy_n(data(), m_length);
}

template<typename T, uint C>
constexpr static_rvector<T, C>& static_rvector<T, C>::operator=(const static_rvector<T, C>& other) noexcept {
	auto ptr = data();
	auto optr = other.data();

	if (other.m_length < m_length) {
		std::destroy(ptr + other.m_length, ptr + m_length);
		std::copy_n(optr, other.m_length, ptr);
	}
	else {
		std::copy_n(optr, m_length, ptr);
		std::uninitialized_copy(optr + m_length, optr + other.m_length, ptr + m_length);
	}

	m_length = other.m_length;

	return *this;
}

template<typename T, uint C>
constexpr static_rvector<T, C>& static_rvector<T, C>::operator=(static_rvector<T, C>&& other) noexcept {
	auto ptr = data();
	auto optr = other.data();

	if (other.m_length < m_length) {
		std::destroy(ptr + other.m_length, ptr + m_length);
		for (size_type i = 0; i < other.m_length; ++i)
			ptr[i] = std::move(optr[i]);
	}
	else {
		for (size_type i = 0; i < m_length; ++i)
			ptr[i] = std::move(optr[i]);
		std::uninitialized_move(optr + m_length, optr + other.m_length, ptr + m_length);
	}

	m_length = other.m_length;

	return *this;
}

template<typename T, uint C>
constexpr static_rvector<T, C>& static_rvector<T, C>::operator=(std::initializer_list<value_type> il) noexcept {
	auto ptr = data();
	auto optr = il.begin();
	auto il_len = il.size();

	if (il_len< m_length) {
		std::destroy(ptr + il_len, ptr + m_length);
		std::copy_n(optr, il_len, ptr);
	}
	else {
		std::copy_n(optr, m_length, ptr);
		std::uninitialized_copy(optr + m_length, optr + il_len, ptr + m_length);
	}

	m_length = il_len;

	return *this;
}

template<typename T, uint C>
typename static_rvector<T, C>::iterator
static_rvector<T, C>::begin() noexcept {
	return data();
}

template<typename T, uint C>
typename static_rvector<T, C>::const_iterator
static_rvector<T, C>::begin() const noexcept {
	return data();
}

template<typename T, uint C>
typename static_rvector<T, C>::iterator
static_rvector<T, C>::end() noexcept {
	return data() + m_length;
}

template<typename T, uint C>
typename static_rvector<T, C>::const_iterator
static_rvector<T, C>::end() const noexcept {
	return data() + m_length;
}

template<typename T, uint C>
typename static_rvector<T, C>::reverse_iterator
static_rvector<T, C>::rbegin() noexcept {
	return data() + m_length;
}

template<typename T, uint C>
typename static_rvector<T, C>::const_reverse_iterator
static_rvector<T, C>::rbegin() const noexcept {
	return data() + m_length;
}

template<typename T, uint C>
typename static_rvector<T, C>::reverse_iterator
static_rvector<T, C>::rend() noexcept {
	return data();
}

template<typename T, uint C>
typename static_rvector<T, C>::const_reverse_iterator
static_rvector<T, C>::rend() const noexcept {
	return data();
}

template<typename T, uint C>
typename static_rvector<T, C>::const_iterator
static_rvector<T, C>::cbegin() const noexcept {
	return data();
}

template<typename T, uint C>
typename static_rvector<T, C>::const_iterator
static_rvector<T, C>::cend() const noexcept {
	return data() + m_length;
}

template<typename T, uint C>
typename static_rvector<T, C>::const_reverse_iterator
static_rvector<T, C>::crbegin() const noexcept {
	return data() + m_length;
}

template<typename T, uint C>
typename static_rvector<T, C>::const_reverse_iterator
static_rvector<T, C>::crend() const noexcept {
	return data();
}

template<typename T, uint C>
typename static_rvector<T, C>::size_type
static_rvector<T, C>::size() const noexcept {
	return m_length;
}

template<typename T, uint C>
constexpr typename static_rvector<T, C>::size_type
static_rvector<T, C>::max_size() const noexcept {
	return C;
}

template<typename T, uint C>
constexpr void static_rvector<T, C>::resize(size_type n) {
	if (n > m_length) {
		auto ptr = data();
		std::uninitialized_default_construct(ptr + m_length, ptr + n);
		m_length = n;
	}
	else if (n < m_length) {
		auto ptr = data();
		std::destroy(ptr + n, ptr + m_length);
		m_length = n;
	}
}

template<typename T, uint C>
constexpr void static_rvector<T, C>::resize(size_type n, const value_type& val) {
	if (n > m_length) {
		auto ptr = data();
		std::uninitialized_fill(ptr + m_length, ptr + n, val);
		m_length = n;
	}
	else if (n < m_length) {
		auto ptr = data();
		std::destroy(ptr + n, ptr + m_length);
		m_length = n;
	}
}

template<typename T, uint C>
constexpr typename static_rvector<T, C>::size_type
static_rvector<T, C>::capacity() const noexcept {
	return C;
}

template<typename T, uint C>
bool static_rvector<T, C>::empty() const noexcept {
	return m_length == 0;
}

template<typename T, uint C>
void static_rvector<T, C>::reserve(size_type n) noexcept {
}

template<typename T, uint C>
void static_rvector<T, C>::shrink_to_fit() noexcept {
}

template<typename T, uint C>
typename static_rvector<T, C>::reference
static_rvector<T, C>::operator[](size_type n) {
	return *data_at(n);
}

template<typename T, uint C>
typename static_rvector<T, C>::const_reference
static_rvector<T, C>::operator[](size_type n) const {
	return *data_at(n);
}

template<typename T, uint C>
typename static_rvector<T, C>::reference
static_rvector<T, C>::at(size_type n) {
	if (n >= m_length)
		throw std::out_of_range("Index out of range: " + std::to_string(n));
	return *data_at(n);
}

template<typename T, uint C>
typename static_rvector<T, C>::reference
static_rvector<T, C>::front() {
	return *data_at(0);
}

template<typename T, uint C>
typename static_rvector<T, C>::const_reference
static_rvector<T, C>::front() const {
	return *data_at(0);
}

template<typename T, uint C>
typename static_rvector<T, C>::reference
static_rvector<T, C>::back() {
	return *data_at(m_length - 1);
}

template<typename T, uint C>
typename static_rvector<T, C>::const_reference
static_rvector<T, C>::back() const {
	return *data_at(m_length - 1);
}

template<typename T, uint C>
typename static_rvector<T, C>::value_type*
static_rvector<T, C>::data() noexcept {
	return data_at(0);
}

template<typename T, uint C>
const typename static_rvector<T, C>::value_type*
static_rvector<T, C>::data() const noexcept {
	return data_at(0);
}

template<typename T, uint C>
typename static_rvector<T, C>::const_reference
static_rvector<T, C>::at(size_type n) const {
	// TODO: unlikely
	if (n >= m_length)
		throw std::out_of_range("Index out of range: " + std::to_string(n));
	return *data_at(n);
}

template<typename T, uint C>
template<typename InputIterator, typename>
void static_rvector<T, C>::assign(InputIterator first, InputIterator last) {
	auto n = std::distance(first, last);
	std::destroy_n(data(), m_length);
	std::uninitialized_copy(first, last, data());
	m_length = n;
}
template<typename T, uint C>
void static_rvector<T, C>::assign(size_type n, const value_type& val) {
	std::fill_n(data(), n, val);
	m_length = n;
}

template<typename T, uint C>
void static_rvector<T, C>::assign(std::initializer_list<value_type> il) {
	assign(il.begin(), il.end());
}

template<typename T, uint C>
void static_rvector<T, C>::push_back(const_reference value) {
	// TODO: uncomment
	// if (m_length == C)
		// throw std::out_of_range("Out of bounds");
	new (data() + m_length++) value_type(value);
}

template<typename T, uint C>
void static_rvector<T, C>::fast_push_back(const_reference value) noexcept {
	new (data() + m_length++) value_type(value);
}

template<typename T, uint C>
void static_rvector<T, C>::push_back(value_type&& value) {
	// TODO: uncomment
	// if (m_length == C)
		// throw std::out_of_range("Out of bounds");
	new (data() + m_length++) value_type(std::move(value));
}

template<typename T, uint C>
void static_rvector<T, C>::fast_push_back(value_type&& value) noexcept {
	new (data() + m_length++) value_type(std::move(value));
}

template<typename T, uint C>
void static_rvector<T, C>::pop_back() {
	std::destroy_at(data() + --m_length);
}

template<typename T, uint C>
void static_rvector<T, C>::safe_pop_back() noexcept {
	if (m_length == 0)
		return;
	std::destroy_at(data() + --m_length);
}

template<typename T, uint C>
void static_rvector<T, C>::swap(static_rvector<T, C>& other) {
	static_rvector<T, C> *o1 = this, *o2 = &other;
	size_type min_len = std::min(o1->m_length, o2->m_length);
	for (size_type i = 0; i < min_len; ++i)
		std::swap((*this)[i], other[i]);

	if (o1->m_length != min_len)
		std::swap(o1, o2);

	std::uninitialized_move(o2->begin() + min_len, o2->end(), o1->begin() + min_len);
	std::destroy(o2->begin() + min_len, o2->end());
	std::swap(o1->m_length, o2->m_length);
}

template<typename T, uint C>
void static_rvector<T, C>::clear() noexcept {
	auto ptr = data();
	std::destroy(ptr, ptr + m_length);
	m_length = 0;
}

template<typename T, uint C>
template<typename... Args>
void static_rvector<T, C>::emplace_back(Args&&... args) {
	// TODO: uncomment
	// if (m_length == C)
		// throw std::out_of_range("Out of bounds");
	new (data() + m_length++) value_type(std::forward<Args>(args)...);
}

template<typename T, uint C>
template<typename... Args>
void static_rvector<T, C>::fast_emplace_back(Args&&... args) noexcept {
	new (data() + m_length++) value_type(std::forward<Args>(args)...);
}

template<typename T, uint C>
typename static_rvector<T, C>::value_type*
static_rvector<T, C>::data_at(size_type n) noexcept {
	return reinterpret_cast<value_type*>(&m_data[n]);
}

template<typename T, uint C>
const typename static_rvector<T, C>::value_type*
static_rvector<T, C>::data_at(size_type n) const noexcept {
	return reinterpret_cast<const value_type*>(&m_data[n]);
}

template<typename T, uint C>
std::ostream& operator<<(std::ostream& out, const static_rvector<T, C>& v) {
	for (typename static_rvector<T, C>::size_type i = 0; i < v.size() - 1; ++i)
		out << v[i] << ' ';
	if (!v.empty())
		out << v.back();
	return out;
}

template<typename T, uint C>
void swap(static_rvector<T, C>& x, static_rvector<T, C>& y) {
	x.swap(y);
}
