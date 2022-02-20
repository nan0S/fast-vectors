#include <bits/stdc++.h>
#include <mmap_allocator/mmap_allocator.hpp>
using namespace std;

void init() {
   cout << boolalpha;
}

template<class T>
void print(const T& x) {
   cout << x << endl;
}

template<>
void print(const vector<int>& v) {
   for (const auto& x : v)
      cout << x << " ";
   cout << endl;
}

template<>
void print(const vector<string>& v) {
   for (const auto& x : v)
      cout << x << " ";
   cout << endl;
}

template<>
void print(const set<int>& s) {
   for (const auto& x : s)
      cout << x << " ";
   cout << endl;
}

template<>
void print(const unordered_set<int>& s) {
   for (const auto& x : s)
      cout << x << " ";
   cout << endl;
}

template<>
void print(const string& s) {
   cout << s << endl;
}

template<>
void print(const map<int, int>& m) {
   for (const auto& [k, v] : m)
      cout << "[" << k << "," << v << "] ";
   cout << endl;
}

template<>
void print(const unordered_map<int, int>& m) {
   for (const auto& [k, v] : m)
      cout << "[" << k << "," << v << "] ";
   cout << endl;
}

template<>
void print(const unordered_map<string, string>& m) {
   for (const auto& [k, v] : m)
      cout << "[" << k << "," << v << "] ";
   cout << endl;
}

template<class T>
void initialize(T& x, size_t i) {
   x = i;
}

template<>
void initialize(vector<int>& x, size_t i) {
   x.assign(i + 1, i + 1);
}

template<>
void initialize(vector<string>& x, size_t i) {
   x.assign(i + 1, string(i + 1, 'a'));
}

template<>
void initialize(set<int>& x, size_t i) {
   for (size_t n = 0; n < i + 1; ++n)
      x.emplace(n);
}

template<>
void initialize(unordered_set<int>& x, size_t i) {
   for (size_t n = 0; n < i + 1; ++n)
      x.emplace(n);
}

template<>
void initialize(string& x, size_t i) {
   // x.assign(i + 1, '0' + i);
   x.assign(20 + i + 1, '0' + i);
}

template<>
void initialize(map<int, int>& x, size_t i) {
   for (size_t n = 0; n < i + 1; ++n)
      x.emplace(n, n);
}

template<>
void initialize(unordered_map<int, int>& x, size_t i) {
   for (size_t n = 0; n < i + 1; ++n)
      x.emplace(n, n);
}

template<>
void initialize(unordered_map<string, string>& x, size_t i) {
   for (size_t n = 0; n < i + 1; ++n)
      x.emplace(to_string(n), to_string(n));
}


int main() {
   init();

   // using T = vector<int>;
   // using T = vector<string>;
   // using T = set<int>;
   // using T = unordered_set<int>;
   // using T = string;
   // using T = map<int, int>;
   // using T = unordered_map<int, int>;
   using T = unordered_map<string, string>;

   size_t n = 10;
   T* data = allocate<T>(n);
   for (size_t i = 0; i < n; ++i) {
      initialize(data[i], i);
      print(data[i]);
   }

   T* new_data = reallocate(data, n, 2 * pagesize);
   print(data);
   print(new_data);
   print(data == new_data);

   for (size_t i = 0; i < n; ++i) {
      print(new_data[i]);
   }

   return 0;
}
