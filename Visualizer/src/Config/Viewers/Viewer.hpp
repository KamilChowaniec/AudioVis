#pragma once
#include <string>

template<typename T>
class Viewer {
public:
	virtual inline void show(std::string_view name, T* first) = 0;
};