/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Text header               */
/*                                        */
/*                                        */

#ifndef TEXT_H
#define TEXT_H

#include "utils.h"
#include "color.h"

#include <SFML/Graphics.hpp>

class Font;

/// Used to draw some text on the screen
/**
*/
class Text
{
public :

    /// Contains a string that will be drawn on a line
    struct Line
    {
        std::string sCaption;
        float       fWidth = 0.0;
    };

    enum ColorAction
    {
        COLOR_ACTION_NONE,
        COLOR_ACTION_SET,
        COLOR_ACTION_RESET
    };

    /// Contains information about the text at a given position
    struct Format
    {
        Format() : mColorAction(COLOR_ACTION_NONE)
        {}

        Color       mColor;
        ColorAction mColorAction;
    };

    enum Alignment
    {
        ALIGN_LEFT,
        ALIGN_CENTER,
        ALIGN_RIGHT
    };

    enum VerticalAlignment
    {
        ALIGN_TOP,
        ALIGN_MIDDLE,
        ALIGN_BOTTOM
    };

    /// Holds the position, tex. coordinates and color of a character.
    struct Letter
    {
        float fX1 = 0.0, fY1 = 0.0;
        float fX2 = 0.0, fY2 = 0.0;
        int   iU1 = 0, iV1 = 0;
        int   iU2 = 0, iV2 = 0;
        Color mColor = Color::NaN;
    };

    Text();

    /// Constructor.
    /** \param sFileName The path to the .ttf file to use
    *   \param fSize    The size of the font (in point)
    */
    Text(const std::string& sFileName, float fSize);

    /// Destructor.
    ~Text();

    /// Returns the path to the .ttf file.
    /** \return The path to the .ttf file
    */
    const std::string&   GetFontName() const;

    /// Returns the size of the font.
    /** \return The size of the font
    */
    float GetFontSize() const;

    /// Returns the height of one line (constant).
    /** \return The height of one line (constant)
    */
    float        GetLineHeight() const;

    /// Set the text to render.
    /** \param sText The text to render
    *   \note This text can be formated :<br>
    *         - "|cAARRGGBB" : sets text color (hexadecimal).<br>
    *         - "|r" : sets text color to default.<br>
    *         - "||" : writes "|".
    */
    void           SetText(const std::string& sText);

    /// Returns the text that will be rendered.
    /** \return The text that will be rendered
    *   \note This string contains format tags.
    */
    const std::string&   GetText() const;

    /// Sets this text's default color.
    /** \param mColor      The default color
    *   \param bForceColor 'true' to ignore color tags
    */
    void           SetColor(const Color& mColor, bool bForceColor = false);

    /// Returns this text's default color.
    /** \return This text's default color
    */
    const Color&   GetColor() const;

    /// Sets the dimensions of the text box.
    /** \param fW The new witdh
    *   \param fH The new height
    *   \note To remove the text box, use std::numeric_limits<float>::quiet_nan().
    */
    void           SetDimensions(float fW, float fH);

    /// Sets the width of the text box.
    /** \param fBoxW The new witdh
    *   \note To remove it, use std::numeric_limits<float>::quiet_nan().
    */
    void           SetBoxWidth(float fBoxW);

    /// Sets the height of the text box.
    /** \param fBoxH The new height
    *   \note To remove it, use std::numeric_limits<float>::quiet_nan().
    */
    void           SetBoxHeight(float fBoxH);

    /// Returns the width of the rendered text.
    /** \return The width of the rendered text
    *   \note Takes the text box into account if any.
    */
    float GetWidth();

    /// Returns the height of the rendered text.
    /** \return The height of the rendered text
    *   \note Takes the text box into account if any.
    */
    float GetHeight();

    /// Returns the width of the text box.
    /** \return The width of the text box
    */
    float GetBoxWidth() const;

    /// Returns the height of the text box.
    /** \return The height of the text box
    */
    float GetBoxHeight() const;

    /// Returns the length of the text.
    /** \return The length of the text
    *   \note Ignores the text box, but not manual line jumps.
    */
    float        GetTextWidth() const;

    /// Returns the lenght of a provided string.
    /** \param sString The string to measure
    *   \return The lenght of the provided string
    */
    float        GetStringWidth(const std::string& sString) const;

