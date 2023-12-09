#pragma once
#include "Editor3D/Editors/Panels/Panel.h"
#include "Proof/Asset/Asset.h"
#include "ContentBrowserUtils.h"
#include  "Proof/Utils/FileSystem.h"
#include <map>
#include <functional>
#include <mutex>
#define MAX_INPUT_BUFFER_LENGTH 128
namespace Proof
{
	class SelectionStack
	{
	public:
		void CopyFrom(const SelectionStack& other)
		{
			m_Selections.assign(other.begin(), other.end());
		}

		void CopyFrom(const std::vector<UUID>& other)
		{
			m_Selections.assign(other.begin(), other.end());
		}

		void Select(UUID handle)
		{
			if (IsSelected(handle))
				return;

			m_Selections.push_back(handle);
		}

		void Deselect(UUID handle)
		{
			if (!IsSelected(handle))
				return;

			for (auto it = m_Selections.begin(); it != m_Selections.end(); it++)
			{
				if (handle == *it)
				{
					m_Selections.erase(it);
					break;
				}
			}
		}

		bool IsSelected(UUID handle) const
		{
			for (const auto& selectedHandle : m_Selections)
			{
				if (selectedHandle == handle)
					return true;
			}

			return false;
		}

		void Clear()
		{
			m_Selections.clear();
		}

		size_t SelectionCount() const { return m_Selections.size(); }
		const UUID* SelectionData() const { return m_Selections.data(); }

		UUID operator[](size_t index) const
		{
			PF_CORE_ASSERT(index >= 0 && index < m_Selections.size());
			return m_Selections[index];
		}

		std::vector<UUID>::iterator begin() { return m_Selections.begin(); }
		std::vector<UUID>::const_iterator begin() const { return m_Selections.begin(); }
		std::vector<UUID>::iterator end() { return m_Selections.end(); }
		std::vector<UUID>::const_iterator end() const { return m_Selections.end(); }

	private:
		std::vector<UUID> m_Selections;
	};

	struct ContentBrowserItemList
	{
		static constexpr size_t InvalidItem = std::numeric_limits<size_t>::max();

		std::vector<Count<ContentBrowserItem>> Items;

		std::vector<Count<ContentBrowserItem>>::iterator begin() { return Items.begin(); }
		std::vector<Count<ContentBrowserItem>>::iterator end() { return Items.end(); }
		std::vector<Count<ContentBrowserItem>>::const_iterator begin() const { return Items.begin(); }
		std::vector<Count<ContentBrowserItem>>::const_iterator end() const { return Items.end(); }

		Count<ContentBrowserItem>& operator[](size_t index) { return Items[index]; }
		const Count<ContentBrowserItem>& operator[](size_t index) const { return Items[index]; }

		ContentBrowserItemList() = default;

		ContentBrowserItemList(const ContentBrowserItemList& other)
			: Items(other.Items)
		{
		}

		ContentBrowserItemList& operator=(const ContentBrowserItemList& other)
		{
			Items = other.Items;
			return *this;
		}

		void Clear()
		{
			std::scoped_lock<std::mutex> lock(m_Mutex);
			Items.clear();
		}

		void erase(AssetID handle)
		{
			size_t index = FindItem(handle);
			if (index == InvalidItem)
				return;

			std::scoped_lock<std::mutex> lock(m_Mutex);
			auto it = Items.begin() + index;
			Items.erase(it);
		}

		size_t FindItem(AssetID handle)
		{
			if (Items.size() == 0)
				return InvalidItem;

			std::scoped_lock<std::mutex> lock(m_Mutex);
			for (size_t i = 0; i < Items.size(); i++)
			{
				if (Items[i]->GetID() == handle)
					return i;
			}

			return InvalidItem;
		}

	private:
		std::mutex m_Mutex;
	};
	

	class ContentBrowserPanel : public Panel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender(const char* dsiplayName, bool& isOpen);
		static ContentBrowserPanel& Get();
		ContentBrowserItemList& GetCurrentItems() { return m_CurrentItems; }
		Count<DirectoryInfo> GetDirectory(const std::filesystem::path& filepath) const;
		void RegisterItemActivateCallbackForType(AssetType type, const std::function<void(const AssetInfo&)>& callback)
		{
			m_ItemActivationCallbacks[type] = callback;
		}

