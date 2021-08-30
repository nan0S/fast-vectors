#pragma once

#include <string.h>
#include <algorithm>
#include <memory>
#include <linux/mman.h>

// TODO: remove
#ifdef RVECTOR_TRACK
#include "../common/alloc_counter.hpp"
#endif

#define RVECTOR_LIKELY(x)       __builtin_expect((x),1)
#define RVECTOR_UNLIKELY(x)     __builtin_expect((x),0)

template<class T>
class rvector;

// TODO: remove
// #define RVECTOR_VERBOSE_PRINTING

namespace mm
{
	// TODO: remove
	#ifdef RVECTOR_TRACK
	uwr::mem::alloc_counter counter("rvector");
	#endif

	// Policies
	template<typename T>
	using Trivial = std::enable_if_t<std::is_trivial<T>::value>;

	template<typename T>
	using T_Destr = std::enable_if_t<std::is_trivially_destructible<T>::value>;
	template<typename T>
	using NT_Destr = std::enable_if_t<!std::is_trivially_destructible<T>::value>;

	template<typename T, typename R = void>
	using T_Copy = std::enable_if_t<std::is_trivially_copy_constructible<T>::value, R>;
	template<typename T, typename R = void>
	using NT_Copy = std::enable_if_t<!std::is_trivially_copy_constructible<T>::value, R>;

	template<typename T, typename R = void>
	using T_Move = std::enable_if_t<std::is_trivially_move_constructible<T>::value, R>;
	template<typename T, typename R = void>
	using NT_Move = std::enable_if_t<!std::is_trivially_move_constructible<T>::value, R>;

	template<typename T>
	using T_Move_a = std::enable_if_t<std::is_trivially_move_assignable<T>::value>;
	template<typename T>
	using NT_Move_a = std::enable_if_t<!std::is_trivially_move_assignable<T>::value>;

	using size_type = size_t;
	template <typename T>
	constexpr size_t map_threshold = 4096 / sizeof(T);

	template<typename T>
	T* allocate(size_type n)
	{
        if(n > map_threshold<T>)
	    	return (T*) mmap(NULL, n*sizeof(T), 
	                PROT_READ | PROT_WRITE,
	                MAP_PRIVATE | MAP_ANONYMOUS,
	                -1, 0);
        else
        	return (T*) malloc(n*sizeof(T));
	}

	template<typename T>
	void deallocate(T* p, size_type n)
	{
        if (n > map_threshold<T>)
	    	munmap(p, n*sizeof(T));
        else
	        free(p);
	}

// destruct
	template<typename T>
	NT_Destr<T>
	destruct(T* begin, T* end)
	{
		std::destroy(begin, end);
	}

	template<typename T>
	T_Destr<T>
	destruct(T*, T*)
	{
	}

// fill
	template<typename T>
	T_Copy<T> fill(T* data, size_type n, const T& value = T())
	{
		std::uninitialized_fill_n(data, n, value);
	}

	template<typename T>
	NT_Copy<T> fill(T* data, size_type n, const T& value = T())
	{
		std::uninitialized_fill_n(data, n, value);
	}

	template<typename T, typename InIt>
	T_Copy<T> fill(T* data, InIt begin, InIt end)
	{
        std::copy(begin, end, data);
	}

	template<typename T, typename InIt>
	NT_Copy<T> fill(T* data, InIt begin, InIt end)
	{
		std::uninitialized_copy(begin, end, data);
	}

// fix_capacity
	template <typename T>
	size_type fix_capacity(size_type n)
	{
		if(n < map_threshold<T>)
			return n;
	        // return std::max(64/sizeof(T), n);
	    // return map_threshold<T> * (n/map_threshold<T> + 1);
	    // return map_threshold<T> * ((n + map_threshold<T> - 1) / map_threshold<T>);
	    return ((n * sizeof(T) + 4096 - 1) / 4096) * 4096 / sizeof(T);
	}

// realloc
	template<typename T>
	T_Move<T, T*> realloc_(T* data, 
							size_type length, 
							size_type capacity, 
							size_type n)
	{
        if((n > map_threshold<T>) != (capacity > map_threshold<T>))
	    {
	        T* new_data = allocate<T>(n);
	        memcpy(new_data, data, length * sizeof(T));
	        deallocate(data, capacity);
	        return new_data;
	    }
        else
	    {
	        if(capacity > map_threshold<T>) {
	        	// TODO: remove
	        	#ifdef RVECTOR_TRACK
	        	T* ret = (T*) mremap(data, capacity*sizeof(T), 
                        		n*sizeof(T), MREMAP_MAYMOVE);
	        	counter.mremaps(ret == data);
	        	return ret;
	        	#else
            	return (T*) mremap(data, capacity*sizeof(T), 
                        		n*sizeof(T), MREMAP_MAYMOVE);
            	#endif
	        }
	        else
	        	return (T*) realloc(data, n*sizeof(T));
	    }
	}

	template<typename T>
	NT_Move<T, T*> realloc_(T* data, 
							size_type length, 
							size_type capacity, 
							size_type n)
	{
        if(capacity > map_threshold<T>)
        {
            void* new_data = mremap(data, capacity*sizeof(T), 
                        		n*sizeof(T), 0);

            // TODO: remove
            #ifdef RVECTOR_TRACK
            counter.mremaps(data == (T*)new_data);
            #endif

            #ifdef RVECTOR_VERBOSE_PRINTING
            if (data == (T*)new_data) {
                std::cout << "rv: "
                	<< (capacity * sizeof(T) / 4096)
                	<< " -> "
                	<< (n * sizeof(T) / 4096)
                	<< std::endl;
            }
            #endif

            if(new_data != (void*)-1) {
                return (T*) new_data;
            }
        }
	    T* new_data = allocate<T>(n);
	    std::uninitialized_move_n(data, length, new_data);
	    destruct(data, data + length);
	    deallocate(data, capacity);
	    return new_data;
	}

// change_capacity
	template<typename T>
	void change_capacity(T*& data, 
						size_type length, 
						size_type& capacity, 
						size_type n)
	{
		size_type new_capacity = fix_capacity<T>(n);
		if(RVECTOR_UNLIKELY(new_capacity < map_threshold<T> and capacity > map_threshold<T>))
			return;

		// TODO: remove
		#ifdef RVECTOR_TRACK
		if (capacity)
			counter.grows(double(new_capacity) / capacity);
		#endif

	    if(data)
	        data = realloc_(data, length, capacity, new_capacity);
	    else
	        data = allocate<T>(new_capacity);
	    capacity = new_capacity;
	}

#include <iostream>
// grow
	template<typename T>
	void grow(T*& data, size_type length, size_type& capacity)
	{
		if(RVECTOR_LIKELY(length < capacity)) return;
		change_capacity(data, length, capacity, capacity*2 + !capacity);
	}

// TODO: check if policies are sufficient
// shiftr data
	template<typename T>
	T_Move_a<T> 
	shiftr_data(T* begin, size_type end)
	{
		memmove(begin + 1, begin, end * sizeof(T));
	}

	template<typename T>
	NT_Move_a<T> 
	shiftr_data(T* begin, size_type end)
	{
        if (RVECTOR_UNLIKELY(!end))
            return;
		auto end_p = begin + end;
		new (end_p) T(std::move(*(end_p - 1)));
		std::move_backward(begin, end_p - 1, end_p);
		begin->~T();
	}
} // namespace mm
