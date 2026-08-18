#include "Proofprch.h"
#include "AudioEffects.h"

#include "AudioEngine.h"

namespace Proof
{
	static ma_uint32 GetAudioChannels()
	{
		return ma_engine_get_channels(&AudioEngine::GetEngine());
	}

	static ma_uint32 GetAudioSampleRate()
	{
		return ma_engine_get_sample_rate(&AudioEngine::GetEngine());
	}
	static ma_node_graph* GetAudioNodeGraph()
	{
		return ma_engine_get_node_graph(&AudioEngine::GetEngine());
	}

	// --------------------------------------------------------------------------------------------
	// Low Pass
	// --------------------------------------------------------------------------------------------

	AudioLowPassEffect::AudioLowPassEffect(float cutoffFrequency, uint32_t order)
		: m_CutoffFrequency(cutoffFrequency), m_Order(order)
	{
		ma_lpf_node_config config = ma_lpf_node_config_init(GetAudioChannels(), GetAudioSampleRate(), m_CutoffFrequency, m_Order);

		ma_result result = ma_lpf_node_init(ma_engine_get_node_graph(&AudioEngine::GetEngine()), &config, nullptr, &m_Node);
		PF_CORE_ASSERT(result == MA_SUCCESS, "Failed to create AudioLowPassEffect");
	}

	AudioLowPassEffect::~AudioLowPassEffect()
	{
		ma_lpf_node_uninit(&m_Node, nullptr);
	}

	void AudioLowPassEffect::SetCutoffFrequency(float cutoffFrequency)
	{
		if (m_CutoffFrequency == cutoffFrequency)
			return;

		m_CutoffFrequency = cutoffFrequency;
		UpdateFilter();
	}

	void AudioLowPassEffect::SetOrder(uint32_t order)
	{
		if (m_Order == order)
			return;

		m_Order = order;
		UpdateFilter();
	}

	void AudioLowPassEffect::UpdateFilter()
	{
		ma_lpf_node_config config = ma_lpf_node_config_init(GetAudioChannels(), GetAudioSampleRate(), m_CutoffFrequency, m_Order);
		ma_lpf_node_reinit(&config.lpf, &m_Node);
	}


	// --------------------------------------------------------------------------------------------
	// High Pass
	// --------------------------------------------------------------------------------------------

	AudioHighPassEffect::AudioHighPassEffect(float cutoffFrequency, uint32_t order)
		: m_CutoffFrequency(cutoffFrequency), m_Order(order)
	{
		ma_hpf_node_config config = ma_hpf_node_config_init(GetAudioChannels(), GetAudioSampleRate(), m_CutoffFrequency, m_Order);

		ma_result result = ma_hpf_node_init(ma_engine_get_node_graph(&AudioEngine::GetEngine()), &config, nullptr, &m_Node);
		PF_CORE_ASSERT(result == MA_SUCCESS, "Failed to create AudioHighPassEffect");
	}

	AudioHighPassEffect::~AudioHighPassEffect()
	{
		ma_hpf_node_uninit(&m_Node, nullptr);
	}

	void AudioHighPassEffect::SetCutoffFrequency(float cutoffFrequency)
	{
		if (m_CutoffFrequency == cutoffFrequency)
			return;

		m_CutoffFrequency = cutoffFrequency;
		UpdateFilter();
	}

	void AudioHighPassEffect::SetOrder(uint32_t order)
	{
		if (m_Order == order)
			return;

		m_Order = order;
		UpdateFilter();
	}

	void AudioHighPassEffect::UpdateFilter()
	{
		ma_hpf_node_config config = ma_hpf_node_config_init(GetAudioChannels(), GetAudioSampleRate(), m_CutoffFrequency, m_Order);
		ma_hpf_node_reinit(&config.hpf, &m_Node);
	}


	// --------------------------------------------------------------------------------------------
	// Echo / Delay
	// --------------------------------------------------------------------------------------------

	AudioEchoEffect::AudioEchoEffect(float delaySeconds, float decay, float wet, float dry)
		: m_DelaySeconds(delaySeconds), m_Decay(decay), m_Wet(wet), m_Dry(dry)
	{
		ma_uint32 sampleRate = GetAudioSampleRate();
		ma_uint32 delayFrames = (ma_uint32)(sampleRate * m_DelaySeconds);

		ma_delay_node_config config = ma_delay_node_config_init(GetAudioChannels(), sampleRate, delayFrames, m_Decay);

		ma_result result = ma_delay_node_init(ma_engine_get_node_graph(&AudioEngine::GetEngine()), &config, nullptr, &m_Node);
		PF_CORE_ASSERT(result == MA_SUCCESS, "Failed to create AudioEchoEffect");

		ma_delay_node_set_wet(&m_Node, m_Wet);
		ma_delay_node_set_dry(&m_Node, m_Dry);
	}

	AudioEchoEffect::~AudioEchoEffect()
	{
		ma_delay_node_uninit(&m_Node, nullptr);
	}

	void AudioEchoEffect::SetWet(float wet)
	{
		m_Wet = wet;
		ma_delay_node_set_wet(&m_Node, m_Wet);
	}

	void AudioEchoEffect::SetDry(float dry)
	{
		m_Dry = dry;
		ma_delay_node_set_dry(&m_Node, m_Dry);
	}

