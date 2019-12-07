#pragma once
#include <vector>

class Spectrum {
	struct OctaveBand {
		OctaveBand(float min, float centr, float max);
		float min, centr, max;
	};
public:
	Spectrum(float startFreq, float endFreq, unsigned int count);

	void calculate(const std::vector<float>& fft);
	
	void setOctaveBandCount(unsigned int N);
	void setStartFreq(float startFreq);
	void setEndFreq(float endFreq);

	size_t size() { return m_Amplitudes.size(); }
	float operator[](unsigned int ind) { return m_Amplitudes[ind]; }
	std::vector<float>::iterator begin() { return m_Amplitudes.begin(); }
	std::vector<float>::iterator end() { return m_Amplitudes.end(); }

private:
	void calculateOctaveBands();
	std::vector<OctaveBand> m_OctaveBands;
	std::vector<float> m_Amplitudes;
	unsigned int m_OctaveBandCount;
	float m_StartFreq;
	float m_EndFreq;
};