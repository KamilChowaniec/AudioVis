#pragma once
#include <memory>
#include <tsl/ordered_map.h>
#include "Types/Configurable.hpp"
#include "YAMLStream.hpp"

class Config {
public:
	Config();

	void resetToDefault();
	void show();

	void loadFromFile(std::string_view filePath);
	void saveToFile(std::string_view filePath);

	const Configurable& operator[](std::string_view name) const;
private:
	tsl::ordered_map<std::string, std::unique_ptr<Configurable>> m_Data;
	YAMLStream m_YamlStream;

	template<typename T>
	void setEntry(std::string_view name, T&& val) {
		if (auto it = m_Data.find(name.data()); it != m_Data.end())
			* ((*it).second) = std::move(val);
		else m_Data[name.data()] = std::make_unique<T>(val);
	}

	void showEntry(std::string_view name) const;
};