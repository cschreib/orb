#include "orb.h"
#include "sprite.h"

Orb::Orb(const Vector2D& mPos, const Slot& mSlot, Type mType) :
    mPosition_(mPos), mTempPosition_(mPos), mSlot_(mSlot), mType_(mType)
{
    pMovementSprite_ = std::unique_ptr<Sprite>(new Sprite("cross.png"));

    switch (mType_)
    {
        case BLUE  :
            pSprite_ = std::unique_ptr<Sprite>(new Sprite("orb_blue.png"));
            pMovementSprite_->SetColor(Color(0, 150, 255));
            break;
        case RED   :
            pSprite_ = std::unique_ptr<Sprite>(new Sprite("orb_red.png"));
            pMovementSprite_->SetColor(Color(255, 90, 0));
            break;
        case GREEN :
            pSprite_ = std::unique_ptr<Sprite>(new Sprite("orb_green.png"));
            pMovementSprite_->SetColor(Color(0, 255, 0));
            break;
        case PINK  :
            pSprite_ = std::unique_ptr<Sprite>(new Sprite("orb_pink.png"));
            pMovementSprite_->SetColor(Color(255, 0, 255));
            break;
    }

    pSprite_->SetHotSpot(32, 32);
    pMovementSprite_->SetHotSpot(32, 32);
}

Orb::~Orb()
{
}

void Orb::SetTempPosition(const Vector2D& mPos)
{
    mTempPosition_ = mPos;
}

void Orb::SetPosition(const Vector2D& mPos, const Slot& mSlot)
{
    mPosition_ = mPos;
    mTempPosition_ = mPosition_;
    mSlot_ = mSlot;
}

const Vector2D& Orb::GetPosition() const
{
    return mPosition_;
}

void Orb::Render() const
{
    if (bMouseOver_)
    {
        for (auto& mAvailableSlot : lAvailableMovements_)
        {
            if (!IsOnSlot(mAvailableSlot))
            {
                pMovementSprite_->Render(
                    float(mAvailableSlot.X()*64) + mPosition_.X() - 64.0f*float(mSlot_.X()),
                    float(mAvailableSlot.Y()*64) + mPosition_.Y() - 64.0f*float(mSlot_.Y())
                );
            }
        }
    }

    pSprite_->Render(mTempPosition_.X(), mTempPosition_.Y());
}

bool Orb::Contains(const Vector2D& mPos) const
{
    return ((std::fabs(mPos.X() - mPosition_.X()) < 32.0f) &&
            (std::fabs(mPos.Y() - mPosition_.Y()) < 32.0f));
}

const Slot& Orb::GetSlot()
{
    return mSlot_;
}

bool Orb::IsOnSlot(const Slot& mSlot) const
{
    return (mSlot_ == mSlot);
}

bool Orb::CanMoveTo(const Slot& mSlot) const
{
    for (auto& mAvailableSlot : lAvailableMovements_)
    {
        if (mAvailableSlot == mSlot)
            return true;
    }

    return false;
}

Orb::Type Orb::GetType() const
{
    return mType_;
}

bool Orb::IsWellPlaced() const
{
    return bWellPlaced_;
}

void Orb::NotifyAvailableMovements(const std::vector<Slot>& lMovementList)
{
    lAvailableMovements_ = lMovementList;
}

void Orb::NotifyMouseOver(bool bMouseOver)
{
    bMouseOver_ = bMouseOver;
}

void Orb::NotifyDragged(bool bDragged)
{
    bDragged_ = bDragged;
}

void Orb::NotifyWellPlaced(bool bWellPlaced)
{
    bWellPlaced_ = bWellPlaced;
}
