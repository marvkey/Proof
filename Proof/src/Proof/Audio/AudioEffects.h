#pragma once

#include "Proof/Asset/Asset.h"

#include "MiniAudio.h"

namespace Proof
{
	enum class AudioEffectType
	{
		None = 0,
		LowPass,
		HighPass,
		Echo,
		Equalizer,
		LowShelf,
		HighShelf,
	};

	class AudioEffect : public RefCounted
	{
	public:
		virtual ~AudioEffect() = default;

		virtual AudioEffectType GetType() const = 0;
		virtual ma_node* GetNode() = 0;

		bool IsEnabled() const { return m_Enabled; }
		void SetEnabled(bool enabled) { m_Enabled = enabled; }

	private:
		bool m_Enabled = true;
	};



	class AudioEffectTable : public RefCounted
	{
	public:
		AudioEffectTable() = default;

		void AddEffect(const Count<AudioEffect>& effect)
		{
			if (!effect)
				return;

			m_Effects.push_back(effect);
		}

		void InsertEffect(const Count<AudioEffect>& effect, uint32_t index)
		{
			if (!effect)
				return;

			if (index >= m_Effects.size())
			{
				m_Effects.push_back(effect);
				return;
			}

			m_Effects.insert(m_Effects.begin() + index, effect);
		}

		void RemoveEffect(uint32_t index)
		{
			if (index >= m_Effects.size())
				return;

			m_Effects.erase(m_Effects.begin() + index);
		}

		void MoveEffect(uint32_t fromIndex, uint32_t toIndex)
		{
			if (fromIndex >= m_Effects.size() || toIndex >= m_Effects.size())
				return;

			if (fromIndex == toIndex)
				return;

			Count<AudioEffect> effect = m_Effects[fromIndex];

			m_Effects.erase(m_Effects.begin() + fromIndex);
			m_Effects.insert(m_Effects.begin() + toIndex, effect);
		}

		void Clear()
		{
			m_Effects.clear();
		}

		Count<AudioEffect> GetEffect(uint32_t index) const
		{
			if (index >= m_Effects.size())
				return nullptr;

			return m_Effects[index];
		}

		uint32_t GetEffectCount() const
		{
			return (uint32_t)m_Effects.size();
		}

		bool Empty() const
		{
			return m_Effects.empty();
		}

		const std::vector<Count<AudioEffect>>& GetEffects() const
		{
			return m_Effects;
		}

		std::vector<Count<AudioEffect>>& GetEffects()
		{
			return m_Effects;
		}

	private:
		std::vector<Count<AudioEffect>> m_Effects;
	};

/*
	Audio Filter Order
	------------------

	The "order" controls how steep/aggressive the filter becomes after the cutoff frequency.

	Example:
		AudioLowPassEffect(float cutoffFrequency = 20000.0f, uint32_t order = 2);

	Here:
		- cutoffFrequency = WHERE the filtering starts happening.
		- order           = HOW aggressively frequencies are removed after that point.

	For a Low Pass Filter:
		- Frequencies BELOW the cutoff are mostly allowed through.
		- Frequencies ABOVE the cutoff are reduced.

	For a High Pass Filter:
		- Frequencies ABOVE the cutoff are mostly allowed through.
		- Frequencies BELOW the cutoff are reduced.

	Higher order = steeper cutoff.

	Approximate rolloff:

		Order 1 =  6 dB/octave  - very gentle
		Order 2 = 12 dB/octave  - normal/default
		Order 3 = 18 dB/octave
		Order 4 = 24 dB/octave  - strong
		Order 5 = 30 dB/octave
		Order 6 = 36 dB/octave  - very strong
		Order 7 = 42 dB/octave
		Order 8 = 48 dB/octave  - extremely steep

	Think of it like this:

		LOW ORDER:

		Volume
		  |
		1 |-------------------\
		  |                    \
		  |                     \
		  |                      \
		  +----------------------------> Frequency
		                       ^
		                     Cutoff


		HIGH ORDER:

		Volume
		  |
		1 |--------------------\
		  |                     |
		  |                     |
		  |                     \
		  +----------------------------> Frequency
		                       ^
		                     Cutoff

	The cutoff frequency DOES NOT mean everything instantly becomes silent there.
	It is the frequency where the filter begins its transition.

	The order determines how quickly the sound is reduced after that cutoff.

	Examples:

		Gentle muffling:
			Cutoff = 4000 Hz
			Order  = 1

		Normal wall/muffled effect:
			Cutoff = 2500 Hz
			Order  = 2

		Strong underwater/wall effect:
			Cutoff = 1200 Hz
			Order  = 4

		Extremely aggressive filtering:
			Order = 8

	MiniAudio's high-order filters support orders up to MA_MAX_FILTER_ORDER,
	which is normally 8.

	Good default:
		Order = 2

	In short:

		CUTOFF = where the filter acts.
		ORDER  = how steep/strong the filter acts.
*/




	class AudioLowPassEffect : public AudioEffect
	{
	public:
		AudioLowPassEffect(float cutoffFrequency = 20000.0f, uint32_t order = 2);
		~AudioLowPassEffect();

		AudioEffectType GetType() const override { return AudioEffectType::LowPass; }
		ma_node* GetNode() override { return (ma_node*)&m_Node; }

		void SetCutoffFrequency(float cutoffFrequency);
		void SetOrder(uint32_t order);