	void AudioEchoEffect::SetDecay(float decay)
	{
		m_Decay = decay;
		ma_delay_node_set_decay(&m_Node, m_Decay);
	}


	// --------------------------------------------------------------------------------------------
	// Equalizer
	// --------------------------------------------------------------------------------------------

	AudioEqualizerEffect::AudioEqualizerEffect(float frequency, float gainDB, float q)
		: m_Frequency(frequency), m_GainDB(gainDB), m_Q(q)
	{
		ma_peak_node_config config = ma_peak_node_config_init(GetAudioChannels(), GetAudioSampleRate(), m_GainDB, m_Q, m_Frequency);

		ma_result result = ma_peak_node_init(ma_engine_get_node_graph(&AudioEngine::GetEngine()), &config, nullptr, &m_Node);
		PF_CORE_ASSERT(result == MA_SUCCESS, "Failed to create AudioEqualizerEffect");
	}

	AudioEqualizerEffect::~AudioEqualizerEffect()
	{
		ma_peak_node_uninit(&m_Node, nullptr);
	}

	void AudioEqualizerEffect::SetFrequency(float frequency)
	{
		if (m_Frequency == frequency)
			return;

		m_Frequency = frequency;
		UpdateFilter();
	}

	void AudioEqualizerEffect::SetGainDB(float gainDB)
	{
		if (m_GainDB == gainDB)
			return;

		m_GainDB = gainDB;
		UpdateFilter();
	}

	void AudioEqualizerEffect::SetQ(float q)
	{
		if (m_Q == q)
			return;

		m_Q = q;
		UpdateFilter();
	}

	void AudioEqualizerEffect::UpdateFilter()
	{
		ma_peak_node_config config = ma_peak_node_config_init(GetAudioChannels(), GetAudioSampleRate(), m_GainDB, m_Q, m_Frequency);
		ma_peak_node_reinit(&config.peak, &m_Node);
	}


	// --------------------------------------------------------------------------------------------
	// Low Shelf
	// --------------------------------------------------------------------------------------------

	AudioLowShelfEffect::AudioLowShelfEffect(float frequency, float gainDB, float q)
		: m_Frequency(frequency), m_GainDB(gainDB), m_Q(q)
	{
		ma_loshelf_node_config config = ma_loshelf_node_config_init(GetAudioChannels(), GetAudioSampleRate(), m_GainDB, m_Q, m_Frequency);

		ma_result result = ma_loshelf_node_init(GetAudioNodeGraph(), &config, nullptr, &m_Node);
		PF_CORE_ASSERT(result == MA_SUCCESS, "Failed to create AudioLowShelfEffect");
	}

	AudioLowShelfEffect::~AudioLowShelfEffect()
	{
		ma_loshelf_node_uninit(&m_Node, nullptr);
	}

	void AudioLowShelfEffect::SetFrequency(float frequency)
	{
		if (m_Frequency == frequency)
			return;

		m_Frequency = frequency;
		UpdateFilter();
	}

	void AudioLowShelfEffect::SetGainDB(float gainDB)
	{
		if (m_GainDB == gainDB)
			return;

		m_GainDB = gainDB;
		UpdateFilter();
	}

	void AudioLowShelfEffect::SetQ(float q)
	{
		if (m_Q == q)
			return;

		m_Q = q;
		UpdateFilter();
	}

	void AudioLowShelfEffect::UpdateFilter()
	{
		ma_loshelf_node_config config = ma_loshelf_node_config_init(GetAudioChannels(), GetAudioSampleRate(), m_GainDB, m_Q, m_Frequency);
		ma_loshelf_node_reinit(&config.loshelf, &m_Node);
	}


	// --------------------------------------------------------------------------------------------
	// High Shelf
	// --------------------------------------------------------------------------------------------

	AudioHighShelfEffect::AudioHighShelfEffect(float frequency, float gainDB, float q)
		: m_Frequency(frequency), m_GainDB(gainDB), m_Q(q)
	{
		ma_hishelf_node_config config = ma_hishelf_node_config_init(GetAudioChannels(), GetAudioSampleRate(), m_GainDB, m_Q, m_Frequency);

		ma_result result = ma_hishelf_node_init(GetAudioNodeGraph(), &config, nullptr, &m_Node);
		PF_CORE_ASSERT(result == MA_SUCCESS, "Failed to create AudioHighShelfEffect");
	}

	AudioHighShelfEffect::~AudioHighShelfEffect()
	{
		ma_hishelf_node_uninit(&m_Node, nullptr);
	}

	void AudioHighShelfEffect::SetFrequency(float frequency)
	{
		if (m_Frequency == frequency)
			return;

		m_Frequency = frequency;
		UpdateFilter();
	}

	void AudioHighShelfEffect::SetGainDB(float gainDB)
	{
		if (m_GainDB == gainDB)
			return;

		m_GainDB = gainDB;
		UpdateFilter();
	}

	void AudioHighShelfEffect::SetQ(float q)
	{
		if (m_Q == q)
			return;

		m_Q = q;
		UpdateFilter();
	}

	void AudioHighShelfEffect::UpdateFilter()
	{
		ma_hishelf_node_config config = ma_hishelf_node_config_init(GetAudioChannels(), GetAudioSampleRate(), m_GainDB, m_Q, m_Frequency);
		ma_hishelf_node_reinit(&config.hishelf, &m_Node);
	}
}