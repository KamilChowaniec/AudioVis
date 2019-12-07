#pragma once
#include <string>
#include <vector>
#include "bass.h"

class Audio {
public:
	static void init();
	Audio();
	Audio(std::string_view filepath);
	
	void loadFromFile(std::string_view filepath);

	void play();
	void restart();
	void pause();
	void stop();

	bool isPlaying() const;
	
	void setVolume(float percent);
	float getVolume() const;

	void setPosition(float percent);
	float getPosition();

	double getLength();

	const std::vector<float>& getFFT();

	static const unsigned int s_SamplingRate;
	static const std::vector<const char*> s_AvailablePatterns;
private:

	std::vector<float> m_FFT;
	long m_Length;
	bool m_IsPlaying;
	unsigned long m_Channel;

};