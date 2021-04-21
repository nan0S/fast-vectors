#include <iostream>
#include <vector>
#include <boost/container/static_vector.hpp>
#include <iterator>

template<class T,uint C>
class trivial_static_vector {
public:
	// using tsvsize_t = unsigned int;
	using tsvsize_t = size_t;

	trivial_static_vector(): s(0) {}
	trivial_static_vector(const trivial_static_vector<T,C> &v): s(v.s) {
		for (tsvsize_t i = 0; i < v.s; i++) t[i] = v.t[i];
	}

	tsvsize_t size() const { return s; }
	constexpr tsvsize_t capacity() const { return C; }
	constexpr tsvsize_t max_size() const { return C; }

	T& operator[](const tsvsize_t i) { return t[i]; }
	T operator[](const tsvsize_t i) const { return t[i]; }
	T& back() { return t[s-1]; }
	T back() const { return t[s-1]; }
	T& front() { return t[0]; }
	T front() const { return t[0]; }

	template<class... Args> void emplace_back(Args&&... args) {
		if (s == C)
			throw std::out_of_range("test");
		t[s++] = T(std::forward<Args>(args)...);
	}
	void push_back(const T &e) { t[s++] = e; }
	void pop_back() { s--; }
	void resize(tsvsize_t new_size) { s = new_size; }
	void clear() { s = 0; }

	trivial_static_vector& operator=(const trivial_static_vector &v) {
		s = v.s;
		for (tsvsize_t i = 0; i < v.s; i++) 
			t[i] = v.t[i];
		return *this;
	}

	bool operator==(const trivial_static_vector &v) const {
		if (s != v.s) return false;
		for (tsvsize_t i = 0; i < s; i++) if (!(t[i] == v.t[i])) return false;
		return true;
	}
	bool operator!=(const trivial_static_vector &v) const { return !((*this) == v); }

	template<bool is_const>
	class tsv_iterator: public std::iterator<std::random_access_iterator_tag, T> {
		using cT = typename std::conditional<is_const, const T*, T*>::type;
		using iterator_category = std::output_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = cT;
		using reference = typename std::remove_pointer<cT>::type&;
		using iterator = tsv_iterator<is_const>;
		cT p;
		public:
		tsv_iterator(const T *_p): p(_p) {}
		tsv_iterator(T *_p): p(_p) {}
		tsv_iterator(const tsv_iterator&) = default;
		template<bool was_const,class = std::enable_if_t<is_const || !was_const>> tsv_iterator(const tsv_iterator<was_const> v): p(v.p) {}
		template<bool was_const,class = std::enable_if_t<is_const && !was_const>> tsv_iterator& operator=(const tsv_iterator<was_const> v) {p(v.p); return *this;}
		iterator  operator++(int)                    {return p++;}
		iterator& operator++()                       {++p; return *this;}
		iterator  operator--(int)                    {return p--;}
		iterator& operator--()                       {--p; return *this;}
		iterator  operator+(difference_type v) const {return p + v;}
		iterator  operator-(difference_type v) const {return p - v;}
		reference operator*() const                  {return *p;}
		pointer   operator->() const                 {return p;}
		template<bool was_const> bool operator==(const tsv_iterator<was_const> it) const {return p == it.p;}
		template<bool was_const> bool operator!=(const tsv_iterator<was_const> it) const {return p != it.p;}
	};
	using iterator = tsv_iterator<false>;
	using const_iterator = tsv_iterator<true>;
	static_assert(std::is_convertible_v<const_iterator, const_iterator>);
	static_assert(std::is_convertible_v<iterator, const_iterator>);
	static_assert(!std::is_convertible_v<const_iterator, iterator>);
	static_assert(std::is_convertible_v<iterator, iterator>);
	static_assert(std::is_trivially_copy_constructible_v<const_iterator>);
	static_assert(std::is_trivially_copy_constructible_v<iterator>);

	tsv_iterator<false> begin() {return tsv_iterator<false>(&t[0]);}
	tsv_iterator<false> end() {return tsv_iterator<false>(&t[s]);}
	tsv_iterator<true> begin() const {return tsv_iterator<true>(&t[0]);}
	tsv_iterator<true> end() const {return tsv_iterator<true>(&t[s]);}
	tsv_iterator<true> cbegin() const {return tsv_iterator<true>(&t[0]);}
	tsv_iterator<true> cend() const {return tsv_iterator<true>(&t[s]);}

private:
	tsvsize_t s;
	T t[C];
};

