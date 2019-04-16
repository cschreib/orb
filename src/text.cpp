#include "text.h"
#include "fontmanager.h"
#include "sprite.h"
#include "font.h"
#include "application.h"
#include "log.h"

const std::string Text::CLASS_NAME = "Text";

Text::Text()
{
    fBoxW_ = fBoxH_ = std::numeric_limits<float>::infinity();
    mAlign_ = ALIGN_LEFT;
    mVertAlign_ = ALIGN_MIDDLE;
    fLineSpacing_ = 1.5f;
    fTracking_ = 0.0f;
    mColor_ = Color(255, 255, 255);
    fX_ = fY_ = std::numeric_limits<float>::infinity();
}

Text::Text( const std::string& sFileName, float fSize )
{
    fBoxW_ = fBoxH_ = std::numeric_limits<float>::infinity();
    mAlign_ = ALIGN_LEFT;
    mVertAlign_ = ALIGN_MIDDLE;
    fLineSpacing_ = 1.5f;
    fTracking_ = 0.0f;
    sFileName_ = sFileName;
    fSize_ = fSize;
    mColor_ = Color(255, 255, 255);
    fX_ = fY_ = std::numeric_limits<float>::infinity();
    pFont_ = FontManager::GetSingleton()->GetFont(sFileName_, uint_t(fSize_));
    if (pFont_)
    {
        bReady_ = true;

        fSpaceWidth_ = GetCharacterWidth((uint_t)'0')*0.5f;
    }
    else
    {
        Error(CLASS_NAME,
            "Error initializing \""+sFileName+"\" (size : ", fSize, ")."
        );
    }
}

Text::~Text()
{
}

const std::string& Text::GetFontName() const
{
    return sFileName_;
}

float Text::GetFontSize() const
{
    return fSize_;
}

float Text::GetLineHeight() const
{
    return fSize_;
}

void Text::SetText( const std::string& sText )
{
    if (sText_ != sText)
    {
        sText_ = sText;
        bUpdateCache_ = true;
    }
}

const std::string& Text::GetText() const
{
    return sText_;
}

void Text::SetColor( const Color& mColor, bool bForceColor )
{
    if (mColor_ != mColor || bForceColor_ != bForceColor)
    {
        mColor_ = mColor;
        bForceColor_ = bForceColor;
        bUpdateQuads_ = true;
    }
}

const Color& Text::GetColor() const
{
    return mColor_;
}

void Text::SetDimensions( float fW, float fH )
{
    if (((fBoxW_ != fW) && (!std::isnan(fBoxW_) && !std::isnan(fW))) ||
        ((fBoxH_ != fH) && (!std::isnan(fBoxH_) && !std::isnan(fH))))
    {
        fBoxW_ = fW; fBoxH_ = fH;
        bUpdateCache_ = true;
    }
}

void Text::SetBoxWidth( float fBoxW )
{
    if ( (fBoxW_ != fBoxW) && (!std::isnan(fBoxW_) && !std::isnan(fBoxW)) )
    {
        fBoxW_ = fBoxW;
        bUpdateCache_ = true;
    }
}

void Text::SetBoxHeight( float fBoxH )
{
    if ( (fBoxH_ != fBoxH) && (!std::isnan(fBoxH_) && !std::isnan(fBoxH)) )
    {
        fBoxH_ = fBoxH;
        bUpdateCache_ = true;
    }
}

float Text::GetWidth()
{
    Update();

    return fW_;
}

float Text::GetHeight()
{
    Update();

    return fH_;
}

float Text::GetBoxWidth() const
{
    return fBoxW_;
}

float Text::GetBoxHeight() const
{
    return fBoxH_;
}

float Text::GetTextWidth() const
{
    float fWidth = 0.0;
    float fMaxWidth = -std::numeric_limits<float>::infinity();

    if (bReady_)
    {
        std::string::const_iterator iterChar, iterNext;
        for (iterChar = sText_.begin(); iterChar != sText_.end(); ++iterChar)
        {
            iterNext = iterChar + 1;
            if (*iterChar == ' ')
                fWidth += fSpaceWidth_;
            else if (*iterChar == '\n')
            {
                if (fWidth > fMaxWidth)
                    fMaxWidth = fWidth;

                fWidth = 0.0f;
            }
            else
            {
                fWidth += GetCharacterWidth((uint_t)*iterChar) + fTracking_;
                if (iterNext != sText_.end())
                {
                    if (*iterNext != ' ' && *iterNext != '\n')
                        fWidth += GetCharacterKerning((uint_t)*iterChar, (uint_t)*iterNext);
                }
            }
        }
    }

    return fWidth;
}