    /// Returns the length of a single character.
    /** \param uiChar The character to measure
    *   \return The lenght of this character
    *   \note Thanks to <windows.h>, I can't name this function
    *         "GetCharWidth"... Bloody macros !
    */
    float        GetCharacterWidth(const uint_t& uiChar) const;

    /// Returns the kerning between two characters.
    /** \param uiChar1 The first character
    *   \param uiChar2 The second character
    *   \return The kerning between two characters
    *   \note Kerning is a letter spacing adjustment that makes the
    *         text look more condensed : is you stick an A near a V,
    *         you can reduce the space between the two letters, but not
    *         if you put two Vs side to side.
    */
    float        GetCharacterKerning(const uint_t& uiChar1, const uint_t& uiChar2) const;

    /// Returns the height of the text.
    /** \return The height of one text
    *   \note Ignores the text box, but not manual line jumps.
    */
    float        GetTextHeight() const;

    /// Sets text alignment.
    /** \param mAlign The new alignment
    */
    void           SetAlignment(const Alignment& mAlign);

    /// Sets text vertical alignment.
    /** \param mVertAlign The new vertical alignment
    */
    void           SetVerticalAlignment(const VerticalAlignment& mVertAlign);

    /// Returns the text aligment.
    /** \return The text alignment
    */
    const Alignment& GetAlignment() const;

    /// Returns the text aligment.
    /** \return The text alignment
    */
    const VerticalAlignment& GetVerticalAlignment() const;

    /// Sets this Text's tracking.
    /** \param fTracking The new tracking
    *   \note Tracking is the space between each character. Default
    *         is 0.
    */
    void           SetTracking(float fTracking);

    /// Returns this Text's tracking.
    /** \return This Text's tracking
    */
    float GetTracking() const;

    /// Sets this Text's line spacing.
    /** \param fLineSpacing The new line spacing
    *   \note Line spacing is a coefficient that, multiplied by the
    *         height of a line, gives the space between two lines.
    *         Default is 1.5f.
    */
    void           SetLineSpacing(float fLineSpacing);

    /// Returns this Text's line spacing.
    /** \return This Text's line spacing
    */
    float GetLineSpacing() const;

    /// Allows removal of a line's starting spaces.
    /** \param bRemoveStartingSpaces 'true' to remove them
    *   \note The text box does word wrapping : it cuts too long
    *         lines only between words. But sometimes, the rendered
    *         text must be cut between several spaces. By default,
    *         the algorithm puts cuted spaces at the beginning of
    *         the next line. You can change this behavior by setting
    *         this function to 'true'.
    */
    void           SetRemoveStartingSpaces(bool bRemoveStartingSpaces);

    /// Checks if starting spaces removing is active.
    /** \return 'true' if starting spaces removing is active
    */
    bool  GetRemoveStartingSpaces() const;

    /// Renders this Text at the given position.
    /** \param fX The horizontal position of the top left corner
    *   \param fY The vertical position of the top left corner
    *   \note Must be called between SpriteManager::Begin() and
    *         SpriteManager::End().
    */
    void           Render(float fX, float fY);

    /// Updates this Text's cache.
    /** \note Automatically done by Render().<br>
    *         Only use this method if you need it to
    *         be updated sooner.
    */
    void           Update();

    static const std::string CLASS_NAME;

private :

    void UpdateLines_();
    void UpdateCache_();

    std::string       sFileName_;
    bool              bReady_ = false;
    float             fSize_ = 0.0;
    float             fTracking_ = 0.0;
    float             fLineSpacing_ = 0.0;
    float             fSpaceWidth_ = 0.0;
    bool              bRemoveStartingSpaces_ = false;
    Color             mColor_;
    bool              bForceColor_ = false;
    float             fW_ = 0.0, fH_ = 0.0;
    float             fX_ = 0.0, fY_ = 0.0;
    float             fBoxW_ = 0.0, fBoxH_ = 0.0;
    std::string       sText_;
    Alignment         mAlign_ = ALIGN_LEFT;
    VerticalAlignment mVertAlign_ = ALIGN_TOP;

    std::vector<Line>        lLineList_;
    std::map<uint_t, Format> lFormatList_;

    bool                bUpdateCache_ = false;
    std::vector<Letter> lLetterCache_;

    bool                    bUpdateQuads_ = false;
    std::vector<sf::Sprite> lQuadList_;

    Font* pFont_ = nullptr;
};

#endif