		void RegisterAssetCreatedCallback(const std::function<void(const AssetInfo&)>& callback)
		{
			m_NewAssetCreatedCallbacks.push_back(callback);
		}

		void RegisterAssetDeletedCallback(const std::function<void(const AssetInfo&)>& callback)
		{
			m_AssetDeletedCallbacks.push_back(callback);
		}
		void OnFileSystemChanged(const std::vector<FileSystemChangedEvent>& events);

	protected:
		virtual void OnEvent(Event& e) override;
		virtual void SetWorldContext(const Count<class World>& world) override { m_WorldContext = world; }

	private:
		Count<class World> m_WorldContext;
		char m_SearchBuffer[MAX_INPUT_BUFFER_LENGTH];
		std::unordered_map<AssetType, std::function<void(const AssetInfo&)>> m_ItemActivationCallbacks;
		std::vector<std::function<void(const AssetInfo&)>> m_NewAssetCreatedCallbacks;
		std::vector<std::function<void(const AssetInfo&)>> m_AssetDeletedCallbacks;

		Count< DirectoryInfo> m_BaseDirectory;
		Count< DirectoryInfo> m_CurrentDirectory;
		Count<DirectoryInfo> m_NextDirectory, m_PreviousDirectory;

		ContentBrowserItemList m_CurrentItems;
		std::map<std::string, Count<Texture2D>> m_AssetIconMap;
		std::unordered_map<UUID, Count<DirectoryInfo>> m_Directories;
		std::vector<Count<DirectoryInfo>> m_BreadCrumbData;

		bool m_UpdateNavigationPath = false;
		bool m_IsContentBrowserHovered = false;
		bool m_IsContentBrowserFocused = false;

		bool m_ShowAssetType = true;
		bool m_IsAnyItemHovered = false;
		bool m_OpenDeletePopup = false;
		bool m_OpenNewScriptPopup = false;
		std::mutex m_LockMutex;
		SelectionStack m_CopiedAssets;
		float m_Padding = 2.0f;
	private:

		bool OnKeyPressedEvent(struct KeyPressedEvent& e);
		bool OnMouseButtonPressed(struct MouseButtonPressedEvent& e);

		void OnBrowseForward();
		void OnBrowseBack();
		void ChangeDirectory(Count<DirectoryInfo>& directory);
		AssetID ProcessDirectory(const std::filesystem::path& directoryPath, const Count<DirectoryInfo>& parent);

		void RenderDirectoryHierarchy(Count<DirectoryInfo>& directory);
		void UpdateDropArea(const Count<DirectoryInfo>& directory);
		void RenderTopBar(float height);
		void RenderBottomBar(float height);

		void RenderItems();
		void RenderDeleteDialogue();
		void RenderNewScriptDialogue();
		void RemoveDirectory(Count<DirectoryInfo>& directory, bool removeFromParent =true);
		void SortItemList();
		void ClearSelections();	
		void Refresh();
		void RefreshWithoutLock();
		void UpdateInput();
		void PasteCopiedAssets();

		ContentBrowserItemList Search(const std::string& query, const Count<DirectoryInfo>& directoryInfo);

		template<typename T, typename...Args>
		Count<T> CreateAsset(const std::string& filenameNoExtesnsion, Args&&... args)
		{
			return CreateAssetInDirectory<T>(filenameNoExtesnsion, m_CurrentDirectory, std::forward<Args>(args)...);
		}
		// NOTE: This should only be used within the ContentBrowserPanel!
		// For creating a new asset outside the content browser, use AssetManager::CreateNewAsset!
		template<typename T, typename... Args>
		Count<T> CreateAssetInDirectory(const std::string& filenameNoExtension, Count<DirectoryInfo>& directory, Args&&... args)
		{
			auto filepath = FileSystem::GenerateUniqueFileName(AssetManager::GetDirectory() / directory->FilePath / (filenameNoExtension + Utils::GetAssetExtensionString(T::GetStaticType())));
			Count<T> asset = AssetManager::NewAsset<T>(filepath, std::forward<Args>(args)...);
			if (!asset)
				return nullptr;

			directory->Assets.push_back(asset->GetID());

			const auto& assetInfo = AssetManager::GetAssetInfo(asset);
			for (auto& callback : m_NewAssetCreatedCallbacks)
				callback(assetInfo);

			return asset;
		}


	};
}