float Text::GetTextHeight() const
{
    float fHeight = 0.0;

    if (bReady_)
    {
        fHeight = GetLineHeight()*(float(CountOccurrences(sText_, "\n"))*GetLineHeight()*fLineSpacing_ + 1.0f);
    }

    return fHeight;
}

float Text::GetStringWidth( const std::string& sString ) const
{
    float fWidth = 0.0;
    float fMaxWidth = -std::numeric_limits<float>::infinity();
    if (bReady_)
    {
        std::string::const_iterator iterChar, iterNext;
        for (iterChar = sText_.begin(); iterChar != sText_.end(); ++iterChar)
        {
            iterNext = iterChar + 1;
            if (*iterChar == ' ')
                fWidth += fSpaceWidth_;
            else if (*iterChar == '\n')
            {
                if (fWidth > fMaxWidth)
                    fMaxWidth = fWidth;
                fWidth = 0.0f;
            }
            else
            {
                fWidth += GetCharacterWidth((uint_t)*iterChar) + fTracking_;
                if (iterNext != sString.end())
                {
                    if (*iterNext != ' ' && *iterNext != '\n')
                        fWidth += GetCharacterKerning((uint_t)*iterChar, (uint_t)*iterNext);
                }
            }
        }
    }

    return fWidth;
}

float Text::GetCharacterWidth( const uint_t& uiChar ) const
{
    if (bReady_)
    {
        return pFont_->GetCharacterWidth(uchar_t(uiChar));
    }
    else
        return 0.0f;
}

float Text::GetCharacterKerning( const uint_t& uiChar1, const uint_t& uiChar2 ) const
{
    return pFont_->GetCharacterKerning(uchar_t(uiChar1), uchar_t(uiChar2));
}

void Text::SetAlignment( const Text::Alignment& mAlign )
{
    if (mAlign_ != mAlign)
    {
        mAlign_ = mAlign;
        bUpdateCache_ = true;
    }
}

void Text::SetVerticalAlignment( const Text::VerticalAlignment& mVertAlign )
{
    if (mVertAlign_ != mVertAlign)
    {
        mVertAlign_ = mVertAlign;
        bUpdateCache_ = true;
    }
}

const Text::Alignment& Text::GetAlignment() const
{
    return mAlign_;
}

const Text::VerticalAlignment& Text::GetVerticalAlignment() const
{
    return mVertAlign_;
}

void Text::SetTracking( float fTracking )
{
    if (fTracking_ != fTracking)
    {
        fTracking_ = fTracking;
        bUpdateCache_ = true;
    }
}

float Text::GetTracking() const
{
    return fTracking_;
}

void Text::SetLineSpacing( float fLineSpacing )
{
    if (fLineSpacing_ != fLineSpacing)
    {
        fLineSpacing_ = fLineSpacing;
        bUpdateCache_ = true;
    }
}

float Text::GetLineSpacing() const
{
    return fLineSpacing_;
}

void Text::SetRemoveStartingSpaces( bool bRemoveStartingSpaces )
{
    if (bRemoveStartingSpaces_ != bRemoveStartingSpaces)
    {
        bRemoveStartingSpaces_ = bRemoveStartingSpaces;
        bUpdateCache_ = true;
    }
}

bool Text::GetRemoveStartingSpaces() const
{
    return bRemoveStartingSpaces_;
}

void Text::Render( float fX, float fY )
{
    if (bReady_)
    {
        Update();

        if (fX != fX_ || fY != fY_)
            bUpdateQuads_ = true;

        if (bUpdateQuads_)
        {
            fX_ = fX;
            fY_ = fY;

            lQuadList_.clear();

            sf::Sprite mSprite;
            mSprite.setTexture(*pFont_->GetTexture());

            for (auto& mLetter : lLetterCache_)
            {
                mSprite.setPosition((mLetter.fX1+fX), (mLetter.fY1+fY));
                mSprite.setTextureRect(sf::IntRect(
                    mLetter.iU1, mLetter.iV1, mLetter.iU2 - mLetter.iU1, mLetter.iV2 - mLetter.iV1
                ));

                if (!mLetter.mColor.IsNaN() && !bForceColor_)
                {
                    mSprite.setColor(sf::Color(
                        mLetter.mColor.GetR(),
                        mLetter.mColor.GetG(),
                        mLetter.mColor.GetB(),
                        mLetter.mColor.GetA()
                    ));
                }
                else
                {
                    mSprite.setColor(sf::Color(
                        mColor_.GetR(),
                        mColor_.GetG(),
                        mColor_.GetB(),
                        mColor_.GetA()
                    ));
                }

                lQuadList_.push_back(mSprite);
            }

            bUpdateQuads_ = false;
        }

        for (auto& mQuad : lQuadList_)
        {
            Application::GetMainApp()->GetRenderWindow()->draw(mQuad);
        }
    }
}

