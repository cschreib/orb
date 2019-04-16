#include "font.h"
#include "log.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include <SFML/Graphics.hpp>

const std::string Font::CLASS_NAME = "Font";

Font::Font( const std::string& sFontFile, const uint_t& uiSize )
{
    // NOTE : code inspired from Ogre::Font, from the OGRE3D graphics engine
    // http://www.ogre3d.org

    FT_Library mFT;
    if (FT_Init_FreeType(&mFT))
        throw std::runtime_error(CLASS_NAME+" : Error initializing FreeType !");

    FT_Face mFace;

    // Add some space between letters to prevent artifacts
    uint_t uiSpacing = 5;

    if (FT_New_Face( mFT, sFontFile.c_str(), 0, &mFace))
    {
        throw std::runtime_error(CLASS_NAME+" : Error loading font : \""+sFontFile+"\".\n"
            "Couldn't load face."
        );
    }

    if (FT_Set_Pixel_Sizes(mFace, uiSize, 0))
    {
        throw std::runtime_error(CLASS_NAME+" : Error loading font : \""+sFontFile+"\".\n"
            "Couldn't set font size."
        );
    }

    int iMaxHeight = 0, iMaxWidth = 0, iMaxBearingY = 0;

    // Calculate maximum width, height and bearing
    for (uint_t cp = 33; cp <= 255; ++cp)
    {
        FT_Load_Char(mFace, cp, FT_LOAD_RENDER);

        int iCharHeight = 2*(mFace->glyph->bitmap.rows << 6) - mFace->glyph->metrics.horiBearingY;
        if (iCharHeight > iMaxHeight)
            iMaxHeight = iCharHeight;

        if (mFace->glyph->metrics.horiBearingY > iMaxBearingY)
            iMaxBearingY = mFace->glyph->metrics.horiBearingY;

        int iCharWidth = (mFace->glyph->advance.x >> 6) + (mFace->glyph->metrics.horiBearingX >> 6);
        if (iCharWidth > iMaxWidth)
            iMaxWidth = iCharWidth;
    }

    iMaxBearingY = iMaxBearingY >> 6;

    // Calculate the size of the texture
    std::size_t uiTexSize = (iMaxWidth + uiSpacing)*((iMaxHeight >> 6) + uiSpacing)*(255-33);

    uint_t uiTexSide = static_cast<uint_t>(::sqrt(uiTexSize));
    uiTexSide += std::max(iMaxWidth, iMaxHeight>>6);

    // Round up to nearest power of two
    uint_t i = 1;
    while (uiTexSide > i)
        i += i;
    uiTexSide = i;

    std::size_t uiFinalWidth, uiFinalHeight;
    if (uiTexSide*uiTexSide/2 >= uiTexSize)
    {
        uiFinalHeight = uiTexSide/2;
    }
    else
    {
        uiFinalHeight = uiTexSide;
    }
    uiFinalWidth = uiTexSide;

    fTextureWidth_ = static_cast<float>(uiFinalWidth);
    fTextureHeight_ = static_cast<float>(uiFinalHeight);

    sf::Image mImage;
    mImage.create(uiFinalWidth, uiFinalHeight, sf::Color(255, 255, 255, 0));

    std::size_t l = 0, m = 0;
    CharacterInfo mCI;
    for (uint_t cp = 33; cp <= 255; ++cp)
    {
        mCI.uiCodePoint = cp;

        if (FT_Load_Char(mFace, cp, FT_LOAD_RENDER))
        {
            Warning(CLASS_NAME, "Can't load character ", uchar_t(uint_t(cp)), " in font \""+sFontFile+"\".");
            continue;
        }

        FT_Int iAdvance = (mFace->glyph->advance.x >> 6) + (mFace->glyph->metrics.horiBearingX >> 6);

        uchar_t* sBuffer = mFace->glyph->bitmap.buffer;

        if (!sBuffer)
            continue;

        int iYBearing = iMaxBearingY - (mFace->glyph->metrics.horiBearingY >> 6);

        for (int j = 0; j < int(mFace->glyph->bitmap.rows); ++j)
        {
            for (int k = 0; k < int(mFace->glyph->bitmap.width); ++k, ++sBuffer)
            {
                mImage.setPixel(l + k, j + m + iYBearing, sf::Color(255, 255, 255, *sBuffer));
            }
        }

        /*if (FT_HAS_KERNING(mFace))
        {
            FT_Vector kern;
            unsigned int prev, next;
            for (uint_t cp2 = 33; cp2 <= 255; ++cp2)
            {
                prev = FT_Get_Char_Index(mFace, cp);
                next = FT_Get_Char_Index(mFace, cp2);
                if (!FT_Get_Kerning(mFace, prev, next, FT_KERNING_DEFAULT, &kern))
                {
                    mCI.lKerningInfo[cp2] = Point<float>(
                        (kern.x >> 6),
                        (kern.y >> 6)
                    );
                }
            }
        }*/

        mCI.fU1 = l/static_cast<float>(uiFinalWidth);
        mCI.fV1 = m/static_cast<float>(uiFinalHeight);
        mCI.fU2 = (l + (mFace->glyph->advance.x >> 6))/static_cast<float>(uiFinalWidth);
        mCI.fV2 = (m + (iMaxHeight >> 6))/static_cast<float>(uiFinalHeight);

        lCharacterList_[cp] = mCI;

        // Advance a column
        l += (iAdvance + uiSpacing);

        // If at end of row
        if (l + iAdvance > uiFinalWidth - 1)
        {
            m += (iMaxHeight >> 6) + uiSpacing;
            l = 0;
        }
    }

    FT_Done_FreeType(mFT);

    mTexture_.loadFromImage(mImage);
}

Font::~Font()
{
}

std::array<float,4> Font::GetCharacterUVs( const uchar_t& ucChar ) const
{
    const CharacterInfo& mChar = lCharacterList_.find(ucChar)->second;
    std::array<float,4> mArray;
    mArray[0] = mChar.fU1;
    mArray[1] = mChar.fV1;
    mArray[2] = mChar.fU2;
    mArray[3] = mChar.fV2;
    return mArray;
}

float Font::GetCharacterWidth( const uchar_t& ucChar ) const
{
    const CharacterInfo& mChar = lCharacterList_.find(ucChar)->second;
    return (mChar.fU2 - mChar.fU1)*fTextureWidth_;
}

float Font::GetCharacterKerning( const uchar_t& ucChar1, const uchar_t& ucChar2 ) const
{
    return lCharacterList_.find(ucChar1)->second.lKerningInfo.find(ucChar2)->second.X();
}

float Font::GetTextureWidth() const
{
    return fTextureWidth_;
}

float Font::GetTextureHeight() const
{
    return fTextureHeight_;
}

sf::Texture* Font::GetTexture()
{
    return &mTexture_;
}
