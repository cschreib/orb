#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "utils.h"
#include "manager.h"

#include <SFML/Graphics.hpp>

class TextureManager : public Manager<TextureManager>
{
friend class Manager<TextureManager>;
public :

    sf::Texture* LoadTexture(const std::string& sFile);

    static const std::string CLASS_NAME;

protected:

    TextureManager();
    ~TextureManager();

private:

    std::map< std::string, std::unique_ptr<sf::Texture> > lTextureList_;

};

#endif
