#include "Application.hpp"
#include "imgui.h"
#include "Config/FileDialog.hpp"

Application::Application()
	: ThreadedApp("Audio Visualizer", 1280, 720), m_Vis(m_Cfg)
{
	Audio::init();
	Visualizer::init();
	Visualizer::setAspectRatio(m_Window.getWindowSize());
	Renderer::setClearColor({ 0,0,0,1 });
	m_Audio.loadFromFile("res/audio/Running Away.mp3");
	m_Audio.play();
	m_Cfg.loadFromFile("res/config/default.yaml");
	m_Window.setVSync(false);

	callbackSetup();
}

void Application::callbackSetup()
{
	using namespace std::chrono_literals;
	m_Timer.addTask(25ms, [this]() {m_Vis.update(m_Audio.getFFT()); }, true);
}

void Application::onUpdate()
{
}

void Application::onRender()
{
	Renderer::clear();
	m_Vis.show();
}

void Application::onImGuiRender()
{
	ImGui::Begin("Config");
	if (ImGui::Button("Change audio")) {
		std::string filePath = FileDialog::chooseFile("Choose Audio", Audio::s_AvailablePatterns);
		if (filePath.length() != 0) {
			m_Audio.loadFromFile(filePath);
			m_Audio.play();
		}

	}
	m_Cfg.show();
	ImGui::End();
}