#include "Audio.hpp"
#include "Logger.hpp"

const std::vector<const char*> Audio::s_AvailablePatterns = { "All Media Files","*.wav","*.aac","*.wma","*.wmv","*.avi","*.mpg","*.mpeg","*.m1v","*.mp2","*.mp3","*.mpa","*.mpe","*.m3u","*.mp4","*.mov","*.3g2","*.3gp2","*.3gp","*.3gpp","*.m4a","*.cda","*.aif","*.aifc","*.aiff","*.mid","*.midi","*.rmi","*.mkv","*.WAV","*.AAC","*.WMA","*.WMV","*.AVI","*.MPG","*.MPEG","*.M1V","*.MP2","*.MP3","*.MPA","*.MPE","*.M3U","*.MP4","*.MOV","*.3G2","*.3GP2","*.3GP","*.3GPP","*.M4A","*.CDA","*.AIF","*.AIFC","*.AIFF","*.MID","*.MIDI","*.RMI","*.MKV" };
const unsigned int Audio::s_SamplingRate = 44100;

void Audio::init()
{
	if (!BASS_Init(-1, s_SamplingRate, 0, 0, NULL)) {
		LOG_ERROR("Can't initialize default device. Error code {0}", BASS_ErrorGetCode());
	}
}

Audio::Audio()
	: m_IsPlaying(false), m_FFT(1024, 0), m_Channel(0)
{ }

Audio::Audio(std::string_view filepath)
	: Audio()
{
	loadFromFile(filepath);
}

void Audio::loadFromFile(std::string_view filepath)
{
	if (m_Channel != 0){
		stop();
		BASS_StreamFree(m_Channel);
	}
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

const std::vector<float>& Audio::getFFT()
{
	BASS_ChannelGetData(m_Channel, m_FFT.data(), BASS_DATA_FFT2048);
	return m_FFT;
}
