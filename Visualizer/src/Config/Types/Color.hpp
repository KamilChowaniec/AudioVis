#pragma once
#pragma once
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Config/Configurable.hpp"
#include "Logger.hpp"

class Color : public Configurable, public glm::vec4 {
public:
	Color(float r, float g, float b, float a);

private:
	void show(std::string_view name) override;
	YAML::Node encode() override;
	void decode(const YAML::Node& node) override;
};