#include "Config.hpp"
#include "Types/Color.hpp"
#include "Logger.hpp"

Config::Config()
{
	resetToDefault();
}

void Config::resetToDefault()
{
	setEntry("lineColor", Color(1, 1, 1, 1));
	setEntry("edgeColor", Color(1, 1, 1, 1));
}

void Config::show() 
{
	ImGui::Begin("Config");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	showEntry("lineColor");

	if (ImGui::Button("Save")) {
		saveToFile("res/config/default.yaml");
	}

	ImGui::End();
}

const Configurable& Config::operator[](std::string_view name) const
{
	return *(*m_Data.find(name.data())).second;
}

void Config::loadFromFile(std::string_view filePath)
{
	m_YamlStream.open(filePath, YAMLStream::in);
	if (m_YamlStream.isOpen())
		for (auto& [name, con] : m_Data) { 
			if(m_YamlStream[name])
				con->decode(m_YamlStream[name]);
			else {
				LOG_WARN("Missing \"{0}\" entry in \"{1}\" config file!", name, filePath);
			}
		}
	m_YamlStream.close();
}

void Config::saveToFile(std::string_view filePath)
{
	m_YamlStream.open(filePath, YAMLStream::out);
	if (m_YamlStream.isOpen())
		for (auto& entry: m_Data) m_YamlStream << entry;
	m_YamlStream.close();
}

void Config::showEntry(std::string_view name) const
{
	(*m_Data.find(name.data())).second->show(name);
}