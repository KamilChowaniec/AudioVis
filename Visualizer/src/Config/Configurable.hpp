#pragma once
#include <yaml-cpp/yaml.h>

class YAMLStream;
class Config;

class Configurable {
	friend class YAMLStream;
	friend class Config;
public:
	Configurable() = default;
	virtual ~Configurable() {};
	template<typename T>
	const T& as() const { return dynamic_cast<const T&>(*this); }

protected:
	virtual void show(std::string_view) = 0;
	virtual YAML::Node encode() = 0;
	virtual void decode(const YAML::Node&) = 0;
};