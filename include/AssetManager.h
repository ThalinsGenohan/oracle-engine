//
// Created by Thalins on 11/24/22.
//

#ifndef ORACLES_ASSETMANAGER_H
#define ORACLES_ASSETMANAGER_H

#include <string>
#include <unordered_map>

namespace oracle
{
template <typename T>
class AssetManager
{
public:
	AssetManager() {}
	~AssetManager() {}

	T& Load(const std::string& name, const std::string& filepath)
	{
		if (this->_assets.contains(name))
			return *this->_assets[name];

		this->_assets[name] = this->LoadFromFile(filepath);

		return *this->_assets[name];
	}

	T& Get(const std::string& name) const
	{
		return *this->_assets.at(name);
	}

	void Unload(const std::string& name)
	{
		if (!this->_assets.contains(name))
			return;

		this->_assets[name].Unload();
		this->_assets[name] = nullptr;
	}

private:
	std::unordered_map<std::string, T*> _assets = {};

	T* LoadFromFile(const std::string& filepath);
};
}
#endif //ORACLES_ASSETMANAGER_H
