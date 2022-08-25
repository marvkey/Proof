#pragma once
#include "Proof/Core/Core.h"
#include "Proof/ProofCore.h"

namespace Proof{
	class AssetThumbnailGenerator {
	public:
		static void GenerateThumbnail(AssetID ID);

		static bool HasThumbnail(AssetID ID) {
			return AssetThumbnails.find(ID) != AssetThumbnails.end();
		}

		static uint32_t GetThumbnail(AssetID ID) {
			PF_CORE_ASSERT(AssetThumbnails.find(ID) != AssetThumbnails.end(), "Not a valid ASSET ID");
			return AssetThumbnails.at(ID);
		}
	private:
		AssetThumbnailGenerator();
		virtual ~AssetThumbnailGenerator();
		void CreateThumbnail(AssetID ID);
		Count<World> m_World = nullptr;
		Count<WorldRenderer> m_WorldRenderer = nullptr;
		static std::unordered_map<AssetID, uint32_t> AssetThumbnails;
	};
}
