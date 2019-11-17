#include "Application.hpp"

Application::Application()
	: ThreadedApp("Audio Visualizer", 1280, 720)
{
	Audio::init();
	Renderer::setClearColor({ 0,0,0,1 });
}

void Application::callbackSetup()
{
}

void Application::onUpdate()
{
}

void Application::onRender()
{
	Renderer::clear();
}

void Application::onImGuiRender()
{
	m_Cfg.show();
}