#pragma once
#include "Proof/Core/Core.h"
namespace Proof {
	struct AssetInfo;
	class  AssetSerializer {
	public:
		virtual void Save(const AssetInfo& data, const Count<class Asset>& asset)const = 0;
		virtual Count<class Asset>TryLoadAsset(const AssetInfo& data)const = 0;
	protected:
		static void SetID(const AssetInfo& data, const Count<class Asset>& asset);
	};

	class TextureAssetSerializer : public  AssetSerializer {
	public:
		virtual void Save(const AssetInfo& data, const Count<class Asset>& asset)const;
		virtual Count<class Asset>TryLoadAsset(const AssetInfo& data)const;
	};

	class MaterialAssetSerializer : public  AssetSerializer {
	public:
		virtual void Save(const AssetInfo& data, const Count<class Asset>& asset)const;
		virtual Count<class Asset>TryLoadAsset(const AssetInfo& data)const;
	};

	class PhysicsMaterialAssetSerializer : public  AssetSerializer {
	public:
		virtual void Save(const AssetInfo& data, const Count<class Asset>& asset)const;
		virtual Count<class Asset>TryLoadAsset(const AssetInfo& data)const;
	};

	class MeshAssetSerializer : public  AssetSerializer {
	public:
		virtual void Save(const AssetInfo& data, const Count<class Asset>& asset)const;
		virtual Count<class Asset>TryLoadAsset(const AssetInfo& data)const;
	};

	class DynamicMeshAssetSerializer : public  AssetSerializer {
	public:
		virtual void Save(const AssetInfo& data, const Count<class Asset>& asset)const;
		virtual Count<class Asset>TryLoadAsset(const AssetInfo& data)const;
	};
	class MeshSourceAssetSerializer : public  AssetSerializer {
	public:
		virtual void Save(const AssetInfo& data, const Count<class Asset>& asset)const;
		virtual Count<class Asset>TryLoadAsset(const AssetInfo& data)const;
	};


	class PrefabAssetSerilizer : public  AssetSerializer {
	public:
		virtual void Save(const AssetInfo& data, const Count<class Asset>& asset)const;
		virtual Count<class Asset>TryLoadAsset(const AssetInfo& data)const;
	};

	class UIPanelAssetSerilizer : public AssetSerializer {
	public:
		virtual void Save(const AssetInfo& data, const Count<class Asset>& asset)const;
		virtual Count<class Asset>TryLoadAsset(const AssetInfo& data)const;
	};

	class ParticleSystemSerilizer : public AssetSerializer {
	public:
		virtual void Save(const AssetInfo& data, const Count<class Asset>& asset)const;
		virtual Count<class Asset> TryLoadAsset(const AssetInfo& data)const;
	};

	class AudioAssetSerilizer : public AssetSerializer {
	public:
		virtual void Save(const AssetInfo& data, const Count<class Asset>& asset)const;
		virtual Count<class Asset> TryLoadAsset(const AssetInfo& data)const;
	};

	class MeshColliderAssetSerilizer : public AssetSerializer {
	public:
		virtual void Save(const AssetInfo& data, const Count<class Asset>& asset)const;
		virtual Count<class Asset> TryLoadAsset(const AssetInfo& data)const;
	};

	class ScriptFileAssetSerilizer : public AssetSerializer
	{
	public:
		virtual void Save(const AssetInfo& data, const Count<class Asset>& asset)const;
		virtual Count<class Asset> TryLoadAsset(const AssetInfo& data)const;
	};

	class WorldAssetSerializer : public AssetSerializer
	{
	public:
		virtual void Save(const AssetInfo& data, const Count<class Asset>& asset)const;
		virtual Count<class Asset> TryLoadAsset(const AssetInfo& data)const;
	};

	class InputActionSerealizer : public AssetSerializer
	{

	public:
		virtual void Save(const AssetInfo& data, const Count<class Asset>& asset)const;
		virtual Count<class Asset> TryLoadAsset(const AssetInfo& data)const;
	};
}