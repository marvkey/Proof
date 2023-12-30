#pragma once
#include "Proof/Core/SmartPointer.h"
#include "Proof/Asset/AssetTypes.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Renderer/Texture.h"
namespace Proof
{
	#define MAX_INPUT_BUFFER_LENGTH 256

	enum class ContentBrowserAction
	{
		None = 0,
		Refresh = BIT(0),
		ClearSelections = BIT(1),
		Selected = BIT(2),
		Deselected = BIT(3),
		Hovered = BIT(4),
		Renamed = BIT(5),
		OpenDeleteDialogue = BIT(6),
		SelectToHere = BIT(7),
		Moved = BIT(8),
		ShowInExplorer = BIT(9),
		OpenExternal = BIT(10),
		Reload = BIT(11),
		Copy = BIT(12),
		Duplicate = BIT(13),
		StartRenaming = BIT(14),
		Activated = BIT(15)
	};

	struct CBItemActionResult
	{
		uint16_t Field = 0;

		void Set(ContentBrowserAction flag, bool value)
		{
			if (value)
				Field |= (uint16_t)flag;
			else
				Field &= ~(uint16_t)flag;
		}

		bool IsSet(ContentBrowserAction flag) const { return (uint16_t)flag & Field; }
	};
	class ContentBrowserItem : public RefCounted
	{
	public:
		enum class ItemType : uint16_t
		{
			Directory, Asset
		};
	public:
		ContentBrowserItem(ItemType type, AssetID id, const std::string& name, const Count<Texture2D>& icon);
		virtual ~ContentBrowserItem() {}

		void OnRenderBegin();
		CBItemActionResult OnRender();
		void OnRenderEnd();

		virtual void Delete() {}
		virtual bool Move(const std::filesystem::path& destination) { return false; }

		AssetID GetID() const { return m_ID; }
		ItemType GetType() const { return m_Type; }
		const std::string& GetName() const { return m_FileName; }

		const Count<Texture2D>& GetIcon() const { return m_Icon; }

		void StartRenaming();
		void StopRenaming();
		bool IsRenaming() const { return m_IsRenaming; }

		void Rename(const std::string& newName);
		void SetDisplayNameFromFileName();

	private:
		virtual void OnRenamed(const std::string& newName) { m_FileName = newName; }
		virtual void RenderCustomContextItems() {}
		virtual void UpdateDrop(CBItemActionResult& actionResult) {}

		void OnContextMenuOpen(CBItemActionResult& actionResult);

	protected:
		ItemType m_Type;
		AssetID m_ID;
		std::string m_DisplayName;
		std::string m_FileName;
		Count<Texture2D> m_Icon;

		bool m_IsRenaming = false;
		bool m_IsDragging = false;
		bool m_JustSelected = false;

	private:
		friend class ContentBrowserPanel;
	};
	struct DirectoryInfo : public RefCounted
	{
		UUID Handle;
		Count<DirectoryInfo> Parent = nullptr;
		std::filesystem::path FilePath;
		std::vector<AssetID> Assets;
		std::unordered_map<UUID, Count< DirectoryInfo>> SubDirectories;
	};
	class ContentBrowserDirectory : public ContentBrowserItem
	{
	public:
		ContentBrowserDirectory(const Count<DirectoryInfo>& directoryInfo);
		virtual ~ContentBrowserDirectory();

		Count<DirectoryInfo>& GetDirectoryInfo() { return m_DirectoryInfo; }

		virtual void Delete() override;
		virtual bool Move(const std::filesystem::path& destination) override;

	private:
		virtual void OnRenamed(const std::string& newName) override;
		virtual void UpdateDrop(CBItemActionResult& actionResult) override;

		void UpdateDirectoryPath(Count<DirectoryInfo> directoryInfo, const std::filesystem::path& newParentPath, const std::filesystem::path& newName);

	private:
		Count<DirectoryInfo> m_DirectoryInfo;
	};

	class ContentBrowserAsset : public ContentBrowserItem
	{
	public:
		ContentBrowserAsset(const AssetInfo& assetInfo, const Count<Texture2D>& icon);
		virtual ~ContentBrowserAsset();

		const AssetInfo& GetAssetInfo() const { return m_AssetInfo; }

		virtual void Delete() override;
		virtual bool Move(const std::filesystem::path& destination) override;

	private:
		virtual void OnRenamed(const std::string& newName) override;

	private:
		AssetInfo m_AssetInfo;
	};
}