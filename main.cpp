#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include "vector.hpp"
#include "stack.hpp"
#include "map.hpp"
#include "set.hpp"
#include "rb_tree.hpp"

#include "pair.hpp"
#include "function.hpp"

// #define FT_STL

#ifdef FT_STL //CREATE A REAL STL EXAMPLE
  #include <vector>
  #include <stack>
  #include <map>
  #include <set>
  namespace ft = std;
#else
  #include "vector.hpp"
  #include "stack.hpp"
  #include "map.hpp"
  #include "set.hpp"
#endif

// test code from the subject

#define MAX_RAM 4294967296
#define BUFFER_SIZE 4096
struct Buffer
{
  int idx;
  char buff[BUFFER_SIZE];
};

#define COUNT (MAX_RAM / (int)sizeof(Buffer))

template<typename T>
class MutantStack : public ft::stack<T> {
public:
  MutantStack() {}
  MutantStack(const MutantStack<T>& src) { *this = src; }
  MutantStack<T>& operator=(const MutantStack<T>& rhs) {
    this->c = rhs.c;
    return *this;
  }
  ~MutantStack() {}

  typedef typename ft::stack<T>::container_type::iterator iterator;

  iterator begin() { return this->c.begin(); }
  iterator end() { return this->c.end(); }
};

int test_subject(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: ./test seed" << std::endl;
    std::cerr << "Provide a seed please" << std::endl;
    std::cerr << "Count value:" << COUNT << std::endl;
    return 1;
  }
  const int seed = atoi(argv[1]);
  srand(seed);

  ft::vector<std::string> vector_str;
  ft::vector<int> vector_int;
  ft::stack<int> stack_int;
  ft::vector<Buffer> vector_buffer;
  ft::stack<Buffer, std::deque<Buffer> > stack_deq_buffer;
  ft::map<int, int> map_int;

  for (int i = 0; i < COUNT; i++) {
    vector_buffer.push_back(Buffer());
  }

  for (int i = 0; i < COUNT; i++) {
    const int idx = rand() % COUNT;
    vector_buffer[idx].idx = 5;
  }
  ft::vector<Buffer>().swap(vector_buffer);

  try {
    for (int i = 0; i < COUNT; i++) {
      const int idx = rand() % COUNT;
      vector_buffer.at(idx);
      std::cerr << "Error: THIS VECTOR SHOULD BE EMPTY!!" <<std::endl;
    }
  }
  catch(const std::exception& e) {
    //NORMAL ! :P
  }

  for (int i = 0; i < COUNT; ++i) {
    map_int.insert(ft::make_pair(rand(), rand()));
  }

  int sum = 0;
  for (int i = 0; i < 10000; i++) {
    int access = rand();
    sum += map_int[access];
  }
  std::cout << "should be constant with the same seed: " << sum << std::endl;

  {
    ft::map<int, int> copy = map_int;
  }
  MutantStack<char> iterable_stack;
  for (char letter = 'a'; letter <= 'z'; letter++)
    iterable_stack.push(letter);
  for (MutantStack<char>::iterator it = iterable_stack.begin(); it != iterable_stack.end(); it++) {
    std::cout << *it;
  }
  std::cout << std::endl;
  return (0);
}

/////

template <typename T>
void print_vector_set(T &c) {
  typedef typename T::iterator iter;

  for (iter it = c.begin(); it != c.end(); it++) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
}

template <typename T>
void print_map(T &c) {
  typedef typename T::iterator iter;

  for (iter it = c.begin(); it != c.end(); it++) {
    std::cout << "\t{" << it->first << ", " << it->second << "}" << std::endl;
  }
}

/////

void test_vector() {
  std::cout << "=============== test_vector ===============" << std::endl;

	ft::vector<int> v1;
	for (int i = 1; i <= 5; i++)
		v1.push_back(i);
	ft::vector<int> v2(3, 42);
	ft::vector<int> v3(v2.begin(), v2.end());
  ft::vector<int> v4(v1);

  std::cout << "- v1: ";
	print_vector_set(v1);
  std::cout << "- v2: ";
	print_vector_set(v2);
  std::cout << "- v3: ";
	print_vector_set(v3);
  std::cout << "- v4: ";
	print_vector_set(v4);

	v1.insert(v1.begin(), 111);
	v1.insert(v1.end() - 2, 999);
	v2.insert(v2.begin(), v1.begin(), v1.end());
	std::cout << "[insert]" << std::endl;
  std::cout << "- v1: ";
	print_vector_set(v1);
  std::cout << "- v2: ";
	print_vector_set(v2);
  std::cout << "- v4: ";
	print_vector_set(v4);

	v3.erase(v3.begin(), v3.end());
	v4.erase(v4.begin());
  std::cout << "[erase]" << std::endl;
  std::cout << "- v3: (size: " << v3.size() << ") ";
	print_vector_set(v3);
  std::cout << "- v4: (size: " << v4.size() << ") ";
	print_vector_set(v4);

  std::cout << "[push_back]" << std::endl;
  v3.push_back(1);
  std::cout << "- v3: ";
	print_vector_set(v3);

  std::cout << "[pop_back]" << std::endl;
	v3.pop_back();
  std::cout << "- v3: ";
	print_vector_set(v3);
}

