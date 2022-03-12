#ifndef _bes_lib_alg_
#define _bes_lib_alg_
#include <stdlib.h>
namespace bes {
	namespace lib {
		namespace alg {
			class allocator;
			class str;
			template <class T> class vec;
			template <class T> class str_map;
			class allocator {
			private:
				size_t max;
				size_t min;
				size_t real_size;
			public:
				void* ptr;
				allocator();
				~allocator();
				void resize(size_t);
				void upsize(size_t);
				void downsize(size_t);
				size_t size();
				size_t size_of();
			};
			class str {
			private:
				allocator self;
			public:
				char* c_str;
				str();
				str(char*);
				str(char);
				str(char*, size_t);
				str(str&);
				size_t size();
				size_t size_of();
				size_t length();
				inline char& operator[](size_t);
				str strcopy();
				str& reverse();
				str operator()(size_t);
				str operator()(size_t, size_t);
				str operator=(str&);
				void push_back(char);
				void push_back(str&);
				str operator+(str&);
				bool operator==(str&);
				bool operator!=(str&);
				bool operator>=(str&);
				bool operator<=(str&);
				bool operator>(str&);
				bool operator<(str&);
			};
			template <class T> class vec {
			private:
				allocator data;
				size_t len;
			public:
				T* ptr;
				vec();
				vec(vec<T>&);
				void push(T);
				T pop();
				T top();
				size_t size();
				size_t size_of();
				size_t length();
				T& operator[](size_t);
				vec<T> operator=(vec<T>&);
				vec<T> operator+(vec<T>&);
				vec<T> veccopy();
				vec<T> operator()(size_t);
				vec<T> operator()(size_t, size_t);
			};
			allocator::allocator() {
				ptr = malloc(0);
				max = min = real_size = 0;
				return;
			}
			allocator::~allocator() {
				free(ptr);
				return;
			}
			void allocator::resize(size_t size) {
				real_size = size;
				if (min <= size && size <= max) {
					return;
				}
				else {
					size_t n_min = size - (size >> 2);
					size_t n_max = size + (size >> 1);
					void* f = realloc(ptr, n_max);
					if (f == NULL)return;
					ptr = f;
					max = n_max;
					min = n_min;
					return;
				}
			}
			inline void allocator::upsize(size_t delta) {
				resize(delta + real_size);
				return;
			}
			inline void allocator::downsize(size_t delta) {
				resize(real_size - delta);
				return;
			}
			size_t allocator::size() {
				return real_size;
			}
			size_t allocator::size_of() {
				return max;
			}
			str::str() {
				c_str = (char*)self.ptr;
				return;
			}
			namespace function {
				size_t get_c_str_length(char* x) {
					size_t i;
					for (i = 0; x[i] != 0; i++);;
					return i;
				}
				void strcpy(char* x, char* y, size_t length) {
					for (size_t i = 0; i < length; i++) {
						x[i] = y[i];
					}
					return;
				}
			}
			str::str(char* x) {
				size_t length = function::get_c_str_length(x);
				self.resize(length);
				c_str = (char*)self.ptr;
				function::strcpy(c_str, x, length);
				return;
			}
			str::str(char x) {
				self.resize(1);
				c_str = (char*)self.ptr;
				c_str[0] = x;
				return;
			}
			str::str(char* x, size_t length) {
				self.resize(length);
				c_str = (char*)self.ptr;
				function::strcpy(c_str, x, length);
				return;
			}
			str::str(str& string0) {
				size_t len = string0.length();
				self.resize(len);
				c_str = (char*)self.ptr;
				function::strcpy(c_str, string0.c_str, len);
				return;
			}
			size_t str::size() {
				return self.size();
			}
			size_t str::size_of() {
				return self.size_of();
			}
			size_t str::length() {
				return self.size();
			}
			inline char& str::operator[](size_t index) {
				return c_str[index];
			}
			str str::strcopy() {
				return *this;
			}
			namespace function {
				inline void swap(char& x, char& y) {
					x = x ^ y;
					y = y ^ x;
					x = x ^ y;
					return;
				}
			}
			str& str::reverse() {
				size_t len = length();
				for (size_t i = 0; i < (len >> 1); i++) {
					function::swap(c_str[i], c_str[len - i]);
				}
				return *this;
			}
			str str::operator()(size_t index) {
				str ret(c_str + index, 1);
				return ret;
			}
			str str::operator()(size_t indexleft, size_t indexright) {
				str ret(c_str + indexleft, indexright - indexleft);
				return ret;
			}
			str str::operator=(str& string0) {
				size_t len = string0.length();
				self.resize(len);
				c_str = (char*)self.ptr;
				function::strcpy(c_str, string0.c_str, len);
				return string0;
			}
			void str::push_back(char c) {
				self.upsize(1);
				c_str = (char*)self.ptr;
				c_str[length() - 1] = c;
				return;
			}
			void str::push_back(str& x) {
				size_t len = length();
				self.upsize(x.length());
				c_str = (char*)self.ptr;
				function::strcpy(c_str + len, x.c_str, x.length());
				return;
			}
			str str::operator+(str& x) {
				str ret(*this);
				ret.push_back(x);
				return ret;
			}
			namespace function {
				char str_op(char* a, char* b, size_t length) {
					for (size_t i = 0; i < length; i++) {
						if (a[i] > b[i])return 1;
						else if (a[i] < b[i])return -1;
					}
					return 0;
				}
				inline size_t min(size_t x, size_t y) {
					return (x < y) ? x : y;
				}
				inline size_t max(size_t x, size_t y) {
					return (x > y) ? x : y;
				}
			}
			bool str::operator==(str& x) {
				size_t len = length();
				if (len != x.length())return false;
				else {
					char p = function::str_op((*this).c_str, x.c_str, len);
					if (p == 0)return true;
					else return false;
				}
			}
			bool str::operator!=(str& x) {
				size_t len = length();
				if (len != x.length())return true;
				else {
					char p = function::str_op((*this).c_str, x.c_str, len);
					if (p == 0)return false;
					else return true;
				}
			}
			bool str::operator>=(str& x) {
				size_t len = length();
				char p = function::str_op((*this).c_str, x.c_str, len);
				if (p != -1)return true;
				return false;
			}
			bool str::operator<=(str& x) {
				size_t len = length();
				char p = function::str_op((*this).c_str, x.c_str, len);
				if (p != 1)return true;
				return false;
			}
			bool str::operator>(str& x) {
				size_t len = length();
				char p = function::str_op((*this).c_str, x.c_str, len);
				if (p == 1)return true;
				return false;
			}
			bool str::operator<(str& x) {
				size_t len = length();
				char p = function::str_op((*this).c_str, x.c_str, len);
				if (p == -1)return true;
				return false;
			}
			template <class T> vec<T>::vec() {
				length = 0;
				return;
			}
			template <class T> vec<T>::vec(vec<T>& vec0) {
				len = vec0.length();
				data.resize(sizeof(T) * len);
				ptr = (T*)data.ptr;
				strcpy((char*)ptr, (char*)vec0.ptr, sizeof(T) * len);
				return;
			}
			template <class T>void vec<T>::push(T x) {
				data.upsize(sizeof(T));
				ptr = (T*)data.ptr;
				ptr[len] = x;
				++len;
				return;
			}
			template <class T>T vec<T>::pop() {
				--len;
				T ret = ptr[len];
				data.downsize(sizeof(T));
				ptr = (T*)data.ptr;
				return ret;
			}
			template <class T>T vec<T>::pop() {
				return ptr[len-1];
			}
			template <class T> size_t vec<T>::size() {
				return len;
			}
			template <class T> size_t vec<T>::length() {
				return len;
			}
			template <class T> size_t vec<T>::size_of() {
				return len * sizeof(T);
			}
			template <class T> T& vec<T>::operator[](size_t index) {
				return ptr[index];
			}
			template <class T> vec<T> vec<T>::operator=(vec<T>& x) {
				len = x.length();
				data.resize(sizeof(T) * len);
				ptr = (T*)data.ptr;
				strcpy((char*)ptr, (char*)x.ptr, sizeof(T) * len);
				return;
			}
			template <class T> vec<T> vec<T>::operator+(vec<T>& x) {
				return;
			}
			template <class T> vec<T> vec<T>::veccopy() {
				vec<T> ret;
				return ret;
			}
			template <class T> vec<T> vec<T>::operator()(size_t index) {
				;
			}
		}
	}
}
#endif
