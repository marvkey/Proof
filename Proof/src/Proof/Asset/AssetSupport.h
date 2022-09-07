#pragma once
#include <vector>
#include <string>
namespace Proof
{
	// more formats come in the future
	static const std::vector<std::string> MeshSourceFormats = 
	{
		"fbx",
		"obj",
		"blend",
		"gltf",
		"glb",
		"blend",
	};

	bool MeshHasFormat(const std::string& format) {
		for (const auto& a : MeshSourceFormats) {
			if (a == format)
				return true;
		}
		return false;
	}
	// more formats to come in the future
	static const std::vector < std::string> TextureSourceFormats =
	{
		"jpeg",
		"png",
		"tga",
		"big",
		"hdr"
	};

	bool TextureHasFormat(const std::string& format) {
		for (const auto& a : TextureSourceFormats) {
			if (a == format)
				return true;
		}
		return false;
	}
}