template<class T> class trivial_static_vector<T,1> {
	using tsvsize_t = unsigned int;
	// using tsvsize_t = size_t;
	T t;
	public:
	constexpr tsvsize_t size() const {return 1;}
	constexpr tsvsize_t capacity() const {return 1;}
	constexpr tsvsize_t max_size() const {return 1;}

	T& operator[](const tsvsize_t i) {return t;}
	T operator[](const tsvsize_t i) const {return t;}
	T& back() {return t;}
	T back() const {return t;}
	T& front() {return t;}
	T front() const {return t;}

	template<class... Args> void emplace_back(Args&&... args) {t = T(std::forward<Args>(args)...);}
	void push_back(const T &e) {t = e;}
	void pop_back() {}
	void resize(tsvsize_t new_size) {}
	void clear() {}

	trivial_static_vector& operator=(const trivial_static_vector &v) {
		t = v.t;
		return *this;
	}

	bool operator==(const trivial_static_vector &v) const {return (t == v.t);}
	bool operator!=(const trivial_static_vector &v) const {return !((*this)==v);}

	template<bool is_const> class tsv_iterator: public std::iterator<std::random_access_iterator_tag, T> {
		using cT = typename std::conditional<is_const, const T*, T*>::type;
		using iterator_category = std::output_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = cT;
		using reference = typename std::remove_pointer<cT>::type&;
		using iterator = tsv_iterator<is_const>;
		cT p;
		public:
		tsv_iterator(const T *_p): p(_p) {}
		tsv_iterator(T *_p): p(_p) {}
		tsv_iterator(const tsv_iterator&) = default;
		template<bool was_const,class = std::enable_if_t<is_const || !was_const>> tsv_iterator(const tsv_iterator<was_const> v): p(v.p) {}
		template<bool was_const,class = std::enable_if_t<is_const && !was_const>> tsv_iterator& operator=(const tsv_iterator<was_const> v) {p(v.p); return *this;}
		iterator  operator++(int)                    {return p++;}
		iterator& operator++()                       {++p; return *this;}
		iterator  operator--(int)                    {return p--;}
		iterator& operator--()                       {--p; return *this;}
		iterator  operator+(difference_type v) const {return p + v;}
		iterator  operator-(difference_type v) const {return p - v;}
		reference operator*() const                  {return *p;}
		pointer   operator->() const                 {return p;}
		template<bool was_const> bool operator==(const tsv_iterator<was_const> it) const {return p == it.p;}
		template<bool was_const> bool operator!=(const tsv_iterator<was_const> it) const {return p != it.p;}
	};
	using iterator = tsv_iterator<false>;
	using const_iterator = tsv_iterator<true>;
	static_assert(std::is_convertible_v<const_iterator, const_iterator>);
	static_assert(std::is_convertible_v<iterator, const_iterator>);
	static_assert(!std::is_convertible_v<const_iterator, iterator>);
	static_assert(std::is_convertible_v<iterator, iterator>);
	static_assert(std::is_trivially_copy_constructible_v<const_iterator>);
	static_assert(std::is_trivially_copy_constructible_v<iterator>);

	tsv_iterator<false> begin() {return tsv_iterator<false>(&t[0]);}
	tsv_iterator<false> end() {return tsv_iterator<false>(&t+1);}
	tsv_iterator<true> begin() const {return tsv_iterator<true>(&t);}
	tsv_iterator<true> end() const {return tsv_iterator<true>(&t+1);}
	tsv_iterator<true> cbegin() const {return tsv_iterator<true>(&t);}
	tsv_iterator<true> cend() const {return tsv_iterator<true>(&t+1);}
};



using uint = unsigned int;
static constexpr uint COUNT = 50000;

//using Vector = std::vector<uint>;
// using Vector = boost::container::static_vector<uint, COUNT>;
// using Vector = boost::container::vector<int>;
using Vector = trivial_static_vector<uint, COUNT>;

int main() {
	uint sum = 0;
	for (uint i = 0; i < COUNT; i++) {
		Vector v;
		for (uint i = 0; i < COUNT; i++) v.emplace_back(i);

		for (auto x: v) sum += x;// Way 1
		// for (uint i = 0; i < v.size(); i++) sum += v[i]; // Way 2

		/*
		   for (auto x: v) sum += x;
		   for (auto it = v.begin(); it != v.end(); it++) sum += *it;
		   for (auto it = v.cbegin(); it != v.cend(); it++) sum += *it;
		   auto const &w = v;
		   for (auto x: w) sum += x;
		   for (auto it = w.begin(); it != w.end(); it++) sum += *it;
		   for (auto it = w.cbegin(); it != w.cend(); it++) sum += *it;
		   */
	}
	std::cout << sum << std::endl;
}
