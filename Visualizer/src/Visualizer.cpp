#include "Visualizer.hpp"
#include "Config/Types/Vec.hpp"
#include <glm/gtc/matrix_transform.hpp>

std::shared_ptr<VertexArray> Visualizer::s_FreqVa;
std::shared_ptr<Shader> Visualizer::s_FreqShader;

void Visualizer::init()
{
	s_FreqVa = std::make_shared<VertexArray>();

	float square[] = {
		 -.5f, -.5f,
		 -.5f,  .5f,
		  .5f,  .5f,
		  .5f, -.5f
	};

	float square2[] = {
		 0.f, -.5f,
		 1.f, -.5f,
		 1.f,  .5f,
		 0.f, -.5f,
		 1.f,  .5f,
		 0.f,  .5f
	};

	std::shared_ptr<VertexBuffer> vertexVb = std::make_shared<VertexBuffer>(square, 4 * 2 * sizeof(float));
	vertexVb->setLayout({
		{0, Shader::DataType::vec2, "a_Vertex"},
		});

	std::shared_ptr<VertexBuffer> dimVb = std::make_shared<VertexBuffer>(nullptr, 1024 * sizeof(glm::vec4), GL_STREAM_DRAW);
	dimVb->setLayout({
		{1, Shader::DataType::vec4, "a_Dims"}
		});

	std::shared_ptr<VertexBuffer> colorVb = std::make_shared<VertexBuffer>(nullptr, 1024 * sizeof(glm::vec4), GL_STREAM_DRAW);
	colorVb->setLayout({
		{2, Shader::DataType::vec4, "a_Color"}
		});

	std::shared_ptr<VertexBuffer> rotationVb = std::make_shared<VertexBuffer>(nullptr, 1024 * sizeof(float), GL_STREAM_DRAW);
	rotationVb->setLayout({
		{3, Shader::DataType::float_, "a_Rot"}
		});

	s_FreqVa->addVertexBuffer(vertexVb);
	s_FreqVa->addVertexBuffer(dimVb);
	s_FreqVa->addVertexBuffer(colorVb);
	s_FreqVa->addVertexBuffer(rotationVb);

	s_FreqVa->setDivisor({
		{0, 0},
		{1, 1},
		{2, 1},
		{3, 1}
		});

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	std::shared_ptr<IndexBuffer> ib = std::make_shared<IndexBuffer>(indices, 6);
	s_FreqVa->setIndexBuffer(ib);

	s_FreqShader = std::make_shared<Shader>("res/shaders/freq.vert", "res/shaders/freq.frag");
}

void Visualizer::setAspectRatio(glm::ivec2 dims)
{
	s_FreqShader->bind();
	s_FreqShader->setUniform("u_ViewProjection", glm::ortho(-dims.x / 1000.f, dims.x / 1000.f, -dims.y / 1000.f, dims.y / 1000.f));
}

Visualizer::Visualizer(const Config& cfg)
	: m_Cfg(cfg), m_SmoothFft(1024, 0), m_SpinAngle(0), m_FreqCount(1024),
	m_FreqColor(1024), m_FreqDims(1024), m_FreqRot(1024)
{ }

void Visualizer::update(const std::vector<float>& fft)
{
	const glm::vec2& visibleFreq = m_Cfg["visibleFreq"].as<Vec2<float>>();
	const int startFreq = visibleFreq.x * fft.size();
	const int stopFreq = visibleFreq.y * fft.size();
	m_FreqCount = stopFreq - startFreq;

	for (int i = startFreq; i < stopFreq; ++i) {
		float& val = m_SmoothFft[i] = 0.5f * m_SmoothFft[i] + ((1 - 0.5f) * fft[i]);
		m_FreqDims[i] = { ((i - startFreq) / (float)m_FreqCount) * 2 - 1, 0, 1. / m_FreqCount, std::sqrt(val) * 3 };
		m_FreqColor[i] = m_Cfg["lineColor"].as<Color<float>>();
		m_FreqRot[i] = 0;
	}

	if (m_FreqCount > 0) {
		s_FreqVa->bind();
		s_FreqVa->getVertexBuffer(1)->updateData(&m_FreqDims[startFreq], m_FreqCount * sizeof(glm::vec4));
		s_FreqVa->getVertexBuffer(2)->updateData(&m_FreqColor[startFreq], m_FreqCount * sizeof(glm::vec4));
		s_FreqVa->getVertexBuffer(3)->updateData(&m_FreqRot[startFreq], m_FreqCount * sizeof(float));
	}
}

void Visualizer::show()
{
	if(m_FreqCount > 0)
		Renderer::drawInstanced(s_FreqVa, s_FreqShader, m_FreqCount);
}