void Text::Update()
{
    if (bReady_ && bUpdateCache_)
    {
        UpdateLines_();
        UpdateCache_();
        bUpdateCache_ = false;
        bUpdateQuads_ = true;
    }
}

void GetFormat( std::string::iterator& iterChar, Text::Format& mFormat )
{
    if (*iterChar == 'r')
    {
        mFormat.mColorAction = Text::COLOR_ACTION_RESET;
    }
    else if (*iterChar == 'c')
    {
        std::string sColorPart;
        iterChar++;
        sColorPart += *iterChar; iterChar++;
        sColorPart += *iterChar; iterChar++;
        std::string sA = sColorPart;
        uchar_t ucA = uchar_t(HexToUInt(sColorPart));
        sColorPart.clear();
        sColorPart += *iterChar; iterChar++;
        sColorPart += *iterChar; iterChar++;
        std::string sR = sColorPart;
        uchar_t ucR = uchar_t(HexToUInt(sColorPart));
        sColorPart.clear();
        sColorPart += *iterChar; iterChar++;
        sColorPart += *iterChar; iterChar++;
        std::string sG = sColorPart;
        uchar_t ucG = uchar_t(HexToUInt(sColorPart));
        sColorPart.clear();
        sColorPart += *iterChar; iterChar++;
        sColorPart += *iterChar;
        std::string sB = sColorPart;
        uchar_t ucB = uchar_t(HexToUInt(sColorPart));

        mFormat.mColorAction = Text::COLOR_ACTION_SET;
        mFormat.mColor = Color(ucA, ucR, ucG, ucB);
    }
}

