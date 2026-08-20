#include "Proofprch.h"
#include "AudioMixer.h"

#include "AudioEngine.h"
#include "AudioEffects.h"

namespace Proof
{

	static void AudioMixerMeterProcess(ma_node* node, const float** framesIn, ma_uint32* frameCountIn, float** framesOut, ma_uint32* frameCountOut)
	{
		AudioMixerMeterNode* meter = reinterpret_cast<AudioMixerMeterNode*>(node);

		ma_uint32 frameCount = *frameCountOut;

		if (*frameCountIn < frameCount)
			frameCount = *frameCountIn;

		ma_uint32 sampleCount = frameCount * meter->Channels;

		const float* input = framesIn[0];
		float* output = framesOut[0];

		float peak = 0.0f;

		for (ma_uint32 i = 0; i < sampleCount; i++)
		{
			float sample = input[i];

			output[i] = sample;

			float level = std::abs(sample);

			if (level > peak)
				peak = level;
		}

		meter->CurrentLevel.store(peak, std::memory_order_relaxed);

		*frameCountIn = frameCount;
		*frameCountOut = frameCount;
	}

	static ma_node_vtable AudioMixerMeterVTable =
	{
		AudioMixerMeterProcess,
		nullptr,
		1,
		1,
		MA_NODE_FLAG_CONTINUOUS_PROCESSING
	};


	AudioMixerGroup::AudioMixerGroup(UUID id, const std::string& name, UUID parentID, ma_sound_group* parentGroup)
		: m_ID(id), m_Name(name), m_ParentID(parentID)
	{
		ma_engine& engine = AudioEngine::GetEngine();

		ma_result result = ma_sound_group_init(&engine, MA_SOUND_FLAG_NO_SPATIALIZATION, parentGroup, &m_Group);
		PF_CORE_ASSERT(result == MA_SUCCESS, "Failed to initialize AudioMixerGroup");

		m_MeterNode.Channels = ma_engine_get_channels(&engine);

		ma_uint32 inputChannels[] = { m_MeterNode.Channels };
		ma_uint32 outputChannels[] = { m_MeterNode.Channels };

		ma_node_config meterConfig = ma_node_config_init();
		meterConfig.vtable = &AudioMixerMeterVTable;
		meterConfig.pInputChannels = inputChannels;
		meterConfig.pOutputChannels = outputChannels;

		result = ma_node_init(ma_engine_get_node_graph(&engine), &meterConfig, nullptr, &m_MeterNode.Base);
		PF_CORE_ASSERT(result == MA_SUCCESS, "Failed to initialize AudioMixer meter");

		ma_node_attach_output_bus(&m_Group, 0, &m_MeterNode.Base, 0);

		if (parentGroup)
			ma_node_attach_output_bus(&m_MeterNode.Base, 0, parentGroup, 0);
		else
			ma_node_attach_output_bus(&m_MeterNode.Base, 0, ma_engine_get_endpoint(&engine), 0);

		ma_sound_group_set_volume(&m_Group, m_Volume);
	}

	AudioMixerGroup::~AudioMixerGroup()
	{
		ma_node_uninit(&m_MeterNode.Base, nullptr);
		ma_sound_group_uninit(&m_Group);
	}

	void AudioMixerGroup::SetVolume(float volume)
	{
		m_Volume = volume;
		ma_sound_group_set_volume(&m_Group, m_Volume);
	}


	AudioMixer::AudioMixer()
	{
		m_MasterGroupID = GenerateGroupID();
		Count<AudioMixerGroup> master = Count<AudioMixerGroup>::Create(m_MasterGroupID, "Master", UUID(0), nullptr);
		m_Groups.emplace(m_MasterGroupID, master);
	}

	AudioMixer::AudioMixer(UUID masterGroupID)
	{

		if (masterGroupID.Get() == 0)
			masterGroupID = GenerateGroupID();

		m_MasterGroupID = masterGroupID;


		Count<AudioMixerGroup> master = Count<AudioMixerGroup>::Create(m_MasterGroupID, "Master", UUID(0), nullptr);
		m_Groups.emplace(m_MasterGroupID, master);
	}

	AudioMixer::~AudioMixer()
	{
		if (HasGroup(m_MasterGroupID))
			DestroyGroupRecursive(m_MasterGroupID);

		m_Groups.clear();
	}

	UUID AudioMixer::GenerateGroupID() const
	{
		UUID id = UUID();

		while (id.Get() == 0 || HasGroup(id))
			id = UUID();

		return id;
	}

	bool AudioMixer::HasGroup(UUID groupID) const
	{
		return m_Groups.contains(groupID);
	}

	Count<AudioMixerGroup> AudioMixer::CreateGroup(const std::string& name)
	{
		return CreateGroup(name, m_MasterGroupID);
	}

	Count<AudioMixerGroup> AudioMixer::CreateGroup(const std::string& name, UUID parentID)
	{
		return CreateGroupWithID(GenerateGroupID(), name, parentID);
	}

	Count<AudioMixerGroup> AudioMixer::CreateGroupWithID(UUID groupID, const std::string& name, UUID parentID)
	{
		if (groupID.Get() == 0)
		{
			PF_ENGINE_ERROR("Cannot create AudioMixerGroup '{}' with invalid UUID", name);
			return nullptr;
		}

		if (HasGroup(groupID))
		{
			PF_ENGINE_ERROR("Cannot create AudioMixerGroup '{}' because UUID {} is already used", name, groupID.Get());
			return nullptr;
		}

		Count<AudioMixerGroup> parent = TryGetGroup(parentID);

		if (!parent)
		{
			PF_ENGINE_ERROR("Cannot create AudioMixerGroup '{}' because parent UUID {} does not exist", name, parentID.Get());
			return nullptr;
		}

		Count<AudioMixerGroup> group = Count<AudioMixerGroup>::Create(groupID, name, parentID, parent->GetNativeGroup());

		m_Groups.emplace(groupID, group);
		parent->m_Children.push_back(groupID);

		return group;
	}

	bool AudioMixer::RemoveGroup(UUID groupID)
	{
		if (groupID == m_MasterGroupID)
		{
			PF_ENGINE_ERROR("Cannot remove Master AudioMixerGroup");
			return false;
		}

		if (!HasGroup(groupID))
			return false;

		DestroyGroupRecursive(groupID);

		return true;
	}

	void AudioMixer::DestroyGroupRecursive(UUID groupID)
	{
		Count<AudioMixerGroup> group = TryGetGroup(groupID);

		if (!group)
			return;

		std::vector<UUID> children = group->m_Children;

		for (UUID childID : children)
			DestroyGroupRecursive(childID);

		if (groupID != m_MasterGroupID)
		{
			Count<AudioMixerGroup> parent = TryGetGroup(group->m_ParentID);

			if (parent)
			{
				auto it = std::find(parent->m_Children.begin(), parent->m_Children.end(), groupID);

				if (it != parent->m_Children.end())
					parent->m_Children.erase(it);
			}
		}

		m_Groups.erase(groupID);
	}

	Count<AudioMixerGroup> AudioMixer::TryGetGroup(UUID groupID) const
	{
		auto it = m_Groups.find(groupID);

		if (it == m_Groups.end())
			return nullptr;

		return it->second;
	}

	Count<AudioMixerGroup> AudioMixer::TryGetGroup(const std::string& name) const
	{
		for (const auto& [id, group] : m_Groups)
		{
			if (group->GetName() == name)
				return group;
		}

		return nullptr;
	}
}