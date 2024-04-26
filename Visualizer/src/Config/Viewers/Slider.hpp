#pragma once
#include <string>
#include "Viewer.hpp"
#include "imgui.h"
#include "Logger.hpp"

template<size_t N, typename T>
class Slider;

template<typename T>
using Slider1 = Slider<1, T>;

template<typename T>
using Slider2 = Slider<2, T>;

template<size_t N, typename T>
class Slider : public Viewer<T> {
public:
	Slider(T min, T max)
		: m_Min{ min }, m_Max{ max }
	{ }

	inline void show(std::string_view name, T* first) {
		// if constexpr (N == 1)
		// 	ImGui::SmoothSlider<T>(name.data(), first, m_Min, m_Max);
		// else if constexpr (N == 2)
		// 	ImGui::RangeSlider<T>(name.data(), first, m_Min, m_Max);
		// else ASSERT(false, "Slider can have only 1 or 2 elements!");
	}
private:
	T m_Min, m_Max;
};