#pragma once
#include "App.hpp"
#include "Audio/Audio.hpp"
#include "Config/Config.hpp"
#include "Visualizer.hpp"

class Application : public ThreadedApp {
public:
	Application();

private:
	void callbackSetup();

	void onUpdate() override;
	void onRender() override;
	void onImGuiRender() override;

	Audio m_Audio;
	Config m_Cfg;
	Visualizer m_Vis;
};