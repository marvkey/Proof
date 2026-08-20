#pragma once

#include "Proof/Asset/Asset.h"
#include "Proof/Core/UUID.h"

#include <MiniAudio.h>

namespace Proof
{
	struct AudioMixerMeterNode
	{
		ma_node_base Base{};
		std::atomic<float> CurrentLevel = 0.0f;
		ma_uint32 Channels = 0;
	};
	
	class AudioMixerGroup : public RefCounted
	{
	public:
		AudioMixerGroup(UUID id, const std::string& name, UUID parentID, ma_sound_group* parentGroup);
		~AudioMixerGroup();

		AudioMixerGroup(const AudioMixerGroup&) = delete;
		AudioMixerGroup& operator=(const AudioMixerGroup&) = delete;

		UUID GetID() const { return m_ID; }
		UUID GetParentID() const { return m_ParentID; }

		const std::string& GetName() const { return m_Name; }
		void SetName(const std::string& name) { m_Name = name; }

		float GetVolume() const { return m_Volume; }
		void SetVolume(float volume);

		const std::vector<UUID>& GetChildren() const { return m_Children; }

		float GetCurrentLevel() const
		{
			return m_MeterNode.CurrentLevel.load(std::memory_order_relaxed);
		}

		ma_sound_group* GetNativeGroup() { return &m_Group; }
		const ma_sound_group* GetNativeGroup() const { return &m_Group; }

		Count<class AudioEffectTable> GetEffectTable(){return m_EffectTable;}

	private:
		UUID m_ID = { 0 };
		UUID m_ParentID = { 0 };

		std::string m_Name;
		float m_Volume = 1.0f;

		std::vector<UUID> m_Children;

		ma_sound_group m_Group{};
		Count<class AudioEffectTable> m_EffectTable = nullptr;
		AudioMixerMeterNode m_MeterNode{};

		friend class AudioMixer;
		friend class AudioMixerAssetSerilizer;
	};


	class AudioMixer : public Asset
	{
	public:
		AudioMixer();
		AudioMixer(UUID masterGroupID);
		~AudioMixer();

		ASSET_CLASS_TYPE(AudioMixer);

		Count<AudioMixerGroup> CreateGroup(const std::string& name);
		Count<AudioMixerGroup> CreateGroup(const std::string& name, UUID parentID);

		bool RemoveGroup(UUID groupID);

		bool HasGroup(UUID groupID) const;

		Count<AudioMixerGroup> TryGetGroup(UUID groupID) const;
		Count<AudioMixerGroup> TryGetGroup(const std::string& name) const;

		Count<AudioMixerGroup> GetMasterGroup() const { return TryGetGroup(m_MasterGroupID); }
		UUID GetMasterGroupID() const { return m_MasterGroupID; }

		const std::unordered_map<UUID, Count<AudioMixerGroup>>& GetGroups() const { return m_Groups; }

		bool IsDescendant(UUID groupID, UUID potentialDescendantID) const
		{
			Count<AudioMixerGroup> group = TryGetGroup(groupID);

			if (!group)
				return false;

			for (UUID childID : group->m_Children)
			{
				if (childID == potentialDescendantID)
					return true;

				if (IsDescendant(childID, potentialDescendantID))
					return true;
			}

			return false;
		}

		bool ReparentGroup(UUID groupID, UUID newParentID)
		{
			if (groupID == m_MasterGroupID)
				return false;

			if (groupID == newParentID)
				return false;

			Count<AudioMixerGroup> group = TryGetGroup(groupID);
			Count<AudioMixerGroup> newParent = TryGetGroup(newParentID);

			if (!group || !newParent)
				return false;

			if (IsDescendant(groupID, newParentID))
				return false;

			Count<AudioMixerGroup> oldParent = TryGetGroup(group->m_ParentID);

			if (oldParent)
			{
				auto it = std::find(oldParent->m_Children.begin(), oldParent->m_Children.end(), groupID);

				if (it != oldParent->m_Children.end())
					oldParent->m_Children.erase(it);
			}

			group->m_ParentID = newParentID;
			newParent->m_Children.push_back(groupID);

			// MiniAudio's native routing needs to be moved as well.
			ma_node_detach_output_bus(&group->m_Group, 0);
			ma_node_attach_output_bus(&group->m_Group, 0, &newParent->m_Group, 0);

			return true;
		}
	private:
		UUID GenerateGroupID() const;

		Count<AudioMixerGroup> CreateGroupWithID(UUID groupID, const std::string& name, UUID parentID);

		void DestroyGroupRecursive(UUID groupID);

	private:
		UUID m_MasterGroupID = { 0 };

		std::unordered_map<UUID, Count<AudioMixerGroup>> m_Groups;

		friend class AudioMixerAssetSerilizer;
	};
}