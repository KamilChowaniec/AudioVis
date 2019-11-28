#pragma once
#include "Viewer.hpp"

template<typename T>
class NullViewer : public Viewer<T>{
public:
	inline void show(std::string_view name, T* first) override {

	}
};