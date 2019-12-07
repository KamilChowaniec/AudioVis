#include "Player.hpp"
#include <imgui.h>
#include "Logger.hpp"

Player::Player(Audio& audio, glm::vec4 dims)
	: m_Audio(audio), m_Dims(dims), m_Progress(0), m_Volume(0.5), m_ShouldUpdatePosition(true)
{ }

void Player::onUpdate()
{
	if (m_ShouldUpdatePosition)
		m_Progress = m_Audio.getPosition();
}

void Player::onEvent(Event& e)
{
	e.dispatch<MousePressEvent>([this](MousePressEvent& e) { onMousePressEvent(e); });
}

void Player::onMousePressEvent(MousePressEvent& e)
{
	LOG_TRACE("YES");
	if (e.button == 0)
		if (m_Audio.isPlaying()) m_Audio.pause();
		else m_Audio.play();
}

void Player::renderImGui()
{
	ImGui::SetNextWindowPos({ m_Dims.x, m_Dims.y });
	ImGui::SetNextWindowSize({ m_Dims.z, m_Dims.w });
	ImGui::Begin("Player", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.7);
	if (ImGui::SliderFloat("##Progress", &m_Progress, 0, 1)) m_ShouldUpdatePosition = false;
	if (ImGui::IsItemDeactivatedAfterEdit()) {
		m_Audio.setPosition(m_Progress);
		m_ShouldUpdatePosition = true;
	}


	ImGui::SameLine();
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.28);
	if (ImGui::SliderFloat("##Volume", &m_Volume, 0, 1))
		m_Audio.setVolume(m_Volume);

	ImGui::End();
}

void Player::hide()
{
}

void Player::show()
{
}

void Player::setDims(glm::vec4 dims)
{
	m_Dims = dims;
}

void Player::setAudio(Audio& audio)
{
	m_Audio = audio;
}

void Player::audioChanged()
{
	m_Progress = 0;
	m_Volume = m_Audio.getVolume();
}

