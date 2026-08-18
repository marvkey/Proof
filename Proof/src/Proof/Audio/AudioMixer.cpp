#include "Proofprch.h"
#include "AudioMixer.h"

#include "AudioEngine.h"
#include "AudioEffects.h"

namespace Proof
{
	AudioMixerGroup::AudioMixerGroup(UUID id, const std::string& name, UUID parentID, ma_sound_group* parentGroup)
		: m_ID(id), m_Name(name), m_ParentID(parentID)
	{
		m_EffectTable = Count<AudioEffectTable>::Create();

		ma_result result = ma_sound_group_init(&AudioEngine::GetEngine(), MA_SOUND_FLAG_NO_SPATIALIZATION, parentGroup, &m_Group);
		PF_CORE_ASSERT(result == MA_SUCCESS, "Failed to initialize AudioMixerGroup");

		ma_sound_group_set_volume(&m_Group, m_Volume);
	}

	AudioMixerGroup::~AudioMixerGroup()
	{
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

		Count<AudioMixerGroup> parent = GetGroup(parentID);

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
		Count<AudioMixerGroup> group = GetGroup(groupID);

		if (!group)
			return;

		std::vector<UUID> children = group->m_Children;

		for (UUID childID : children)
			DestroyGroupRecursive(childID);

		if (groupID != m_MasterGroupID)
		{
			Count<AudioMixerGroup> parent = GetGroup(group->m_ParentID);

			if (parent)
			{
				auto it = std::find(parent->m_Children.begin(), parent->m_Children.end(), groupID);

				if (it != parent->m_Children.end())
					parent->m_Children.erase(it);
			}
		}

		m_Groups.erase(groupID);
	}

	Count<AudioMixerGroup> AudioMixer::GetGroup(UUID groupID) const
	{
		auto it = m_Groups.find(groupID);

		if (it == m_Groups.end())
			return nullptr;

		return it->second;
	}

	Count<AudioMixerGroup> AudioMixer::GetGroup(const std::string& name) const
	{
		for (const auto& [id, group] : m_Groups)
		{
			if (group->GetName() == name)
				return group;
		}

		return nullptr;
	}
}