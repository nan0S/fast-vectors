#pragma once

#include <cinttypes>
#include <iterator>
#include <algorithm>

namespace uwr {	

#define CPP_ABOVE_17 __cplusplus > 201703L
typedef unsigned int uint;

template<typename T, uint C>
class static_vector;

template<typename T, uint C>
constexpr inline bool operator==(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs);
template<typename T, uint C>
constexpr inline bool operator!=(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs);
template<typename T, uint C>
constexpr inline bool operator<(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs);
template<typename T, uint C>
constexpr inline bool operator<=(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs);
template<typename T, uint C>
constexpr inline bool operator>(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs);
template<typename T, uint C>
constexpr inline bool operator>=(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs);
template<typename T, uint C>
constexpr std::ostream& operator<<(std::ostream& out, const static_vector<T, C>& v);

#if CPP_ABOVE_17
inline constexpr struct synth_three_way_t {
	template<typename T, std::totally_ordered_with<T> U>
	auto operator()(const T& lhs, const U& rhs) {
		if constexpr (std::three_way_comparable_with<T, U>)
			return lhs <=> rhs;
		else {
			if (lhs == rhs)
				return std::strong_ordering::equal;
			else if (lhs < rhs)
				return std::strong_ordering::less;
			else
				return std::strong_ordering::greater;
		}
	}
} synth_three_way;

template<typename T, uint C>
constexpr inline auto operator<=>(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs);
#endif /* CPP_ABOVE_17 */

template<typename T, uint C>
constexpr void swap(static_vector<T, C>& x, static_vector<T, C>& y);

template<typename T, uint C>
class static_vector {
public:
	using value_type = T;
	using size_type = uint_fast32_t;
	using difference_type = ptrdiff_t;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;
	using iterator = pointer;
	using const_iterator = const_pointer;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	constexpr static_vector() noexcept;
	constexpr explicit static_vector(size_type n);
	constexpr static_vector(size_type n, const T& val);
	template<typename InputIterator,
			 typename = typename std::iterator_traits<InputIterator>::T>
	constexpr static_vector(InputIterator first, InputIterator last);
	constexpr static_vector(const static_vector& x);
	constexpr static_vector(static_vector&& x) noexcept;
	constexpr static_vector(std::initializer_list<T> il);

	#if CPP_ABOVE_17
	constexpr
	#endif
	~static_vector();

	constexpr static_vector& operator=(const static_vector& other) noexcept;
	constexpr static_vector& operator=(static_vector&& other) noexcept;
	constexpr static_vector& operator=(std::initializer_list<T> il) noexcept;

	constexpr iterator begin() noexcept;
	constexpr const_iterator begin() const noexcept;
	constexpr iterator end() noexcept;
	constexpr const_iterator end() const noexcept;

	constexpr reverse_iterator rbegin() noexcept;
	constexpr const_reverse_iterator rbegin() const noexcept;
	constexpr reverse_iterator rend() noexcept;
	constexpr const_reverse_iterator rend() const noexcept;

	constexpr const_iterator cbegin() const noexcept;
	constexpr const_iterator cend() const noexcept;
	constexpr const_reverse_iterator crbegin() const noexcept;
	constexpr const_reverse_iterator crend() const noexcept;

	constexpr size_type size() const noexcept;
	constexpr size_type max_size() const noexcept;
	constexpr void resize(size_type n);
	constexpr void resize(size_type n, const T& val);
	constexpr size_type capacity() const noexcept;
	[[nodiscard]] constexpr bool empty() const noexcept;
	constexpr void reserve(size_type n) noexcept;
	constexpr void shrink_to_fit() noexcept;

	constexpr reference operator[](size_type n);
	constexpr const_reference operator[](size_type n) const;
	constexpr reference at(size_type n);
	constexpr const_reference at(size_type n) const;
	constexpr reference front();
	constexpr const_reference front() const;
	constexpr reference back();
	constexpr const_reference back() const;
	constexpr T* data() noexcept;
	constexpr const T* data() const noexcept;

