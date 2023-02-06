#include <iostream>
#include <vector>
#include <stack>
#include "stack.hpp"
#include "rb_tree.hpp"

#include "pair.hpp"
#include "function.hpp"

void test_stack() {
	std::stack<int,std::vector<int> > org_stack;
	org_stack.push(10);
	org_stack.push(20);
	std::cout << "org_stack\t size: " << org_stack.size() << std::endl;

	std::stack<int,std::vector<int> > org_stack2(org_stack);
	if (org_stack == org_stack2)
		std::cout << "org_stack == org_stack2" << std::endl;
	else
		std::cout << "org_stack != org_stack2" << std::endl;

	ft::stack<int> ft_stack;
	ft_stack.push(10);
	ft_stack.push(20);
	std::cout << "ft_stack\t size: " << ft_stack.size() << std::endl;

	ft::stack<int> ft_stack2(ft_stack);
	if (ft_stack == ft_stack2)
		std::cout << "ft_stack == ft_stack2" << std::endl;
	else
		std::cout << "ft_stack != ft_stack2" << std::endl;
}

void test_rb_tree() {
	typedef ft::_Rb_tree<std::string, ft::pair<const std::string, int>, ft::_Select1st<ft::pair<const std::string, int> > >
      _rep_type;
	_rep_type _tree;

	typedef _rep_type::iterator tree_it;

	_tree.insert_unique(ft::make_pair("a", 11));
	_tree.insert_unique(ft::make_pair("b", 22));
	
	for (tree_it iter = _tree.begin(); iter != _tree.end(); iter++) {
		ft::pair<const std::string, int> p = *iter;
		std::cout << p.first << ": " << p.second << std::endl;
	}
}

int main ()
{
	test_stack();
	test_rb_tree();
	return 0;
}