		float GetCutoffFrequency() const { return m_CutoffFrequency; }
		uint32_t GetOrder() const { return m_Order; }

	private:
		void UpdateFilter();

	private:
		ma_lpf_node m_Node{};

		float m_CutoffFrequency = 20000.0f;
		uint32_t m_Order = 2;
	};


	class AudioHighPassEffect : public AudioEffect
	{
	public:
		AudioHighPassEffect(float cutoffFrequency = 20.0f, uint32_t order = 2);
		~AudioHighPassEffect();

		AudioEffectType GetType() const override { return AudioEffectType::HighPass; }
		ma_node* GetNode() override { return (ma_node*)&m_Node; }

		void SetCutoffFrequency(float cutoffFrequency);
		void SetOrder(uint32_t order);

		float GetCutoffFrequency() const { return m_CutoffFrequency; }
		uint32_t GetOrder() const { return m_Order; }

	private:
		void UpdateFilter();

	private:
		ma_hpf_node m_Node{};

		float m_CutoffFrequency = 20.0f;
		uint32_t m_Order = 2;
	};


	class AudioEchoEffect : public AudioEffect
	{
	public:
		AudioEchoEffect(float delaySeconds = 0.2f, float decay = 0.25f, float wet = 0.25f, float dry = 1.0f);
		~AudioEchoEffect();

		AudioEffectType GetType() const override { return AudioEffectType::Echo; }
		ma_node* GetNode() override { return (ma_node*)&m_Node; }

		void SetWet(float wet);
		void SetDry(float dry);
		void SetDecay(float decay);

		float GetDelaySeconds() const { return m_DelaySeconds; }
		float GetWet() const { return m_Wet; }
		float GetDry() const { return m_Dry; }
		float GetDecay() const { return m_Decay; }

	private:
		ma_delay_node m_Node{};

		float m_DelaySeconds = 0.2f;
		float m_Decay = 0.25f;
		float m_Wet = 0.25f;
		float m_Dry = 1.0f;
	};


	class AudioEqualizerEffect : public AudioEffect
	{
	public:
		AudioEqualizerEffect(float frequency = 1000.0f, float gainDB = 0.0f, float q = 1.0f);
		~AudioEqualizerEffect();

		AudioEffectType GetType() const override { return AudioEffectType::Equalizer; }
		ma_node* GetNode() override { return (ma_node*)&m_Node; }

		void SetFrequency(float frequency);
		void SetGainDB(float gainDB);
		void SetQ(float q);

		float GetFrequency() const { return m_Frequency; }
		float GetGainDB() const { return m_GainDB; }
		float GetQ() const { return m_Q; }

	private:
		void UpdateFilter();

	private:
		ma_peak_node m_Node{};

		float m_Frequency = 1000.0f;
		float m_GainDB = 0.0f;
		float m_Q = 1.0f;
	};

	class AudioLowShelfEffect : public AudioEffect
	{
	public:
		AudioLowShelfEffect(float frequency = 200.0f, float gainDB = 0.0f, float q = 1.0f);
		~AudioLowShelfEffect();

		AudioEffectType GetType() const override { return AudioEffectType::LowShelf; }
		ma_node* GetNode() override { return (ma_node*)&m_Node; }

		void SetFrequency(float frequency);
		void SetGainDB(float gainDB);
		void SetQ(float q);

		float GetFrequency() const { return m_Frequency; }
		float GetGainDB() const { return m_GainDB; }
		float GetQ() const { return m_Q; }

	private:
		void UpdateFilter();

	private:
		ma_loshelf_node m_Node{};

		float m_Frequency = 200.0f;
		float m_GainDB = 0.0f;
		float m_Q = 1.0f;
	};


	class AudioHighShelfEffect : public AudioEffect
	{
	public:
		AudioHighShelfEffect(float frequency = 8000.0f, float gainDB = 0.0f, float q = 1.0f);
		~AudioHighShelfEffect();

		AudioEffectType GetType() const override { return AudioEffectType::HighShelf; }
		ma_node* GetNode() override { return (ma_node*)&m_Node; }

		void SetFrequency(float frequency);
		void SetGainDB(float gainDB);
		void SetQ(float q);

		float GetFrequency() const { return m_Frequency; }
		float GetGainDB() const { return m_GainDB; }
		float GetQ() const { return m_Q; }

	private:
		void UpdateFilter();

	private:
		ma_hishelf_node m_Node{};

		float m_Frequency = 8000.0f;
		float m_GainDB = 0.0f;
		float m_Q = 1.0f;
	};

	static inline Count<AudioEffect> CreateAudioEffect(AudioEffectType type)
	{
		switch (type)
		{
		case AudioEffectType::LowPass: return Count<AudioLowPassEffect>::Create().As<AudioEffect>();
		case AudioEffectType::HighPass: return Count<AudioHighPassEffect>::Create().As<AudioEffect>();
		case AudioEffectType::Echo: return Count<AudioEchoEffect>::Create().As<AudioEffect>();
		case AudioEffectType::Equalizer: return Count<AudioEqualizerEffect>::Create().As<AudioEffect>();
		case AudioEffectType::LowShelf: return Count<AudioLowShelfEffect>::Create().As<AudioEffect>();
		case AudioEffectType::HighShelf: return Count<AudioHighShelfEffect>::Create().As<AudioEffect>();
		case AudioEffectType::None:
		default: return nullptr;
		}
	}
}