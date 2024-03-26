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

	private:
		static void Init();
		static void Unizilize();
		friend class Editore3D;
	};
}
