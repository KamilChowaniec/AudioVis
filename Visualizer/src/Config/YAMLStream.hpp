#pragma once

#include <fstream>
#include <tuple>
#include <memory>
#include <yaml-cpp/yaml.h>
#include "Configurable.hpp"

class YAMLStream {
public:
	enum Type{
		in, out
	};

	YAMLStream();
	YAMLStream(std::string_view filepath, Type flag);

	void open(std::string_view filePath, Type flag);
	void close();
	bool isOpen();

	YAMLStream& operator<<(const std::pair<std::string, std::unique_ptr<Configurable>>& node);
	YAML::Node operator[](std::string_view name);
private:
	bool m_IsOpen;
	YAML::Node m_Node;
	std::unique_ptr<YAML::Emitter> m_Emitter;
	std::ofstream m_OutFile;
};

