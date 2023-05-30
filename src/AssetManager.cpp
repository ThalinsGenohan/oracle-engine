//
// Created by Thalins on 11/25/22.
//

#include "AssetManager.h"

#include <fstream>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

#include "Game.h"
#include "stb_image.h"
#include "Tilemap.h"
#include "Tileset.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture2D.h"

namespace oter
{

// Helper functions
YAML::Node ReadYAML(const std::string& filepath)
{
	YAML::Node yaml;
	try
	{
		yaml = YAML::LoadFile(filepath);
	}
	catch (std::exception& e)
	{
		spdlog::error("Failed to read YAML file '{}': {}", filepath, e.what());
	}
	return yaml;
}

std::vector<u8>& LoadTextureAtlasFromFiles(const std::vector<std::string>& filepaths, v2u& size);

template <>
Shader* AssetManager<Shader>::LoadFromFile(const std::string& filepath)
{
	std::string shaderCode = "";
	try
	{
		std::ifstream fileStream(filepath);
		char          c;
		while (fileStream.get(c))
			shaderCode += c;
		fileStream.close();
	} catch (std::exception& e)
	{
		spdlog::error("Failed to read shader file '{}': {}", filepath, e.what());
		return nullptr;
	}
	const size_t pos = shaderCode.find('\n');

	std::string vertCode = shaderCode;
	std::string geomCode = "";
	std::string fragCode = shaderCode;

	vertCode.insert(pos, "\n#define VERT 1");
	fragCode.insert(pos, "\n#define FRAG 1");

	if (shaderCode.find("#elif GEOM == 1") != std::string::npos)
	{
		geomCode = shaderCode;
		geomCode.insert(pos, "\n#define GEOM 1");
	}

	auto* shader = new Shader();
	shader->Compile(vertCode, geomCode, fragCode);
	return shader;
}

void LoadFrames(const std::string& filepath)
{
	YAML::Node yaml = ReadYAML(filepath);
	if (yaml.IsNull())
		return;

	YAML::Node frameGrid = yaml["frameGrid"];
	v2u size = frameGrid["size"].as<v2u>();
	v2u dimensions = frameGrid["dimensions"].as<v2u>();
	YAML::Node names = frameGrid["names"];
	
}

template <>
Texture2D* AssetManager<Texture2D>::LoadFromFile(const std::string& filepath)
{
	YAML::Node yaml = ReadYAML(filepath);
	if (yaml.IsNull())
		return nullptr;

	auto path = yaml["texturePath"].as<std::string>();

	i32 width;
	i32 height;
	i32 nrChannels;
	u8* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);

	if (data == nullptr)
	{
		spdlog::error("Could not load sprite '{}' texture '{}': {}", filepath, path, stbi_failure_reason());
		return nullptr;
	}

	auto frameSize = yaml["frameSize"].as<v2u>();

	std::unordered_map<std::string, Texture2D::Animation> animations;
	for (YAML::detail::iterator_value iteratorValues : yaml["animations"])
	{
		for (u32 d = 0; d < yaml["directions"].size(); d++)
		{
			std::string name = iteratorValues.first.as<std::string>() + "-" + yaml["directions"][d].as<std::string>();
			YAML::Node& animNode = iteratorValues.second;
			for (YAML::const_iterator it = animNode.begin(); it != animNode.end(); ++it)
			{
				auto frameIndex    = (*it)["pos"].as<u32>();
				auto frameMs       = (*it)["ms"].as<f32>();
				auto framePosition = v2u(frameIndex * frameSize.X, d * frameSize.Y);

				animations[name].push_back({ framePosition, frameMs });
			}
		}
	}

	auto texture = new Texture2D();
	texture->Generate(static_cast<u32>(width), static_cast<u32>(height), data);
	texture->SetFrameSize(frameSize);
	texture->SetAnimations(animations);
	stbi_image_free(data);
	return texture;
}