void Text::UpdateLines_()
{
    // Update the line list, read format tags, do word wrapping, ...
    lLineList_.clear();
    lFormatList_.clear();

    uint_t uiMaxLineNbr = 0, uiCounter = 0;
    if (std::isfinite(fBoxH_))
        uiMaxLineNbr = uint_t(std::floor(fBoxH_/(GetLineHeight()*fLineSpacing_)));
    else
        uiMaxLineNbr = npos;

    if (uiMaxLineNbr >= 1)
    {
        std::vector<std::string> lManualLineList = SplitEach(sText_, "\n");
        for (auto& sLine : lManualLineList)
        {
            // Make a temporary line array
            std::vector<Line> lLines;
            Line mLine;
            std::map<uint_t, Format> lTempFormatList;

            for (std::string::iterator iterChar1 = sLine.begin(); iterChar1 != sLine.end(); ++iterChar1)
            {
                // Read format tags
                if (*iterChar1 == '|')
                {
                    ++iterChar1;
                    if (iterChar1 != sLine.end())
                    {
                        if (*iterChar1 == '|')
                        {
                        }
                        else
                        {
                            GetFormat(iterChar1, lTempFormatList[uiCounter+mLine.sCaption.size()]);
                            continue;
                        }
                    }
                    else
                        break;
                }

                if (*iterChar1 == ' ')
                    mLine.fWidth += fSpaceWidth_;
                else
                {

                    mLine.fWidth += GetCharacterWidth(*iterChar1);
                    //Log("2 : "+mLine.fWidth);
                    /*std::string::iterator iterNext = iterChar1 + 1;
                    if (iterNext != sLine.end())
                    {
                        if (*iterNext != ' ')
                            mLine.fWidth += GetCharacterKerning((uint_t)*iterChar1, (uint_t)*iterNext);
                    }*/
                }
                mLine.sCaption += *iterChar1;

                if (mLine.fWidth > fBoxW_)
                {
                    // Whoops, the line is too long...
                    if (mLine.sCaption.find(" ") != std::string::npos)
                    {
                        // There are several words on this line, we'll
                        // be able to put the last one on the next line
                        std::string::iterator iterChar2 = mLine.sCaption.end();
                        std::string sErasedString;
                        uint_t uiCharToErase = 0;
                        float fErasedWidth = 0.0;
                        bool bLastWasWord = false;
                        while ( (mLine.fWidth > fBoxW_) && (iterChar2 != mLine.sCaption.begin()) )
                        {
                            --iterChar2;
                            if (*iterChar2 == ' ')
                            {
                                if ( bLastWasWord && (mLine.fWidth-fErasedWidth <= fBoxW_) && !bRemoveStartingSpaces_ )
                                {
                                    break;
                                }
                                else
                                {
                                    mLine.fWidth -= fErasedWidth + fSpaceWidth_;
                                    sErasedString = *iterChar2 + sErasedString;
                                    fErasedWidth = 0.0f;
                                    ++uiCharToErase;
                                }
                            }
                            else
                            {
                                fErasedWidth += GetCharacterWidth(*iterChar2);
                                sErasedString = *iterChar2 + sErasedString;
                                ++uiCharToErase;
                                bLastWasWord = true;
                            }
                        }

                        if (bRemoveStartingSpaces_)
                        {
                            while (*iterChar2 == ' ')
                            {
                                --uiCharToErase;
                                sErasedString = sErasedString.substr(1);
                                ++iterChar2;
                            }
                        }

                        mLine.sCaption = mLine.sCaption.substr(0, mLine.sCaption.size() - uiCharToErase);

                        lLines.push_back(mLine);
                        for (auto& p : lTempFormatList)
                        {
                            lFormatList_[p.first] = p.second;
                        }
                        lTempFormatList.clear();
                        uiCounter += mLine.sCaption.size();
                        mLine.fWidth = GetStringWidth(sErasedString);
                        mLine.sCaption = sErasedString;
                    }
                    else
                    {
                        // There is only one word on this line, so this
                        // word is just too long for the text box : our
                        // only option is to truncate it.
                        float fWordWidth = 3*(GetCharacterWidth((uint_t)'.') + fTracking_);
                        std::string::iterator iterChar2 = mLine.sCaption.end();
                        std::string sErasedWord;
                        uint_t uiCharToErase = 0;
                        while ( (mLine.fWidth + fWordWidth > fBoxW_) && (iterChar2 != mLine.sCaption.begin()) )
                        {
                            --iterChar2;
                            mLine.fWidth -= GetCharacterWidth(*iterChar2);
                            ++uiCharToErase;
                        }
                        mLine.sCaption = mLine.sCaption.substr(0, mLine.sCaption.size() - uiCharToErase);
                        mLine.sCaption += "...";

                        std::string::iterator iterTemp = iterChar1;
                        iterChar1 = sLine.find(" ", iterChar1 - sLine.begin()) + sLine.begin();

                        if (iterChar1 != sLine.end())
                        {
                            // Read cutted format tags
                            while (iterTemp != iterChar1)
                            {
                                if ((*iterTemp) == '|')
                                {
                                    ++iterTemp;
                                    if (iterTemp != iterChar1)
                                    {
                                        if ((*iterTemp) == '|')
                                        {
                                        }
                                        else
                                        {
                                            GetFormat(iterTemp, lTempFormatList[uiCounter+mLine.sCaption.size()]);
                                        }
                                    }
                                }
                                ++iterTemp;
                            }

                            // Look for the next word
                            while (iterChar1 != sLine.end())
                            {
                                if ((*iterChar1) == ' ')
                                    ++iterChar1;
                                else
                                    break;
                            }

                            // Add the line
                            if (iterChar1 != sLine.end())
                            {
                                --iterChar1;
                                lLines.push_back(mLine);
                                uiCounter += mLine.sCaption.size();
                                for (auto& p : lTempFormatList)
                                {
                                    lFormatList_[p.first] = p.second;
                                }
                                lTempFormatList.clear();
                                mLine.fWidth = 0.0f;
                                mLine.sCaption = "";
                            }
                            else
                                break;
                        }
                        else
                            break;
                    }
                }
            }

            lLines.push_back(mLine);
            for (auto& p : lTempFormatList)
            {
                lFormatList_[p.first] = p.second;
            }
            lTempFormatList.clear();
            uiCounter += mLine.sCaption.size();

            // Add the maximum number of line to this Text
            for (auto& mLine : lLines)
            {
                lLineList_.push_back(mLine);
                if (lLineList_.size() == uiMaxLineNbr)
                {
                    return;
                }
            }
        }
    }
}