void test_stack() {
  std::cout << "=============== test_stack ===============" << std::endl;

	ft::stack<int> s1;
	for (int i = 1; i <= 5; i++)
		s1.push(i);

  // std::cout << s1.top();

  std::cout << "- s1: ";
  typedef ft::stack<int>::size_type size_type;
  size_type size = s1.size();
  for (ft::stack<int>::size_type i = 0; i < size; i++) {
    std::cout << s1.top();
    s1.pop();
    if (i != size - 1)
      std::cout << ", ";
  }
  std::cout << std::endl;
}

void test_map() {
  std::cout << "=============== test_map ===============" << std::endl;

  ft::map<std::string, int> m1;
  m1.insert(ft::make_pair("aaa", 11));
  m1.insert(ft::make_pair("bbb", 22));
  m1.insert(ft::make_pair("aaa", 99));
  std::cout << "- m1: " << std::endl;
  print_map(m1);

  std::cout << "[access operator]" << std::endl;
  std::cout << "- m1[\"aaa\"]: " << m1["aaa"] << std::endl;
  std::cout << "- m1[\"ccc\"]: " << m1["ccc"] << std::endl;
  std::cout << "- size: " << m1.size() << std::endl;

  ft::map<std::string, int> m2(m1);
  std::cout << "[copy constructor]" << std::endl;
  std::cout << "- m2: " << std::endl;
  print_map(m2);

  ft::map<std::string, int> m3;
  m3["zzz"] = 999;
  m3["yyy"] = 888;
  m3["xxx"] = 777;
  std::cout << "[swap]" << std::endl;
  m1.swap(m3);
  std::cout << "- m1: " << std::endl;
  print_map(m1);
  std::cout << "- m3: " << std::endl;
  print_map(m3);

  std::cout << "[find]" << std::endl;
  std::cout << "- m3.find(\"aaa\") => " << m3.find("aaa")->second << std::endl;
  
  std::cout << "[erase]" << std::endl;
  m3.erase(m3.begin());
  std::cout << "- m3: " << std::endl;
  print_map(m3);
}

void test_set() {
  std::cout << "=============== test_set ===============" << std::endl;

  ft::set<int> s1;
  s1.insert(1);
  s1.insert(2);
  s1.insert(2);
  s1.insert(2);
  std::cout << "- s1: " << std::endl;
  print_vector_set(s1);

  std::cout << "[copy constructor]" << std::endl;
  ft::set<int> s2(s1);
  std::cout << "- s2: " << std::endl;
  print_vector_set(s2);

  std::cout << "[erase]" << std::endl;
  s2.erase(s2.begin());
  std::cout << "- s2: " << std::endl;
  print_vector_set(s2);

  std::cout << "[swap]" << std::endl;
  s1.swap(s2);
  std::cout << "- s1: " << std::endl;
  print_vector_set(s1);
  std::cout << "- s2: " << std::endl;
  print_vector_set(s2);

  std::cout << "[find]" << std::endl;
  std::cout << "- s1.find(2) => " << *s1.find(2) << std::endl;
  std::cout << "- s1.find(3) => " << *s1.find(3) << std::endl;
}

int main (int argc, char**argv) {
  std::clock_t start = std::clock();

  if (test_subject(argc, argv))
    return 1;
  std::clock_t t1 = std::clock();

  test_vector();
  std::clock_t t2 = std::clock();
  test_stack();
  std::clock_t t3 = std::clock();
  test_map();
  std::clock_t t4 = std::clock();
  test_set();
  std::clock_t t5 = std::clock();

#ifdef FT_STL
  std::cout << "=============== time[STL] ===============" << std::endl;
#else
  std::cout << "=============== time[FT] ===============" << std::endl;
#endif
  std::cout << "- subject: " << t1 - start << std::endl;
  std::cout << "- vector: " << t2 - t1 << std::endl;
  std::cout << "- stack: " << t3 - t2 << std::endl;
  std::cout << "- map: " << t4 - t3 << std::endl;
  std::cout << "- set: " << t5 - t4 << std::endl;

  // std::cout << "=============== leaks ===============" << std::endl;
  // system("leaks ft_containers");
  return 0;
}
