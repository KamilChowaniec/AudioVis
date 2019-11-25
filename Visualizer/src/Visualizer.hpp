#pragma once
#include "Renderer.hpp"
#include "Config/Config.hpp"

class Visualizer {
public:
	static void init();
	static void setAspectRatio(glm::ivec2 dims);

	Visualizer(const Config& cfg);

	void update(const std::vector<float>& fft);
	void show();

private:
	std::vector<float> m_SmoothFft;
	std::vector<glm::vec4> m_FreqColor;
	std::vector<glm::vec4> m_FreqDims;
	std::vector<float> m_FreqRot;
	const Config& m_Cfg;
	float m_SpinAngle;

	static std::shared_ptr<VertexArray> s_FreqVa;
	static std::shared_ptr<Shader> s_FreqShader;
};