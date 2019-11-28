#pragma once
#include <yaml-cpp/yaml.h>

class Configurable {
	friend class YAMLStream;
	friend class Config;
public:
	virtual ~Configurable() {};

	template<typename T>
	const T& as() const { return reinterpret_cast<const T&>(*this); }

protected:
	virtual void show(std::string_view) = 0;
	virtual YAML::Node encode() = 0;
	virtual void decode(const YAML::Node&) = 0;
};