#ifndef REFLECTION
#define REFLECTION
#include <string>
#include <map>
using namespace std;
#define VAR(var) vars[#var]=make_pair((size_t)(((char*)(&var))-(size_t)this)-4, typeid(var).name())
#define REFLECTION inline static map<string, pair<size_t, string> > vars;\
	template<typename T>\
    T& get(string name)\
	{\
		if(vars[name].second!=typeid(T).name())throw exception();\
		return (*((T*)((size_t)(this)+vars[name].first)));\
	}\
	template<typename T>\
    void set(string name, T val)\
	{\
		 (*((T*)((size_t)(this)+vars[name].first))) = val;\
	}
    #endif