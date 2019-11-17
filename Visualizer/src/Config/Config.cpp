#include "Config.hpp"
#include "Color.hpp"
#include "Logger.hpp"

Config::Config()
{
	resetToDefault();
}

void Config::resetToDefault()
{
	set("lineColor", Color(1, 1, 1, 1));
}

void Config::show() 
{
	ImGui::Begin("Config");
	m_Data["lineColor"]->show("lineColor");
	ImGui::End();
}

const Configurable& Config::operator[](std::string_view name)
{
	return *(*m_Data.find(name.data())).second;
}

void Config::loadFromFile(std::string_view filePath)
{
	m_YamlStream.open(filePath, YAMLStream::in);
	if (m_YamlStream.isOpen()) 
		for (auto& [name, con] : m_Data) con->decode(m_YamlStream[name]);
	m_YamlStream.close();
}

void Config::saveToFile(std::string_view filePath)
{
	m_YamlStream.open(filePath, YAMLStream::out);
	if (m_YamlStream.isOpen())
		for (auto& entry: m_Data) m_YamlStream << entry;
	m_YamlStream.close();
}