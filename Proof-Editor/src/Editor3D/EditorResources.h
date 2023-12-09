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
	private:
		static void Init();
		static void Unizilize();
		friend class Editore3D;
	};
}
