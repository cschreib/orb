#include "texturemanager.h"

const std::string TextureManager::CLASS_NAME = "TextureManager";

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

sf::Texture* TextureManager::LoadTexture( const std::string& sFile )
{
    auto iter = lTextureList_.find(sFile);
    if (iter == lTextureList_.end())
    {
        iter = lTextureList_.insert(std::make_pair<std::string, std::unique_ptr<sf::Texture>>(
            std::string(sFile), std::unique_ptr<sf::Texture>(new sf::Texture())
        )).first;

        if (!iter->second->loadFromFile(sFile))
        {
            throw std::runtime_error(CLASS_NAME+" : Unable to load Texture : "+sFile);
        }
    }

    return iter->second.get();
}