void Text::UpdateCache_()
{
    lLetterCache_.clear();

    if (!lLineList_.empty())
    {
        if (std::isfinite(fBoxW_))
        {
            fW_ = fBoxW_;
        }
        else
        {
            fW_ = 0.0f;
            for (auto& mLine : lLineList_)
            {
                fW_ = std::max(fW_, mLine.fWidth);
            }
        }

        fH_ = GetLineHeight()*(float(lLineList_.size() - 1)*fLineSpacing_ + 1.0f);

        float fX = 0.0, fY = 0.0;
        float fX0 = 0.0;

        if (std::isfinite(fBoxW_))
        {
            switch (mAlign_)
            {
                case ALIGN_LEFT :
                    fX0 = 0.0f;
                    break;
                case ALIGN_CENTER :
                    fX0 = fBoxW_*0.5f;
                    break;
                case ALIGN_RIGHT :
                    fX0 = fBoxW_;
                    break;
            }
        }
        else
        {
            fX0 = 0.0f;
        }

        float fYOffset = 0.0;

        if (std::isfinite(fBoxH_))
        {
            switch (mVertAlign_)
            {
                case ALIGN_TOP :
                    fY = 0.0f;
                    break;
                case ALIGN_MIDDLE :
                    fY = (fBoxH_ - fH_)*0.5f;
                    break;
                case ALIGN_BOTTOM :
                    fY = (fBoxH_ - fH_);
                    break;
            }
        }
        else
        {
            switch (mVertAlign_)
            {
                case ALIGN_TOP :
                    fY = 0.0f;
                    break;
                case ALIGN_MIDDLE :
                    fY = -fH_*0.5f;
                    break;
                case ALIGN_BOTTOM :
                    fY = -fH_;
                    break;
            }
        }

        uint_t uiCounter = 0;

        Letter mLetter;

        Color mColor = Color::NaN;
        for (auto& mLine : lLineList_)
        {
            switch (mAlign_)
            {
                case ALIGN_LEFT :
                    fX = fX0;
                    break;
                case ALIGN_CENTER :
                    fX = fX0 - mLine.fWidth*0.5f;
                    break;
                case ALIGN_RIGHT :
                    fX = fX0 - mLine.fWidth;
                    break;
            }

            std::string::iterator iterChar, iterNext;
            for (iterChar = mLine.sCaption.begin(); iterChar != mLine.sCaption.end(); ++iterChar)
            {
                // Format our text
                auto iter = lFormatList_.find(uiCounter);
                if (iter != lFormatList_.end())
                {
                    const Format& mFormat = iter->second;
                    switch (mFormat.mColorAction)
                    {
                        case COLOR_ACTION_SET :
                            mColor = mFormat.mColor;
                            break;
                        case COLOR_ACTION_RESET :
                            mColor = Color::NaN;
                            break;
                        default : break;
                    }
                }

                float fCharWidth = 0.0, fCharHeight = 0.0;

                // Add the character to the cache
                if (*iterChar == ' ')
                {
                    fCharWidth = fSpaceWidth_;
                }
                else
                {
                    std::array<float,4> lUVs = pFont_->GetCharacterUVs(*iterChar);
                    fCharWidth = GetCharacterWidth((uint_t)*iterChar);
                    fCharHeight = (lUVs[3] - lUVs[1])*pFont_->GetTextureHeight();
                    float fYOffset = fSize_/2 - fCharHeight/2;

                    mLetter.fX1 = fX;            mLetter.fY1 = fY+fYOffset;
                    mLetter.fX2 = fX+fCharWidth; mLetter.fY2 = fY+fYOffset+fCharHeight;

                    mLetter.iU1 = int(lUVs[0]*pFont_->GetTextureWidth());
                    mLetter.iV1 = int(lUVs[1]*pFont_->GetTextureHeight());
                    mLetter.iU2 = int(lUVs[2]*pFont_->GetTextureWidth());
                    mLetter.iV2 = int(lUVs[3]*pFont_->GetTextureHeight());

                    mLetter.mColor = mColor;

                    lLetterCache_.push_back(mLetter);
                }

                iterNext = iterChar + 1;
                float fKerning = 0.0f;
                /*if (iterNext != mLine.sCaption.end())
                {
                    if (*iterNext != ' ' && *iterChar != ' ')
                    {
                        fKerning = GetCharacterKerning((uint_t)*iterChar, (uint_t)*iterNext);
                    }
                }*/

                fX += fCharWidth + fKerning + fTracking_;
                ++uiCounter;
            }

            fY += GetLineHeight()*fLineSpacing_;
        }
    }
    else
    {
        fW_ = 0.0f;
        fH_ = 0.0f;
    }
}
