#include <iostream>
#include <vector>
#include <stack>
#include "stack.hpp"

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

int main ()
{
	test_stack();
	return 0;
}
