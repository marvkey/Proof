#pragma once
#include "Proof/Core/Core.h"
#include "Proof/ProofCore.h"

namespace Proof{
	class AssetThumbnailGenerator {
	public:
		static void GenerateThumbnail(UUID ID);

		static bool HasThumbnail(UUID ID) {
			return AssetThumbnails.find(ID) != AssetThumbnails.end();
		}

		static uint32_t GetThumbnail(UUID ID) {
			PF_CORE_ASSERT(AssetThumbnails.find(ID) != AssetThumbnails.end(), "Not a valid ASSET ID");
			return AssetThumbnails.at(ID);
		}
	private:
		AssetThumbnailGenerator();
		virtual ~AssetThumbnailGenerator();
		void CreateThumbnail(UUID ID);
		Count<World> m_World = nullptr;
		Count<WorldRenderer> m_WorldRenderer = nullptr;
		static std::unordered_map<UUID, uint32_t> AssetThumbnails;
	};
}