	template<typename InputIterator,
		typename = typename std::iterator_traits<InputIterator>::T>
	constexpr void assign(InputIterator first, InputIterator last);
	constexpr void assign(size_type n, const T& val);
	constexpr void assign(std::initializer_list<T> il);

	constexpr void push_back(const T& value);
	constexpr void fast_push_back(const T& value) noexcept;
	constexpr void push_back(T&& value);
	constexpr void fast_push_back(T&& value) noexcept;

	constexpr void pop_back();
	constexpr void safe_pop_back() noexcept;
	// TODO: insert
	// TODO: erase
	constexpr void swap(static_vector& other);
	constexpr void clear() noexcept;
	// TODO: emplace

	template<typename... Args>
	constexpr void emplace_back(Args&&... args);
	template<typename... Args>
	constexpr void fast_emplace_back(Args&&... args) noexcept;

private:
	constexpr T* data_at(size_type n) noexcept;
	constexpr const T* data_at(size_type n) const noexcept;

private:
	typename std::aligned_storage<sizeof(T), alignof(T)>::type m_data[C];
	size_type m_length;
};

template<typename T, uint C>
constexpr static_vector<T, C>::static_vector() noexcept :
	m_length(0)
{
}

template<typename T, uint C>
constexpr static_vector<T, C>::static_vector(size_type n) :
	m_length(n) {
	std::uninitialized_default_construct_n(data(), n);
}

template<typename T, uint C>
constexpr static_vector<T, C>::static_vector(size_type n, const T& val) :
	m_length(n) {
	std::uninitialized_fill_n(data(), n, val);
}

template<typename T, uint C>
template<typename InputIterator, typename>
constexpr static_vector<T, C>::static_vector(InputIterator first, InputIterator last) :
	m_length(std::distance(first, last)) {
	std::uninitialized_copy(first, last, data());
}

template<typename T, uint C>
constexpr static_vector<T, C>::static_vector(const static_vector& x) :
	m_length(x.m_length) {
	std::uninitialized_copy(x.begin(), x.end(), data());
}

template<typename T, uint C>
constexpr static_vector<T, C>::static_vector(static_vector&& x) noexcept :
	m_length(x.m_length) {
	std::uninitialized_move(x.begin(), x.end(), data());
}
	
template<typename T, uint C>
constexpr static_vector<T, C>::static_vector(std::initializer_list<T> il) :
	m_length(il.size()) {
	std::uninitialized_copy(il.begin(), il.end(), begin());
}

template<typename T, uint C>
#if CPP_ABOVE_17
constexpr
#endif
static_vector<T, C>::~static_vector() {
	std::destroy_n(data(), m_length);
}

template<typename T, uint C>
constexpr static_vector<T, C>& static_vector<T, C>::operator=(const static_vector<T, C>& other) noexcept {
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
constexpr static_vector<T, C>& static_vector<T, C>::operator=(static_vector<T, C>&& other) noexcept {
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
constexpr static_vector<T, C>& static_vector<T, C>::operator=(std::initializer_list<T> il) noexcept {
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
typename static_vector<T, C>::iterator
constexpr static_vector<T, C>::begin() noexcept {
	return data();
}

template<typename T, uint C>
typename static_vector<T, C>::const_iterator
constexpr static_vector<T, C>::begin() const noexcept {
	return data();
}

template<typename T, uint C>
typename static_vector<T, C>::iterator
constexpr static_vector<T, C>::end() noexcept {
	return data() + m_length;
}

template<typename T, uint C>
typename static_vector<T, C>::const_iterator
constexpr static_vector<T, C>::end() const noexcept {
	return data() + m_length;
}

template<typename T, uint C>
typename static_vector<T, C>::reverse_iterator
constexpr static_vector<T, C>::rbegin() noexcept {
	return data() + m_length;
}

template<typename T, uint C>
typename static_vector<T, C>::const_reverse_iterator
constexpr static_vector<T, C>::rbegin() const noexcept {
	return data() + m_length;
}

template<typename T, uint C>
typename static_vector<T, C>::reverse_iterator
constexpr static_vector<T, C>::rend() noexcept {
	return data();
}

template<typename T, uint C>
typename static_vector<T, C>::const_reverse_iterator
constexpr static_vector<T, C>::rend() const noexcept {
	return data();
}

template<typename T, uint C>
typename static_vector<T, C>::const_iterator
constexpr static_vector<T, C>::cbegin() const noexcept {
	return data();
}

template<typename T, uint C>
typename static_vector<T, C>::const_iterator
constexpr static_vector<T, C>::cend() const noexcept {
	return data() + m_length;
}

template<typename T, uint C>
typename static_vector<T, C>::const_reverse_iterator
constexpr static_vector<T, C>::crbegin() const noexcept {
	return data() + m_length;
}

template<typename T, uint C>
typename static_vector<T, C>::const_reverse_iterator
constexpr static_vector<T, C>::crend() const noexcept {
	return data();
}

template<typename T, uint C>
typename static_vector<T, C>::size_type
constexpr static_vector<T, C>::size() const noexcept {
	return m_length;
}

template<typename T, uint C>
constexpr typename static_vector<T, C>::size_type
static_vector<T, C>::max_size() const noexcept {
	return C;
}

template<typename T, uint C>
constexpr void static_vector<T, C>::resize(size_type n) {
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
constexpr void static_vector<T, C>::resize(size_type n, const T& val) {
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
constexpr typename static_vector<T, C>::size_type
static_vector<T, C>::capacity() const noexcept {
	return C;
}

template<typename T, uint C>
constexpr bool static_vector<T, C>::empty() const noexcept {
	return m_length == 0;
}

template<typename T, uint C>
constexpr void static_vector<T, C>::reserve(size_type n) noexcept {
}

template<typename T, uint C>
constexpr void static_vector<T, C>::shrink_to_fit() noexcept {
}

template<typename T, uint C>
typename static_vector<T, C>::reference
constexpr static_vector<T, C>::operator[](size_type n) {
	return *data_at(n);
}

template<typename T, uint C>
typename static_vector<T, C>::const_reference
constexpr static_vector<T, C>::operator[](size_type n) const {
	return *data_at(n);
}

template<typename T, uint C>
constexpr typename static_vector<T, C>::reference
static_vector<T, C>::at(size_type n) {
	if (n >= m_length)
		throw std::out_of_range("Index out of range: " + std::to_string(n));
	return *data_at(n);
}

template<typename T, uint C>
typename static_vector<T, C>::reference
constexpr static_vector<T, C>::front() {
	return *data_at(0);
}

template<typename T, uint C>
typename static_vector<T, C>::const_reference
constexpr static_vector<T, C>::front() const {
	return *data_at(0);
}

template<typename T, uint C>
typename static_vector<T, C>::reference
constexpr static_vector<T, C>::back() {
	return *data_at(m_length - 1);
}

template<typename T, uint C>
typename static_vector<T, C>::const_reference
constexpr static_vector<T, C>::back() const {
	return *data_at(m_length - 1);
}

template<typename T, uint C>
constexpr T* static_vector<T, C>::data() noexcept {
	return data_at(0);
}

template<typename T, uint C>
constexpr const T* static_vector<T, C>::data() const noexcept {
	return data_at(0);
}

template<typename T, uint C>
typename static_vector<T, C>::const_reference
constexpr static_vector<T, C>::at(size_type n) const {
	// TODO: unlikely
	if (n >= m_length)
		throw std::out_of_range("Index out of range: " + std::to_string(n));
	return *data_at(n);
}

template<typename T, uint C>
template<typename InputIterator, typename>
constexpr void static_vector<T, C>::assign(InputIterator first, InputIterator last) {
	auto n = std::distance(first, last);
	std::destroy_n(data(), m_length);
	std::uninitialized_copy(first, last, data());
	m_length = n;
}
template<typename T, uint C>
constexpr void static_vector<T, C>::assign(size_type n, const T& val) {
	std::fill_n(data(), n, val);
	m_length = n;
}

template<typename T, uint C>
constexpr void static_vector<T, C>::assign(std::initializer_list<T> il) {
	assign(il.begin(), il.end());
}

template<typename T, uint C>
constexpr void static_vector<T, C>::push_back(const_reference value) {
	// TODO: uncomment
	// if (m_length == C)
		// throw std::out_of_range("Out of bounds");
	new (data() + m_length++) T(value);
}

template<typename T, uint C>
constexpr void static_vector<T, C>::fast_push_back(const_reference value) noexcept {
	new (data() + m_length++) T(value);
}

template<typename T, uint C>
constexpr void static_vector<T, C>::push_back(T&& value) {
	// TODO: uncomment
	// if (m_length == C)
		// throw std::out_of_range("Out of bounds");
	new (data() + m_length++) T(std::move(value));
}

template<typename T, uint C>
constexpr void static_vector<T, C>::fast_push_back(T&& value) noexcept {
	new (data() + m_length++) T(std::move(value));
}

template<typename T, uint C>
constexpr void static_vector<T, C>::pop_back() {
	std::destroy_at(data() + --m_length);
}

template<typename T, uint C>
constexpr void static_vector<T, C>::safe_pop_back() noexcept {
	if (m_length == 0)
		return;
	std::destroy_at(data() + --m_length);
}

template<typename T, uint C>
constexpr void static_vector<T, C>::swap(static_vector<T, C>& other) {
	static_vector<T, C> *o1 = this, *o2 = &other;
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
constexpr void static_vector<T, C>::clear() noexcept {
	auto ptr = data();
	std::destroy(ptr, ptr + m_length);
	m_length = 0;
}

template<typename T, uint C>
template<typename... Args>
constexpr void static_vector<T, C>::emplace_back(Args&&... args) {
	// TODO: uncomment
	// if (m_length == C)
		// throw std::out_of_range("Out of bounds");
	new (data() + m_length++) T(std::forward<Args>(args)...);
}

template<typename T, uint C>
template<typename... Args>
constexpr void static_vector<T, C>::fast_emplace_back(Args&&... args) noexcept {
	new (data() + m_length++) T(std::forward<Args>(args)...);
}

template<typename T, uint C>
constexpr T* static_vector<T, C>::data_at(size_type n) noexcept {
	return reinterpret_cast<T*>(&m_data[n]);
}

template<typename T, uint C>
constexpr const T* static_vector<T, C>::data_at(size_type n) const noexcept {
	return reinterpret_cast<const T*>(&m_data[n]);
}

template<typename T, uint C>
constexpr bool operator==(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs) {
	if (lhs.size() != rhs.size())
		return false;
	return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename T, uint C>
constexpr bool operator!=(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs) {
	return !(lhs == rhs);
}

template<typename T, uint C>
constexpr bool operator<(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs) {
	return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<typename T, uint C>
constexpr bool operator<=(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs) {
	return !(rhs < lhs);
}

template<typename T, uint C>
constexpr bool operator>(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs) {
	return rhs < lhs;
}

template<typename T, uint C>
constexpr bool operator>=(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs) {
	return !(lhs < rhs);
}

#if CPP_ABOVE_17
template<typename T, uint C>
constexpr auto operator<=>(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs) {
	return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(),
												  rhs.begin(), rhs.end(),
												  synth_three_way);
}
#endif

template<typename T, uint C>
constexpr std::ostream& operator<<(std::ostream& out, const static_vector<T, C>& v) {
	for (typename static_vector<T, C>::size_type i = 0; i < v.size() - 1; ++i)
		out << v[i] << ' ';
	if (!v.empty())
		out << v.back();
	return out;
}

template<typename T, uint C>
constexpr void swap(static_vector<T, C>& x, static_vector<T, C>& y) {
	x.swap(y);
}

} // namespace uwr