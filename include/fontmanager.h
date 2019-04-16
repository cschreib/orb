#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include "utils.h"
#include "manager.h"

class Font;

/// Manages font creation
class FontManager : public Manager<FontManager>
{
friend class Manager<FontManager>;
public :

    /// Creates or retreives a Font.
    /** \param sFontFile The path to the .tff file
    *   \param uiSize    The size at which to render the font
    *   \note This function will create the font if it doens't exists,
    *         or return a pointer to it if has already been created.
    */
    Font*  GetFont(const std::string& sFontFile, const uint_t& uiSize);

    /// Returns the name of the default font.
    /** \return The name of the default font
    *   \note This value is read from config files.
    */
    const std::string& GetDefaultFont() const;

    /// Reads the configuration files.
    void         ReadConfig();

    static const std::string CLASS_NAME;

protected :

    /// Default constructor.
    /** \note This is a singleton class. That means that there can
    *         only be a single instance at the same time. That's why
    *         the constructor is not in public scope.<br>
    *         If you need to get a pointer to that unique instance,
    *         call FontManager::GetSingleton().<br>
    *         Note that, if GetSingleton() has been called at least once,
    *         you'll have to delete the returned pointer when you close
    *         your program by calling FontManager::Delete() (this is
    *         automatically done by Engine).
    */
    FontManager();

    /// Destructor.
    /** \note If GetSingleton() has been called at least once,
    *         you'll have to delete the returned pointer when
    *         you close your program by calling Delete() (this is
    *         automatically done by Engine).
    */
    ~FontManager();

    /// Copy constructor.
    FontManager(const FontManager& mMgr);

    /// Assignment operator.
    FontManager& operator = (const FontManager& mMgr);

private :

    std::string sDefaultFont_;

    std::map< std::string, std::unique_ptr<Font> > lFontList_;
};

#endif
