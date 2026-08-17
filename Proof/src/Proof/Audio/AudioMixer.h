#pragma once

#include "Proof/Asset/Asset.h"
#include "Proof/Core/UUID.h"

#include <MiniAudio.h>

namespace Proof
{

	struct AudioMixerGroupKey
	{
		
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

		ma_sound_group* GetNativeGroup() { return &m_Group; }
		const ma_sound_group* GetNativeGroup() const { return &m_Group; }

	private:
		UUID m_ID = { 0 };
		UUID m_ParentID = { 0 };

		std::string m_Name;
		float m_Volume = 1.0f;

		std::vector<UUID> m_Children;

		ma_sound_group m_Group{};

		friend class AudioMixer;
		friend class AudioMixerAssetSerilizer;
	};


	class AudioMixer : public Asset
	{
	public:
		AudioMixer();
		AudioMixer(UUID masterGroupID);
		~AudioMixer();

		//ASSET_CLASS_TYPE(AudioMixer);

		Count<AudioMixerGroup> CreateGroup(const std::string& name);
		Count<AudioMixerGroup> CreateGroup(const std::string& name, UUID parentID);

		bool RemoveGroup(UUID groupID);

		bool HasGroup(UUID groupID) const;

		Count<AudioMixerGroup> GetGroup(UUID groupID) const;
		Count<AudioMixerGroup> GetGroup(const std::string& name) const;

		Count<AudioMixerGroup> GetMasterGroup() const { return GetGroup(m_MasterGroupID); }
		UUID GetMasterGroupID() const { return m_MasterGroupID; }

		const std::unordered_map<UUID, Count<AudioMixerGroup>>& GetGroups() const { return m_Groups; }

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