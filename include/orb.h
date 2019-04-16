#ifndef ORB_H
#define ORB_H

#include "utils.h"
#include "sprite.h"

using Vector2D = Point<float>;
using Slot = Point<int>;

class Sprite;

class Orb
{
public :

    enum Type
    {
        BLUE = 0,
        RED,
        GREEN,
        PINK
    };

    Orb(const Vector2D& mPos, const Slot& mSlot, Type mType);
    ~Orb();

    void Render() const;

    void SetTempPosition(const Vector2D& mPos);
    void SetPosition(const Vector2D& mPos, const Slot& mSlot);
    const Vector2D& GetPosition() const;
    bool Contains(const Vector2D& mPos) const;
    const Slot& GetSlot();
    bool IsOnSlot(const Slot& mSlot) const;
    bool CanMoveTo(const Slot& mSlot) const;
    Type GetType() const;
    bool IsWellPlaced() const;

    void NotifyAvailableMovements(const std::vector<Slot>& lMovementList);
    void NotifyMouseOver(bool bMouseOver);
    void NotifyDragged(bool bDragged);
    void NotifyWellPlaced(bool bWellPlaced);

private :

    Vector2D mPosition_;
    Vector2D mTempPosition_;
    Slot     mSlot_;
    Type     mType_;

    std::unique_ptr<Sprite> pSprite_;
    std::unique_ptr<Sprite> pMovementSprite_;

    std::vector<Slot> lAvailableMovements_;

    bool bMouseOver_ = false;
    bool bDragged_ = false;
    bool bWellPlaced_ = false;

};

#endif
