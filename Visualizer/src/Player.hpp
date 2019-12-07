#pragma once
#include <glm/glm.hpp>
#include "Window.hpp"
#include "Audio/Audio.hpp"
#include "Interaction/Events.hpp"

class Player {
public:
	Player(Audio& audio, glm::vec4 dims);

	void onUpdate();

	void onEvent(Event& e);
	void renderImGui();

	void hide();
	void show();

	void setDims(glm::vec4 dims);
	void setAudio(Audio& audio);

	void audioChanged();
private:
	void onMousePressEvent(MousePressEvent& e);

	glm::vec4 m_Dims;
	Audio& m_Audio;

	float m_Volume;
	float m_Progress;

	bool m_ShouldUpdatePosition;
};