#pragma once
#include "Proof/ProofCore.h"
namespace Proof
{
	class EditorResources
	{
	public:
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
	private:
		static void Init();
		static void Unizilize();
		friend class Editore3D;
	};
}
