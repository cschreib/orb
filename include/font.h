#ifndef FONT_H
#define FONT_H

#include "utils.h"
#include "point.h"

#include <SFML/Graphics.hpp>

struct CharacterInfo
{
    uint_t uiCodePoint = 0;

    float fU1 = 0.0, fV1 = 0.0;
    float fU2 = 0.0, fV2 = 0.0;

    std::map< uchar_t, Point<float> > lKerningInfo;
};

/// Manages font creation
class Font
{
public :

    Font(const std::string& sFontFile, const uint_t& uiSize);

    ~Font();

    std::array<float,4> GetCharacterUVs(const uchar_t& ucChar) const;

    float GetCharacterWidth(const uchar_t& ucChar) const;

    float GetCharacterKerning(const uchar_t& uiChar1, const uchar_t& uiChar2) const;

    sf::Texture* GetTexture();

    float GetTextureWidth() const;

    float GetTextureHeight() const;

    static const std::string CLASS_NAME;

private :

    std::map<uchar_t, CharacterInfo> lCharacterList_;

    sf::Texture mTexture_;
    float fTextureWidth_ = 0.0;
    float fTextureHeight_ = 0.0;

};

#endif
