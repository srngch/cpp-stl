#ifndef __STACK_HPP__
#define __STACK_HPP__

#include <vector>

namespace ft
{

template <class T, class Container = std::vector<T> >
class stack
{
public:
	typedef Container                                  container_type;
	typedef typename container_type::value_type&       reference;
	typedef const typename container_type::value_type& const_reference;
	typedef typename container_type::value_type        value_type;
	typedef typename container_type::size_type         size_type;

protected:
	container_type c;

public:
	stack() : c() {}
	~stack() {}
	stack(const stack& q) : c(q.c) {}
	stack& operator=(const stack& q) {
		c = q.c;
		return *this;
	}

	// Test whether container is empty
	bool empty() const {
		return c.empty();
	}

	// Return size
	size_type size() const {
		return c.size();
	}

	// Access next element
	reference top() {
		return c.back();
	}
	const_reference top() const {
		return c.back();
	}

	// 	Insert element
	void push(const value_type& x) {
		c.push_back(x);
	}

	// Remove top element
	void pop() {
		c.pop_back();
	}

	template <class T2, class Container2>
	friend bool operator== (const stack<T2,Container2>& lhs, const stack<T2,Container2>& rhs);

	template <class T2, class Container2>
	friend bool operator< (const stack<T2,Container2>& lhs, const stack<T2,Container2>& rhs);
};

template <class T, class Container>
inline bool operator== (const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
	return lhs.c == rhs.c;
}

template <class T, class Container>
inline bool operator!= (const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
	return !(lhs == rhs);
}

template <class T, class Container>
inline bool operator<  (const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
	return lhs.c < rhs.c;
}

template <class T, class Container>
inline bool operator<= (const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
	return !(rhs < lhs);
}

template <class T, class Container>
inline bool operator>  (const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
	return lhs > rhs;
}

template <class T, class Container>
inline bool operator>= (const stack<T,Container>& lhs, const stack<T,Container>& rhs) {
	return !(lhs < rhs);
}
} /* namespace ft */


#endif /* __STACK_HPP__ */
