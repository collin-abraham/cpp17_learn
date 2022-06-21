/* main.cpp
* 2022-06-20
* Collin Abraham
* 
* This program demonstrates some of the main changes to the C++ standard in 2017
*/

#include <iostream>
#include <memory>
#include <algorithm>
#include <vector>
#include <string_view>
#include <array> 
#include <tuple>

using namespace std;

/* helper function to generate a vector of size sizeVal with random numbers ranging from 0 - rangeVal*/
auto generate_vector(int sizeVal, int rangeVal) {
	vector<int> vecA(sizeVal);
	generate(begin(vecA), end(vecA), [rangeVal]() { return rand() % rangeVal; });
	return vecA;
}

/* helper function to print the contents of a vector, in a line */
template <typename VEC_TYPE>
void print_vec(const vector<VEC_TYPE>& vec) {
	for_each(begin(vec), end(vec), [](auto x) {cout << x << " "; });
}

/* cpp17 - copy/move elision is guaranteed 
*
* This function guarantees the pointer returned will not be copied or moved
* The object returned will be created emplace 
* For large, complex programs the value of resources saved is unimaginably huge if
* millions of unique_pointer objects need to be created/managaed
*/
template<typename T>
auto generate_pointer() {
	return make_unique<T>();
}

/* cpp17 - constexpr support for the standard library functions, in this case max_element
* 
* This function creates two iterators directly instantiated to point to the result of max_element on both vectors
* max_element, under the hood, is using constexpr to generate the forward_iterators (explained thoroughly in the standard)
*/
template <typename VEC_TYPE>
void compare_values(vector<VEC_TYPE>& vecA, vector<VEC_TYPE>& vecB) {
	const vector<VEC_TYPE>::iterator vecAIterator = max_element(begin(vecA), end(vecA));
	const vector<VEC_TYPE>::iterator vecBIterator = max_element(begin(vecB), end(vecB));

	cout << endl << "Max element of Vector A: " << *vecAIterator << " at element: " << distance(begin(vecA), vecAIterator) << endl;
	cout << endl << "Max element of Vector B: " << *vecBIterator << " at element: " << distance(begin(vecB), vecBIterator) << endl;
}

/* cpp 17 - constexpr lambda support, unavailable in cpp14
* 
* This function creates a constexpr lambda which will take each someVal and return it multiplied by 5
* The contents are shown, then the lambda is used within std::transform, then contents are shown again 
*/
template <typename VEC_TYPE>
void generate_constexpr_lambda(vector<VEC_TYPE>& vec) {
	constexpr auto multiplyByFive = [](auto someVal) { return someVal * 5; };

	cout << "\nValues in vector before transforming: \n";
	print_vec(vec);

	transform(begin(vec), end(vec), begin(vec), multiplyByFive);

	cout << "\nValues in vector after transforming: \n";
	print_vec(vec);
	
	cout << endl;
}

constexpr auto generate_constexpr_lambda (int i) {
	constexpr auto multiplyByFive = [](auto someVal) { return someVal * 5; };
	return multiplyByFive;
}

/* cpp17 - string_view .. A non-allocating view into a string. Useful for apis so that lots of copy/moves don't need to be made to 
* analyze the contents of a string
* If you can be sure you're only going to ever need to view the contents and won't need to move it around.. very useful
* 
* This function creates a stringview of an array with 3 arbirary strings within, then prints them out to the screen 
*/
void stringview_func() {
	constexpr string_view contents[] {"Some words","More words","Another string!"};
	
	cout << "\nString View tester: \n";
	for (int i = 0; i < 3; ++i)
		cout << endl << contents[i];
}

/* cpp17 - templated class argument deduction. Standard library containers can automatically decide class arguments based on
* values given by the user. A simple use case is using std::array 
* 
* This function constructs a std::array the old way, where <type,size> needed to be given, 
* and then generates a new cpp17 style where it is automatically chosen.
* It has its obvious dangers and pitfalls, but that is the case with all new cpp functionalities. With great power comes
* great responsbilitiy. 
*/
void build_arrays() {
	array <string, 2> stringArray{ "Jimmy", "Sally" }; // previously had to state size of 2 

	array anotherStringArray{ "Jimmy", "Sally" }; // cpp17 compiler realizes the type is string and size is 2 automatically
}

/* cpp17 - fold expressions allow a group of parameters to be used with some kind of binary operator
* Useful when you have comparisons/operators + - * / % ^ == etc. 
* 
* This function takes in a variadic parameter list and sums all of the inputs together, returns as auto 
*/
template <typename ... T>
auto sum_numbers(const T& ... values) {
	return (values + ...);
}

/* cpp17 - structured bindings allow you to access values in an organized way from a map/pair/struct etc
* An alias to an existing object .. does not have to be a reference-type either 
* 
* This function creates a tuple, binds the objects within it, and creates a new tuple from the structured binding 

*/
void  bindings_example() {
	tuple <string, int, double> theTuple{ "Sally", 21, 55'000 };
	const auto& [first, second, third] = theTuple;

	tuple<string, int, double> newTuple{ first,second,third };
}

/* cpp17 - structured bindings also allow binding to data members of classes
* 
* a simple struct is created, a function to instantiate one with 5 and C as its members
* then a structured binding is declared and used via calling createObj() 
*/

struct someObj {
	int memA;
	char memB;
};

someObj createObj() { 
	return someObj{ 5,'C' }; 
}


void  bindings_example_2() {
	const auto [first, second] = createObj();
	cout << "\nClass contains: " << endl; 
	cout << first << " " << second << endl;
}

/* cpp17- if init expressions allows us to initialize variables witin an if statement to do work 
* We can use structured bindings as an example
* 
* This function calls the structured binding example from above, does a conditional on it and processes the result 
*/
void if_init() {
	cout << endl; 

	if (auto const [first, second] = createObj(); first != 6)
		cout << "Value was not 6" << endl;
	else
		cout << "Value was 6" << endl;
	
}



/* cpp17 - if constexpr does a bool check that the result is a constexpr true/false value
* 
* This function checks if the parameter is a pointer .. if so it returns a derefenced value
* otherwise it returns the pointer 
*/
template<typename T>
auto if_constexpr_func(T ptrCheck) {
	if constexpr (is_pointer_v<T>) {
		cout << "\nis a pointer!\n";
		return *ptrCheck;
	}
	else {
		cout << "\nis not a pointer!\n";
		return ptrCheck; 
	}
}

/* cpp17 - nested namespaces .. as the name implies, namespaces can be contained within other namespaces 
* 
* This follow code contains 2 names spaces.. within them a function that returns a string, then an identical function within a nested namespace 
* Then both versions are called in printFoos()
*/

namespace A {
	namespace B {
		string foo() {
			return "I am a B foo!";
		}
	}

	string foo() {
		return "I am an A foo!";
	}
}

void printFoos() {
	cout << endl << A::foo() << endl; 
	cout << endl << A::B::foo() << endl; 
}


int main() {
	auto newPtr = generate_pointer<double>();

	auto testVectorA = generate_vector(10, 100);
	auto testVectorB = generate_vector(10, 100);
	compare_values(testVectorA, testVectorB);

	generate_constexpr_lambda(testVectorA);

	stringview_func();

	build_arrays();

	cout << endl << sum_numbers(4, 6, 33, 7, 3, 4, 5) << endl;

	bindings_example();

	bindings_example_2();

	if_init();

	auto ptr = new int*;
	auto returnedValue = if_constexpr_func(ptr);
	delete ptr; 

	printFoos();
	
}