template <>
Tilemap* AssetManager<Tilemap>::LoadFromFile(const std::string& filepath)
{
	u16              width  = 0x0000;
	u16              height = 0x0000;
	std::vector<u16> tiles;

	try
	{
		std::ifstream mapFileStream(filepath, std::ifstream::in | std::ifstream::binary);
		mapFileStream.read(reinterpret_cast<char*>(&width), sizeof width);
		mapFileStream.read(reinterpret_cast<char*>(&height), sizeof height);
		tiles.resize(static_cast<u64>(width) * static_cast<u64>(height));
		mapFileStream.read(reinterpret_cast<char*>(tiles.data()),
		                   static_cast<u64>(width) * static_cast<u64>(height) * sizeof tiles[0]
		);
		mapFileStream.close();
	} catch (std::exception& e)
	{
		spdlog::error("Failed to read tilemap file '{}': {}", filepath, e.what());
		return nullptr;
	}

	const auto tilemap = new Tilemap();
	tilemap->SetMapSize(width, height);
	tilemap->SetTiles(tiles.data());
	return tilemap;
}

template <>
Tileset* AssetManager<Tileset>::LoadFromFile(const std::string& filepath)
{
	YAML::Node yaml = ReadYAML(filepath);
	if (yaml.IsNull())
		return nullptr;

	v2u tileSize = v2u(yaml["tileSize"]["x"].as<u32>(), yaml["tileSize"]["y"].as<u32>());

	std::vector<std::string> setNames;
	std::vector<std::string> setFilepaths;
	for (YAML::detail::iterator_value iteratorValues : yaml["tilesets"])
	{
		auto        name = iteratorValues.first.as<std::string>();
		YAML::Node& node = iteratorValues.second;
		auto        path = node["filepath"].as<std::string>();

		setNames.push_back(name);
		setFilepaths.push_back(path);
	}

	v2u              atlasSize;
	std::vector<u8>& atlas = LoadTextureAtlasFromFiles(setFilepaths, atlasSize);

	auto tileset = new Tileset();
	tileset->Generate(tileSize, setNames, atlas, atlasSize);
	delete &atlas;
	return tileset;
}

std::vector<u8>& LoadTextureAtlasFromFiles(const std::vector<std::string>& filepaths, v2u& size)
{
	std::vector<std::vector<u8>> sets;
	std::vector<u32>             widths;
	u32                          maxWidth    = 0;
	u32                          totalHeight = 0;
	i32                          nrChannels  = 0;

	for (auto filepathIter = filepaths.begin(); filepathIter < filepaths.end(); ++filepathIter)
	{
		i32 width;
		i32 height;
		u8* data = stbi_load((*filepathIter).c_str(), &width, &height, &nrChannels, 4);

		if (data == nullptr)
		{
			spdlog::error("Could not load texture '{}' for atlas: {}", *filepathIter, stbi_failure_reason());
			continue;
		}

		sets.push_back({});
		for (i32 i = 0; i < width * height * nrChannels; i++)
		{
			sets.back().push_back(data[i]);
		}
		widths.push_back(width);

		maxWidth = std::max(maxWidth, static_cast<u32>(width));
		totalHeight += height;

		stbi_image_free(data);
	}

	std::vector<u8>* vec = new std::vector<u8>();
	for (u32 i = 0; i < sets.size(); i++)
	{
		std::vector<u8>& set   = sets[i];
		const u32&       width = widths[i];

		if (width < maxWidth)
		{
			const u32 height = totalHeight / filepaths.size();
			for (u32 y = 0; y < height; y++)
			{
				const u32 offset = (maxWidth * y + width) * 4;
				const u32 count  = (maxWidth - width) * 4;
				set.insert(set.begin() + offset, count, 0);
			}
		}

		vec->insert(vec->end(), set.begin(), set.end());
	}

	size.X = maxWidth;
	size.Y = totalHeight;

	return *vec;
}
}
