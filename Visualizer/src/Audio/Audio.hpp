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

	const std::vector<float>& getFFT();
private:
	std::vector<float> m_FFT;
	bool m_IsPlaying;
	unsigned long m_Channel;
};