#include "fontmanager.h"
#include "font.h"
#include "log.h"

const std::string FontManager::CLASS_NAME = "FontManager";

FontManager::FontManager()
{
}

FontManager::~FontManager()
{
}

Font* FontManager::GetFont(const std::string& sFontFile, const uint_t& uiSize)
{
    std::string sID = sFontFile + "|" + ToString(uiSize);
    auto iter = lFontList_.find(sID);
    if (iter == lFontList_.end())
    {
        if (!FileExists(sFontFile))
        {
            Error(CLASS_NAME, "Unknown font file : \""+sFontFile+"\"");
            return nullptr;
        }

        iter = lFontList_.insert(std::make_pair<std::string, std::unique_ptr<Font>>(
            std::move(sID), std::unique_ptr<Font>(new Font(sFontFile, uiSize))
        )).first;
    }

    return iter->second.get();
}

const std::string& FontManager::GetDefaultFont() const
{
    return sDefaultFont_;
}

void FontManager::ReadConfig()
{

}
