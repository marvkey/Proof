#pragma once
#include "Proof/ProofCore.h"
namespace Proof
{
	class EditorResources
	{
	public:
		//refrences to icons https://www.flaticon.com/

		static inline Count<class Texture2D> DeleteIcon;
		static inline Count<class Texture2D> AddIcon;
		static inline Count<class Texture2D> InsertIcon;

		static inline Count<class Texture2D> CheckerBoardWhiteGrey;
		static inline Count<class Texture2D> FolderIcon;
		static inline Count<class Texture2D> FileIcon;
		static inline Count<class Texture2D> BackArrowIcon;
		static inline Count<class Texture2D> FowardArrowIcon;

		static inline Count<class Texture2D> AnimationThumbnailIcon;
		static inline Count<class Texture2D> AnimationControllerThumbnailIcon;
		static inline Count<class Texture2D> AudioThumbnailIcon;
		static inline Count<class Texture2D> AudioControllerThumbnailIcon;
		static inline Count<class Texture2D> AudioMixerThumbnailIcon;
		static inline Count<class Texture2D> FontThumbnailIcon;
		static inline Count<class Texture2D> InputActionThumbnailIcon;
		static inline Count<class Texture2D> InputBindingContextThumbnailIcon;
		static inline Count<class Texture2D> MeshColliderThumbnailIcon;
		static inline Count<class Texture2D> ParticleSystemThumbnailIcon;
		static inline Count<class Texture2D> ParticleEmitterThumbnailIcon;
		static inline Count<class Texture2D> PMatThumbnailIcon;
		static inline Count<class Texture2D> PrefabThumbnailIcon;
		static inline Count<class Texture2D> ScriptFileThumbnailIcon;
		static inline Count<class Texture2D> SkeletonThumbnailIcon;
		static inline Count<class Texture2D> UIThumbnailIcon;
		static inline Count<class Texture2D> WorldThumbnailIcon;

		//light
		static inline Count<Texture2D> SkyLightIcon;
		static inline Count<Texture2D> DirectionalLightIcon;
		static inline Count<Texture2D> PointLightIcon;
		static inline Count<Texture2D> SpotLightIcon;
		//viewport
		static inline Count<Texture2D> CameraIcon;
		static inline Count<Texture2D> DropdownIcon;
		static inline Count<Texture2D> LocalSpaceIcon;
		static inline Count<Texture2D> WorldSpaceIcon;
		static inline Count<Texture2D> TranslationIcon;
		static inline Count<Texture2D> RotationIcon;
		static inline Count<Texture2D> ScaleIcon;
		static inline Count<Texture2D> UniversalTransformIcon;
		static inline Count<Texture2D> PointerIcon;
		static inline Count<Texture2D> ViewIcon;
		static inline Count<Texture2D> PlayerStartIcon;

	private:
		static void Init();
		static void Unizilize();
		friend class Editore3D;
	};
}
