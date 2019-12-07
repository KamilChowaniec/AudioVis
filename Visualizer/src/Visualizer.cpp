#include "Visualizer.hpp"
#include "Config/Types/Vec.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

static const float PI = glm::pi<float>();
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

	std::shared_ptr<VertexBuffer> vertexVb = std::make_shared<VertexBuffer>(square, 4 * 2 * sizeof(float), GL_STATIC_DRAW);
	vertexVb->setLayout({{0, Shader::DataType::vec2, "a_Vertex"}});

	std::shared_ptr<VertexBuffer> dimVb = std::make_shared<VertexBuffer>(nullptr, 1024 * sizeof(glm::vec4), GL_STREAM_DRAW);
	dimVb->setLayout({{1, Shader::DataType::vec4, "a_Dims"}});

	std::shared_ptr<VertexBuffer> colorVb = std::make_shared<VertexBuffer>(nullptr, 1024 * sizeof(glm::vec4), GL_STREAM_DRAW);
	colorVb->setLayout({{2, Shader::DataType::vec4, "a_Color"}});

	std::shared_ptr<VertexBuffer> rotationVb = std::make_shared<VertexBuffer>(nullptr, 1024 * sizeof(float), GL_STREAM_DRAW);
	rotationVb->setLayout({{3, Shader::DataType::float_, "a_Rot"}});

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
	//dims = { 1000, 1000 };
	s_FreqShader->bind();
	s_FreqShader->setUniform("u_ViewProjection", glm::ortho(-dims.x / 1000.f, dims.x / 1000.f, -dims.y / 1000.f, dims.y / 1000.f));
}

Visualizer::Visualizer(const Config& cfg)
	: m_Cfg(cfg), m_SmoothFft(1024, 0), m_SpinAngle(0), m_FreqCount(1024),
	m_FreqColor(1024), m_FreqDims(1024), m_FreqRot(1024), m_Spectrum(0,1,3)
{ }

//Visualizer::logAverages(const std::vector<float>& fft, const unsigned int octaveBands) {
//
//}

void Visualizer::update(const std::vector<float>& fft)
{
	// Config variables
	const glm::vec4& lineColor = m_Cfg["lineColor"].as<Color<float>>();
	const glm::vec2& visibleFreq = m_Cfg["visibleFreq"].as<Vec2<float>>();
	const glm::vec2 center = { m_Cfg["centerX"].as<Number<float>>(), m_Cfg["centerY"].as<Number<float>>() };
	const float scale = m_Cfg["scale"].as<Number<float>>();
	const float width = m_Cfg["width"].as<Number<float>>();
	const float smooth = m_Cfg["smoothness"].as<Number<float>>();
	const float slope = m_Cfg["bend"].as<Number<float>>().x / 360.0f;
	const float lineInterspace = m_Cfg["lineInterspace"].as<Number<float>>();
	const float rotation = glm::radians<float>(m_Cfg["rotation"].as<Number<float>>().x);
	const float innerIntensity = m_Cfg["innerIntensity"].as<Number<float>>();
	const float outerIntensity = m_Cfg["outerIntensity"].as<Number<float>>();
	const size_t count = m_Cfg["count"].as<Number<size_t>>();

	m_Spectrum.setStartFreq(visibleFreq.x);
	m_Spectrum.setEndFreq(visibleFreq.y);
	m_Spectrum.setOctaveBandCount(count);
	m_Spectrum.calculate(fft);

	const int startFreq = 0;
	const int stopFreq = m_Spectrum.size();
	m_FreqCount = count;

	const float rotationCos = cos(rotation);
	const float rotationSin = sin(rotation);

	const double arcAngle = 2 * PI * std::max<double>(slope, 0.001);
	const double arcAngleInc = arcAngle / (m_FreqCount - (1 - slope));
	const float arcRad = width / arcAngle;

	double currArcAngle = (3. * PI - arcAngle) / 2.;

	if (m_FreqCount > m_FreqDims.size()) {
		m_SmoothFft.resize(m_FreqCount);
		m_FreqDims.resize(m_FreqCount);
		m_FreqColor.resize(m_FreqCount);
		m_FreqRot.resize(m_FreqCount);
	}

	for (int i = startFreq; i < stopFreq; ++i, currArcAngle += arcAngleInc) {

		// Smoothing out values over time
		float& val = m_SmoothFft[i] = smooth * m_SmoothFft[i] + ((1. - smooth) * m_Spectrum[i]);

		float cosinus = cos(currArcAngle);
		float sinus = sin(currArcAngle);

		const float lineHeight = std::sqrt(val);

		// Distance from [0,0] to center of the quad
		double r = arcRad + lineHeight * (outerIntensity - innerIntensity) / 2.;

		const float arcX = cosinus * r;

		// [ quad y ]  [   keeping at one place   ]   [ changing center based on slope ]
		// sinus * r + (arcRad - width / (2. * PI)) + (width / (2. * PI)) * (1. - slope);
		const float arcY = sinus * r + arcRad - width * slope / (2. * PI);

		// Rotating visualizer
		const float rotX = rotationCos * arcX - rotationSin * arcY;
		const float rotY = rotationSin * arcX + rotationCos * arcY;

		const int freqIndex = i - startFreq;

		const float lineWidth = (width * (1. - lineInterspace)) / (m_FreqCount - (1 - slope));

		m_FreqDims[freqIndex] = {
			rotX * scale + center.x,
			rotY * scale + center.y,
			lineWidth * scale,
			(innerIntensity + outerIntensity) * lineHeight * scale
		};

		m_FreqColor[freqIndex] = lineColor;


		m_FreqRot[freqIndex] = (slope != 0.0 ? (3. * PI / 2. - currArcAngle) : 0) - rotation;
	}

	if (m_FreqCount > 0)
		updateBuffers();
}

void Visualizer::updateBuffers() {
	s_FreqVa->bind();
	s_FreqVa->getVertexBuffer(1)->updateData(m_FreqDims.data(), m_FreqCount * sizeof(glm::vec4));
	s_FreqVa->getVertexBuffer(2)->updateData(m_FreqColor.data(), m_FreqCount * sizeof(glm::vec4));
	s_FreqVa->getVertexBuffer(3)->updateData(m_FreqRot.data(), m_FreqCount * sizeof(float));
}

void Visualizer::render()
{
	//m_FreqDims[0] = { 0, 0, 1, 1 };
	//updateBuffers();
	//Renderer::draw(s_FreqVa, s_FreqShader);

	if (m_FreqCount > 0)
		Renderer::drawInstanced(s_FreqVa, s_FreqShader, m_FreqCount);
}