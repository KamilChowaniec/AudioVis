#pragma once
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Configurable.hpp"
#include "Logger.hpp"

class Color : public Configurable, public glm::vec4 {
public:
	Color(float r, float g, float b, float a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	void show(std::string_view name) override {
		ImGui::ColorPicker4(name.data(), glm::value_ptr<float>(*this));
	}

	YAML::Node encode() override {
		YAML::Node node;
		for (int i = 0; i < 4; ++i) node.push_back((*this)[i]);
		return node;
	}

	void decode(const YAML::Node& node) override {
		if (!node.IsSequence() || node.size() != 4) {
			LOG_ERROR("Wrong type to decode!");
		}
		for (int i = 0; i < 4; ++i) (*this)[i] = node[i].as<float>();
	}
};