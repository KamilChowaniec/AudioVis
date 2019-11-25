#pragma once
#include "Color.hpp"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Logger.hpp"

Color::Color(float r, float g, float b, float a)
	: glm::vec4{ r,g,b,a }
{ }

void Color::show(std::string_view name) {
	ImGui::ColorPicker4(name.data(), glm::value_ptr<float>(*this));
}

YAML::Node Color::encode() {
	YAML::Node node;
	for (int i = 0; i < 4; ++i) node.push_back((*this)[i]);
	return node;
}

void Color::decode(const YAML::Node& node) {
	if (!node.IsSequence() || node.size() != 4) {
		LOG_ERROR("Wrong type to decode!");
	}
	for (int i = 0; i < 4; ++i) (*this)[i] = node[i].as<float>();
}
