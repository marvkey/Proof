#pragma once
#include "Proof/ProofCore.h"
namespace Proof
{
	class EditorResources
	{
	public:
		static inline Count<class Texture2D> CheckerBoardWhiteGrey;
	private:
		static void Init();
		static void Unizilize();
		friend class Editore3D;
	};
}
