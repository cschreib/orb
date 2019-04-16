#include "sprite.h"
#include "texturemanager.h"
#include "application.h"

const std::string Sprite::CLASS_NAME = "Sprite";

Sprite::~Sprite()
{
}

Sprite::Sprite()
{
}

Sprite::Sprite( const std::string& sTextureFile ) : sTextureFile_(sTextureFile)
{
    sf::Texture& mTexture = *TextureManager::GetSingleton()->LoadTexture(sTextureFile_);
    mSprite_.setTexture(mTexture);
    fTextureWidth_ = mTexture.getSize().x;
    fTextureHeight_ = mTexture.getSize().y;
    fWidth_ = fTextureWidth_;
    fHeight_ = fTextureHeight_;
}

Sprite::Sprite( const std::string& sTextureFile, float fWidth, float fHeight ) : sTextureFile_(sTextureFile)
{
    sf::Texture& mTexture = *TextureManager::GetSingleton()->LoadTexture(sTextureFile_);
    mSprite_.setTexture(mTexture);
    fTextureWidth_ = mTexture.getSize().x;
    fTextureHeight_ = mTexture.getSize().y;
    fWidth_ = fWidth;
    fHeight_ = fHeight;
    mSprite_.setScale(fWidth_/fTextureWidth_, fHeight_/fTextureHeight_);
}

Sprite::Sprite( const Color& mColor, float fWidth, float fHeight )
{
    mSprite_.setColor(sf::Color(mColor.GetR(), mColor.GetG(), mColor.GetB(), mColor.GetA()));
    fTextureWidth_ = 1.0;
    fTextureHeight_ = 1.0;
    fWidth_ = fWidth;
    fHeight_ = fHeight;
    mSprite_.setScale(fWidth_/fTextureWidth_, fHeight_/fTextureHeight_);
}

void Sprite::Render( float fX, float fY ) const
{
    mSprite_.setPosition(fX, fY);
    Application::GetMainApp()->GetRenderWindow()->draw(mSprite_);
}

void Sprite::RenderEx( float fX, float fY, float fRot, float fHScale, float fVScale ) const
{
    mSprite_.setPosition(fX, fY);
    mSprite_.setScale(fHScale*fWidth_/fTextureWidth_, fVScale*fHeight_/fTextureHeight_);
    mSprite_.rotate(fRot*360.0);
    Application::GetMainApp()->GetRenderWindow()->draw(mSprite_);
}

void Sprite::SetColor( const Color& mColor )
{
    mSprite_.setColor(sf::Color(mColor.GetR(), mColor.GetG(), mColor.GetB(), mColor.GetA()));
}

void Sprite::SetHotSpot( const Point<float>& mHotSpot )
{
    mSprite_.setOrigin(mHotSpot.X(), mHotSpot.Y());
}

void Sprite::SetHotSpot( float fX, float fY )
{
    mSprite_.setOrigin(fX, fY);
}

void Sprite::SetDimensions(float fWidth, float fHeight )
{
    fWidth_ = fWidth;
    fHeight_ = fHeight;
    mSprite_.setScale(fWidth_/fTextureWidth_, fHeight_/fTextureHeight_);
}

void Sprite::SetBoundingBox( const AxisAlignedBox2D& mBox )
{
    mBoundingBox_ = mBox;
}

const AxisAlignedBox2D& Sprite::GetBoundingBox() const
{
    return mBoundingBox_;
}

bool Sprite::Contains( const Vector2D& mPoint ) const
{
    return mBoundingBox_.Contains(mPoint);
}
