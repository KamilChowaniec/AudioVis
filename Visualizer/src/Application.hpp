#pragma once
#include "App.hpp"
#include "Audio/Audio.hpp"
#include "Config/Config.hpp"
#include "Player.hpp"
#include "Visualizer.hpp"

class Application : public ThreadedApp {
public:
	Application();

private:
	void callbackSetup();

	void onEvent(Event& e) override;
	void onUpdate() override;
	void onRender() override;
	void onImGuiRender() override;

	Player m_Player;
	Audio m_Audio;
	Config m_Cfg;
	Visualizer m_Vis;
};