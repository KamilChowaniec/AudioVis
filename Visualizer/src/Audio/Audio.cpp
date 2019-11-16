#include "Audio.hpp"
#include "Logger.hpp"

void Audio::init()
{
	if (!BASS_Init(-1, 44100, 0, 0, NULL)) {
		LOG_ERROR("Can't initialize default device. Error code {0}", BASS_ErrorGetCode());
	}
}

Audio::Audio(std::string_view filepath)
	: m_IsPlaying(false)
{
	if (!(m_Channel = BASS_StreamCreateFile(FALSE, filepath.data(), 0, 0, BASS_SAMPLE_LOOP))
		&& !(m_Channel = BASS_MusicLoad(FALSE, filepath.data(), 0, 0, BASS_MUSIC_RAMP | BASS_SAMPLE_LOOP, 1))) {
		LOG_ERROR("Can't open audio file: {0}. Error code: {1}", filepath, BASS_ErrorGetCode());
	}

	setVolume(0.1);
}

void Audio::play()
{
	if (!BASS_ChannelPlay(m_Channel, false)) {
		LOG_ERROR("Can't start audio. Error code: {0}", BASS_ErrorGetCode());
	}
	else m_IsPlaying = true;
}

void Audio::restart() {
	if (!BASS_ChannelPlay(m_Channel, true)) {
		LOG_ERROR("Can't restart audio. Error code: {0}", BASS_ErrorGetCode());
	}
	else m_IsPlaying = true;
}

void Audio::pause()
{
	if (!BASS_ChannelPause(m_Channel)) {
		LOG_ERROR("Can't pause audio. Error code: {0}", BASS_ErrorGetCode());
	}
	else m_IsPlaying = true;
}

void Audio::stop()
{
	if (!BASS_ChannelStop(m_Channel)) {
		LOG_ERROR("Can't stop audio. Error code: {0}", BASS_ErrorGetCode());
	}
	else m_IsPlaying = true;
}

bool Audio::isPlaying() const
{
	return m_IsPlaying;
}

void Audio::setVolume(float level)
{
	if (!BASS_ChannelSetAttribute(m_Channel, BASS_ATTRIB_VOL, level)) {
		LOG_ERROR("Can't change volume. Error code: {0}", BASS_ErrorGetCode());
	}
}

float Audio::getVolume() const
{
	float volume;
	if (!BASS_ChannelGetAttribute(m_Channel, BASS_ATTRIB_VOL, &volume)) {
		LOG_ERROR("Can't get volume level. Error code: {0}", BASS_ErrorGetCode());
	}
	return volume;
}

const float* Audio::getFFT()
{
	BASS_ChannelGetData(m_Channel, m_FFT, BASS_DATA_FFT2048);
	return m_FFT;
}
