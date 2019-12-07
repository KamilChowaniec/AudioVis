#include "Spectrum.hpp"
#include "Audio.hpp"
#include <algorithm>

#undef min
#undef max

Spectrum::OctaveBand::OctaveBand(float min, float centr, float max) 
	: min(min), max(max), centr(centr)
{}


Spectrum::Spectrum(float startFreq, float endFreq, unsigned int count)
	: m_StartFreq(startFreq), m_EndFreq(endFreq)
{
	setOctaveBandCount(count);
}

void Spectrum::calculate(const std::vector<float>& fft)
{
	for (int i{ 0 }; i < m_OctaveBands.size(); ++i) {
		const auto& band = m_OctaveBands[i];
		float minIdx = fft.size() * band.min / Audio::s_SamplingRate;
		float maxIdx = fft.size() * band.max / Audio::s_SamplingRate;
		float centrIdx = fft.size() * band.centr / Audio::s_SamplingRate;

		float sum = 0;
		float weights = 0;
		if (floor(minIdx) == floor(maxIdx)) {
			float weightLeft = 1.f - (centrIdx - int(centrIdx));
			float weightRight = (centrIdx - int(centrIdx));
			sum = weightLeft * fft[int(centrIdx)] + weightRight * fft[int(ceil(centrIdx))];
			weights = 1;
		}
		else {
			int lastIdx = std::min<int>(fft.size() - 1, int(ceil(maxIdx)));
			for (int j{ int(minIdx) }; j < lastIdx; ++j) {
				if (j == int(minIdx)) {
					float weight = 1.f - (minIdx - int(minIdx));
					float value = fft[int(minIdx)];
					sum += weight * value ;
					weights += weight;

				}
				else if (j == int(ceil(maxIdx - 1))) {
					float weight = (maxIdx - int(maxIdx));
					float value = fft[int(ceil(maxIdx))];
					sum += weight * value * value;
					weights += weight;
				}
				else {
					float value = fft[j];
					sum += value ;
					weights += 1;
				}
			}
		}

		m_Amplitudes[i] = sum / weights;
	}
}

void Spectrum::setOctaveBandCount(unsigned int count)
{
	if (m_OctaveBandCount != count) {
		m_OctaveBandCount = count;
		calculateOctaveBands();
	}
}

void Spectrum::setStartFreq(float startFreq)
{
	if (m_StartFreq != startFreq) {
		m_StartFreq = startFreq;
		calculateOctaveBands();
	}
}

void Spectrum::setEndFreq(float endFreq)
{
	if (m_EndFreq != endFreq) {
		m_EndFreq = endFreq;
		calculateOctaveBands();
	}
	
}

void Spectrum::calculateOctaveBands() {
	m_OctaveBands.clear();

	/*float centerInc = pow(2.0f, 1.0f / m_OctaveBandCount);
	float sideInc = sqrt(centerInc);
	float center = 15.625f;
	float min = center / sideInc, max = center * sideInc;
	m_OctaveBands.emplace_back(min, center, max);

	while (max < Audio::s_SamplingRate / 2.f) {
		center *= centerInc;
		min *= centerInc;
		max *= centerInc;
		m_OctaveBands.emplace_back(min, center, max);
	}*/

	
	float start = m_StartFreq * Audio::s_SamplingRate / 2.f;
	float end = m_EndFreq * Audio::s_SamplingRate / 2.f;
	float width = (end - start) / m_OctaveBandCount;
	float center = start + width / 2.f;
	float min = start;
	float max = start + width;

	for(int i = 0; i < m_OctaveBandCount; ++i){
		m_OctaveBands.emplace_back(min, center, max);
		center += width;
		min += width;
		max += width;
	}

	m_Amplitudes.resize(m_OctaveBands.size());
}