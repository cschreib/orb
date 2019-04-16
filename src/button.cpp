#include "button.h"
#include "sprite.h"
#include "text.h"

Button::Button(const Vector2D& mPosition, const std::string& sFile, const std::string& sCaption, OnClickFunc mFunc, Application& mApp) :
    mApp_(mApp), mFunc_(mFunc), mPosition_(mPosition)
{
    pHighlight_ = std::unique_ptr<Sprite>(new Sprite(sFile+"_highlight.png"));
    pHighlight_->SetHotSpot(128, 64);

    pButton_ = std::unique_ptr<Sprite>(new Sprite(sFile+".png"));
    pButton_->SetHotSpot(128, 64);
    pButton_->SetBoundingBox(AxisAlignedBox2D(Vector2D(15, 37), Vector2D(241, 91)));

    pButtonDown_ = std::unique_ptr<Sprite>(new Sprite(sFile+"_down.png"));
    pButtonDown_->SetHotSpot(128, 64);

    pButtonDisabled_ = std::unique_ptr<Sprite>(new Sprite(sFile+"_disabled.png"));
    pButtonDisabled_->SetHotSpot(128, 64);

    pCaption_ = std::unique_ptr<Text>(new Text("ravie.ttf", 16));
    pCaption_->SetText(sCaption);
    pCaption_->SetAlignment(Text::ALIGN_CENTER);

    mState_ = STATE_NORMAL;
}

Button::~Button()
{
}

void Button::Update(float fDelta, const Vector2D& mMouse, bool bMouseDown, bool bMouseReleased)
{
    if (mState_ != STATE_DISABLED)
    {
        Vector2D mLocalMouse = mMouse - mPosition_ + Vector2D(128, 64);
        bMouseOver_ = pButton_->Contains(mLocalMouse);
        if (bMouseOver_)
        {
            if (bMouseDown)
                mState_ = STATE_PUSHED;
            else
                mState_ = STATE_NORMAL;

            if (bMouseReleased && mFunc_)
                (*mFunc_)(mApp_);
        }
        else
            mState_ = STATE_NORMAL;

        fTime_ += fDelta;
        while (fTime_ > 1.0f) fTime_ -= 1.0f;

        float fAlpha = 127.5f*(std::cos(fTime_)+1.0f);
        pHighlight_->SetColor(Color(uchar_t(fAlpha), 255, 255, 255));
    }
}

void Button::Render() const
{
    switch (mState_)
    {
        case STATE_NORMAL : pButton_->Render(mPosition_.X(), mPosition_.Y()); break;
        case STATE_PUSHED : pButtonDown_->Render(mPosition_.X(), mPosition_.Y()); break;
        case STATE_DISABLED : pButtonDisabled_->Render(mPosition_.X(), mPosition_.Y()); break;
    }

    pCaption_->Render(mPosition_.X(), mPosition_.Y());

    if (bMouseOver_)
    {
        pHighlight_->Render(mPosition_.X(), mPosition_.Y());
        pHighlight_->SetColor(Color::WHITE);
        pHighlight_->Render(mPosition_.X(), mPosition_.Y());
    }
}

void Button::SetCaption(const std::string& sCaption)
{
    pCaption_->SetText(sCaption);
}

void Button::SetPosition(const Vector2D& mPosition)
{
    mPosition_ = mPosition;
}

void Button::Disable()
{
    mState_ = STATE_DISABLED;
    bMouseOver_ = false;
}

void Button::Enable()
{
    mState_ = STATE_NORMAL;
}

Text* Button::GetText()
{
    return pCaption_.get();
}
