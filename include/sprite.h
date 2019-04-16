#ifndef SPRITE_H
#define SPRITE_H

#include "utils.h"
#include "point.h"
#include "color.h"

#include <SFML/Graphics.hpp>

using Vector2D = Point<float>;

struct AxisAlignedBox2D
{
    AxisAlignedBox2D()
    {
        mP1.X() = mP1.Y() = -std::numeric_limits<float>::infinity();
        mP2.X() = mP2.Y() = std::numeric_limits<float>::infinity();
    }

    AxisAlignedBox2D(const Vector2D& p1, const Vector2D& p2) : mP1(p1), mP2(p2)
    {
    }

    bool Contains(const Vector2D& p) const
    {
        return mP1.X() <= p.X() && p.X() <= mP2.X() && mP1.Y() <= p.Y() && p.Y() <= mP2.Y();
    }

    Vector2D mP1, mP2;
};

/// Use to draw a texture on the screen
/** This class is meant to simplify rendering of
*   2D elements. Everything can be done with the Quad
*   struct, but it's really not simple to use.<br>
*   Thanks to this class, the only thing you have to do
*   to acheive interesting effects is calling 2 or 3
*   functions and let the magic do the rest.
*/
class Sprite
{
public :

    Sprite();

    /// Constructor.
    /** \param pMat The Material to use
    *   \note Using this constructor, you don't have to provide
    *         any width or height : they are calculated from the
    *         Material's texture.<br>
    *         So, your sprite will be as large as the texture and
    *         contain it all.<br>
    *         <b>Note</b> : you can't call that constructor with a
    *         blank Material.
    */
    Sprite(const std::string& sTextureFile);

    /// Constructor.
    /** \param pMat    The Material to use
    *   \param fWidth  The width of the Sprite
    *   \param fHeight The height of the Sprite
    *   \note If the width and height you provide are smaller than
    *         the texture's ones, the texture will be cut on the right
    *         and bottom edges.<br>
    *         However, if they are larger than the texture's one, the
    *         texture will be tiled.
    */
    Sprite(const std::string& sTextureFile, float fWidth, float fHeight);

    /// Constructor.
    /** \param mColor  The color of the Sprite
    *   \param fWidth  The width of the Sprite
    *   \param fHeight The height of the Sprite
    *   \note This constructor allow you to use a simple color instead
    *         of a real texture.
    */
    Sprite(const Color& mColor, float fWidth, float fHeight);

    /// Destructor.
    ~Sprite();

    /// Renders this Sprite on the current render target.
    /** \param fX The horizontal position
    *   \param fY The vertical position
    *   \note Must be called between SpriteManager::Begin() and
    *         SpriteManager::End().
    */
    void Render(float fX, float fY) const;

    /// Deforms this Sprite and render it on the current render target.
    /** \param fX      The horizontal position
    *   \param fY      The vertical position
    *   \param fRot    The rotation to apply (angle in radian)
    *   \param fHScale The horizontal scale to apply
    *   \param fVScale The vertical scale to apply
    *   \note This function doesn't store the deformation.<br>
    *         Must be called between SpriteManager::Begin() and
    *         SpriteManager::End().
    */
    void RenderEx(float fX, float fY,
                  float fRot,
                  float fHScale = 1.0f, float fVScale = 1.0f) const;

    /// Changes the color of this sprite.
    /** \param mColor The new color
    *   \param uiIndex The index of the vertice to change
    *   \note If you provide an index, this function will only change
    *         a single vertex's color.<br>
    *         Index 0 is for top left, index 1 is for top right, ...
    */
    void SetColor(const Color& mColor);

    /// Changes this Sprite's center.
    /** \param mHotSpot A 2D point containing the new center's position
    *   \note HotSpot is used to rotate and scale your sprite with RenderEx().<br>
    *         It is also considered as the reference point when you call Render()
    *         (same goes for RenderEx()).
    */
    void SetHotSpot(const Point<float>& mHotSpot);

    /// Changes this Sprite's center.
    /** \param fX The new center's horizontal position
    *   \param fY The new center's vertical position
    *   \note HotSpot is used to rotate and scale your sprite with RenderEx().<br>
    *         It is also considered as the reference point when you call Render()
    *         (same goes for RenderEx()).
    */
    void SetHotSpot(float fX, float fY);

    /// Changes this Sprite's dimensions.
    /** \param fWidth      The new width
    *   \param fHeight     The new height
    *   \note If you adjust texture coordinates, you texture won't be deformed.
    *         Else, it will be streched to fit the new dimensions.
    */
    void SetDimensions(float fWidth, float fHeight);

    void               SetBoundingBox(const AxisAlignedBox2D& mBox);

    const AxisAlignedBox2D& GetBoundingBox() const;

    bool             Contains(const Vector2D& mPoint) const;

    static const std::string CLASS_NAME;

private :

    mutable sf::Sprite mSprite_;
    std::string sTextureFile_;
    float fTextureWidth_ = 0.0;
    float fTextureHeight_ = 0.0;
    float fWidth_ = 0.0;
    float fHeight_ = 0.0;

    AxisAlignedBox2D mBoundingBox_;
};

#endif
