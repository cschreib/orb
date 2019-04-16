#include "color.h"

const std::string Color::CLASS_NAME = "Color";

const Color Color::BLACK = Color(0, 0, 0);
const Color Color::WHITE = Color(255, 255, 255);
const Color Color::RED   = Color(255, 0, 0);
const Color Color::GREEN = Color(0, 255, 0);
const Color Color::BLUE  = Color(0, 0, 255);
const Color Color::NaN   = Color(
    std::numeric_limits<float>::quiet_NaN(),
    std::numeric_limits<float>::quiet_NaN(),
    std::numeric_limits<float>::quiet_NaN(),
    std::numeric_limits<float>::quiet_NaN()
);

Color::Color()
{
    a = 255;
}

Color::Color( float fR, float fG, float fB )
{
    a = 255;
    r = fR;
    g = fG;
    b = fB;
}

Color::Color( float fA, float fR, float fG, float fB )
{
    a = fA;
    r = fR;
    g = fG;
    b = fB;
}

Color::Color( uint_t uiColor )
{
    BuildABGRColor_(uiColor);
}

float Color::GetA() const
{
    return a;
}

float Color::GetR() const
{
    return r;
}

float Color::GetG() const
{
    return g;
}

float Color::GetB() const
{
    return b;
}

uint_t Color::GetPacked() const
{
    return BuildUIColor_();
}

void Color::SetA( float fA )
{
    a = fA;
}

void Color::SetR( float fR )
{
    r = fR;
}

void Color::SetG( float fG )
{
    g = fG;
}

void Color::SetB( float fB )
{
    b = fB;
}

void Color::SetPacked( uint_t uiColor )
{
    BuildABGRColor_(uiColor);
}

bool Color::IsNaN() const
{
    return std::isnan(a);
}

void Color::Saturate()
{
    a = Clamp(a, 0.0f, 255.0f);
    r = Clamp(r, 0.0f, 255.0f);
    g = Clamp(g, 0.0f, 255.0f);
    b = Clamp(b, 0.0f, 255.0f);
}

Color Color::Saturate( const Color& mColor )
{
    Color mSat = mColor; mSat.Saturate();
    return mSat;
}

uint_t Color::BuildUIColor_() const
{
    uint_t uiColor;

    if (!std::isnan(a) && !std::isnan(b) && !std::isnan(g) && !std::isnan(r))
    {
        uiColor  = uint(a) << 24;
        uiColor += uint(b) << 16;
        uiColor += uint(g) << 8;
        uiColor += uint(r);
    }
    else
        uiColor = npos;

    return uiColor;
}

void Color::BuildABGRColor_(uint_t uiColor)
{
    if (uiColor != npos)
    {
        a = ((uiColor >> 24) & 0xFF);
        b = ((uiColor >> 16) & 0xFF);
        g = ((uiColor >> 8)  & 0xFF);
        r = ( uiColor        & 0xFF);
    }
    else
        a = b = g = r = std::numeric_limits<float>::quiet_NaN();
}

float& Color::operator [] ( uint_t uiIndex )
{
    return (&r)[uiIndex];
}

float Color::operator [] ( uint_t uiIndex ) const
{
    return (&r)[uiIndex];
}

bool Color::operator == ( const Color& mColor ) const
{
    return (a == mColor.a) && (r == mColor.r) && (g == mColor.g) && (b == mColor.b);
}

bool Color::operator != ( const Color& mColor ) const
{
    return (a != mColor.a) || (r != mColor.r) || (g != mColor.g) || (b != mColor.b);
}

Color Color::operator * (const Color& mColor) const
{
    double c = 1.0/(255.0*255.0);
    return Color(
        c*a*mColor.a, c*r*mColor.r, c*g*mColor.g, c*b*mColor.b
    );
}

Color Color::operator + (const Color& mColor) const
{
    return Color(
        a + mColor.a,
        r + mColor.r,
        g + mColor.g,
        b + mColor.b
    );
}

Color Color::operator - (const Color& mColor) const
{
    return Color(
        a - mColor.a,
        r - mColor.r,
        g - mColor.g,
        b - mColor.b
    );
}

Color& Color::operator *= (const Color& mColor)
{
    a *= mColor.a;
    r *= mColor.r;
    g *= mColor.g;
    b *= mColor.b;

    return *this;
}

Color& Color::operator += (const Color& mColor)
{
    a += mColor.a;
    r += mColor.r;
    g += mColor.g;
    b += mColor.b;

    return *this;
}

Color& Color::operator -= (const Color& mColor)
{
    a -= mColor.a;
    r -= mColor.r;
    g -= mColor.g;
    b -= mColor.b;

    return *this;
}

Color Color::operator * ( float fValue ) const
{
    return Color(r*fValue, g*fValue, b*fValue);
}

Color Color::operator / ( float fValue ) const
{
    return Color(r/fValue, g/fValue, b/fValue);
}

void Color::operator *= ( float fValue )
{
    r *= fValue; g *= fValue; b *= fValue;
}

void Color::operator /= ( float fValue )
{
    r /= fValue; g /= fValue; b /= fValue;
}

Color operator * (float fLeft, const Color& mRight)
{
    return mRight*fLeft;
}

Color operator / (float fLeft, const Color& mRight)
{
    return mRight/fLeft;
}
