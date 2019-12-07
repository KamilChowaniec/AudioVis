#include "Config.hpp"
#include "Types/Vec.hpp"
#include "Viewers/ColorPicker.hpp"
#include "Viewers/Slider.hpp"
#include "Logger.hpp"
#include "FileDialog.hpp"

Config::Config()
{
	resetToDefault();
}

void Config::resetToDefault()
{
	setEntry("lineColor", Color<float, ColorPicker4>({ 1, 1, 1, 1 }));
	setEntry("visibleFreq", Vec2<float, Slider2>({ 0, 1 }, {0, 1}));
	setEntry("count", Number<size_t, Slider1>(100L, { 3L, 500L }));
	setEntry("bend", Number<float, Slider1>(0, { 0, 360 }));
	setEntry("scale", Number<float, Slider1>(1, { 0, 2 }));
	setEntry("rotation", Number<float, Slider1>(0, { 0, 360 }));
	setEntry("centerX", Number<float, Slider1>(0, { -2, 2 }));
	setEntry("centerY", Number<float, Slider1>(0, { -2, 2 }));
	setEntry("width", Number<float, Slider1>(2, { 0, 4 }));
	setEntry("lineInterspace", Number<float, Slider1>(0.25, { 0, 1 }));
	setEntry("smoothness", Number<float, Slider1>(0.5, { 0, 1 }));
	setEntry("innerIntensity", Number<float, Slider1>(1, { 0, 2 }));
	setEntry("outerIntensity", Number<float, Slider1>(1, { 0, 2 }));
}

void Config::renderImGui() 
{
	if (ImGui::Button("Save")) {
		std::string filePath = FileDialog::saveFile("Save Config", "config.yaml", { "*.yaml" });
		if(filePath.length() != 0)
			saveToFile(filePath);
	}
	ImGui::SameLine();
	if (ImGui::Button("Load")) {
		std::string filePath = FileDialog::chooseFile("Choose Config", { "*.yaml" });
		if (filePath.length() != 0)
			loadFromFile(filePath);
	}
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	showEntry("lineColor");
	showEntry("visibleFreq");
	showEntry("count");
	showEntry("bend");
	showEntry("scale");
	showEntry("rotation");
	showEntry("centerX");
	showEntry("centerY");
	showEntry("width");
	showEntry("lineInterspace");
	showEntry("smoothness");
	showEntry("innerIntensity");
	showEntry("outerIntensity");
}

const Configurable& Config::operator[](std::string_view name) const
{
	auto it = m_Data.find(name.data());
	ASSERT(it != m_Data.end(), std::string("No entry [") + name.data() + "]!");
	return *(*it).second;
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