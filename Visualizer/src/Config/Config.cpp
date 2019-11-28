#include "Config.hpp"
#include "Types/Vec.hpp"
#include "Viewers/ColorPicker.hpp"
#include "Viewers/Slider.hpp"
#include "Logger.hpp"

Config::Config()
{
	resetToDefault();
}

void Config::resetToDefault()
{
	setEntry("lineColor", Color<float, ColorPicker4>({ 1, 1, 1, 1 }));
	setEntry("visibleFreq", Vec2<float, Slider2>({ 0, 1 }, {0, 1}));
	setEntry("octaves", Number<int, Slider1>(50, { 3, 100 }));
}

void Config::show() 
{
	ImGui::Begin("Config");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	showEntry("lineColor");
	showEntry("visibleFreq");
	showEntry("octaves");
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
	auto it = m_Data.find(name.data());
	ASSERT(it != m_Data.end(), std::string("Unknown entry [") + name.data() + "]!");
	(*it).second->show(name);
}