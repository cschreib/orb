#ifndef COLOR_H
#define COLOR_H

#include "utils.h"

/// Color container
/** Automatically builds ARGB packed
*   color out of separated A, R, G and B
*   values and vice versa.
*/
class Color
{
public :

    /// Default constructor.
    Color();

    /// Constructor.
    /** \param fR The red component
    *   \param fG The green component
    *   \param fB The blue component
    */
    Color(float fR, float fG, float fB);

    /// Constructor.
    /** \param fA The alpha component
    *   \param fR The green component
    *   \param fG The green component
    *   \param fB The blue component
    */
    Color(float fA, float fR, float fG, float fB);

    /// Constructor.
    /** \param uiColor The ARGB packed color
    */
    Color(uint_t uiColor);

    /// Returns the alpha component.
    /** \return The alpha component
    */
    float GetA() const;

    /// Returns the red component.
    /** \return The red component
    */
    float GetR() const;

    /// Returns the green component.
    /** \return The green component
    */
    float GetG() const;

    /// Returns the blue component.
    /** \return The blue component
    */
    float GetB() const;

    /// Returns the packed ARGB value.
    /** \return The packed ARGB value
    */
    uint_t         GetPacked() const;

    /// Sets the alpha component.
    /** \param fA The alpha component
    */
    void           SetA(float fA);

    /// Sets the red component.
    /** \param fR The red component
    */
    void           SetR(float fR);

    /// Sets the green component.
    /** \param fG The green component
    */
    void           SetG(float fG);

    /// Sets the blue component.
    /** \param fB The blue component
    */
    void           SetB(float fB);

    /// Sets the packed ARGB component.
    /** \param uiColor packed ARGB component.
    */
    void           SetPacked(uint_t uiColor);

    /// Clamps color components to [0,1].
    void           Saturate();

    /// Sets if this Color has a NaN chanel.
    /** \return 'true' if this Color has a NaN chanel
    */
    bool         IsNaN() const;

    float&       operator [] (uint_t uiIndex);
    float operator [] (uint_t uiIndex) const;

    bool operator == (const Color& mColor) const;
    bool operator != (const Color& mColor) const;

    Color  operator * (const Color& mColor) const;
    Color  operator + (const Color& mColor) const;
    Color  operator - (const Color& mColor) const;

    Color&  operator *= (const Color& mColor);
    Color&  operator += (const Color& mColor);
    Color&  operator -= (const Color& mColor);

    Color   operator *  (float fValue) const;
    Color   operator /  (float fValue) const;
    void    operator *= (float fValue);
    void    operator /= (float fValue);

    /// Clamps color components to [0,1].
    /** \param mColor The color to clamp
    *   \return The clamped color
    */
    static Color  Saturate(const Color& mColor);

    static const Color BLACK;
    static const Color WHITE;
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color NaN;

    static const std::string CLASS_NAME;

    float r = 0;
    float g = 0;
    float b = 0;
    float a = 0;

private :

    uint_t BuildUIColor_() const;
    void BuildABGRColor_(uint_t uiColor);
};

Color operator * (float fLeft, const Color& mRight);
Color operator / (float fLeft, const Color& mRight);

#endif
