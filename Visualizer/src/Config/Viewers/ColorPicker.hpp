#pragma once
#include "Viewer.hpp"
#include "imgui.h"
#include "Logger.hpp"

template<size_t N, typename T>
class ColorPicker;

template<typename T>
using ColorPicker4 = ColorPicker<4, T>;

template<typename T>
using ColorPicker3 = ColorPicker<3, T>;

template<size_t N, typename T>
class ColorPicker : public Viewer<T> {
public:
	inline void show(std::string_view name, T* first) override {
		if constexpr (N == 3)
			ImGui::ColorEdit3(name.data(), first);
		else if constexpr (N == 4)
			ImGui::ColorEdit4(name.data(), first);
		else ASSERT(false, "ColorPicker can have only 3 or 4 elements!");
	}
};