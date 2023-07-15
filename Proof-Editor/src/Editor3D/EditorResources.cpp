#include "EditorResources.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Core/Buffer.h"
namespace Proof {
	void EditorResources::Init()
	{
		{
			int textureSize = 10;
			Buffer buffer(textureSize * textureSize);
			 // Generate the checkerboard pattern
			for (int y = 0; y < textureSize; ++y)
			{
				for (int x = 0; x < textureSize; ++x)
				{
					if ((x + y) % 2 == 0)
					{
						// White color (hexadecimal: 0xFFFFFFFF)
						buffer.Get()[y * textureSize + x] = 0xFFFFFFFF;
					}
					else
					{
							 // Gray color (hexadecimal: 0xFF808080)
						buffer.Get()[y * textureSize + x] = 0xFF808080;
					}
				}
			}
			CheckerBoardWhiteGrey = Texture2D::Create(buffer.Get(), TextureConfiguration("CheckerBoardTextureWhiteAndGrey", ImageFormat::RGBA, textureSize, textureSize));
			
			buffer.Release();
		}
	}

	void EditorResources::Unizilize()
	{
		CheckerBoardWhiteGrey = nullptr;
	}
}
