#ifndef BOARD_H
#define BOARD_H

#include "utils.h"

#include "point.h"
#include "orb.h"
#include "application.h"

class Tile;
class Sprite;
class Button;

using Vector2D = Point<float>;
using Slot = Point<int>;

class Board
{
public :

    enum State
    {
        STATE_PLAYER1,
        STATE_PLAYER2,
        STATE_VICTORY1,
        STATE_VICTORY2
    };

    Board(const Vector2D& mPos, Application& mApp);
    ~Board();

    void Update(float fDelta);
    void Render();

    void SetState(State mState);

private :

    void CreateGrid_();
    void RenderGrid_();
    void CreateOrbs_();
    void RenderOrbs_();

    void AddOrb_(const Slot& mSlot, uint_t uiType);
    Tile* GetTile_(const Slot& mSlot);

    Application& mApp_;
    State mState_;

    Vector2D mPosition_;

    // GUI
    std::unique_ptr<Text> pWinText_;
    sf::RectangleShape      mRect_;

    std::unique_ptr<Text>   pPlayer1Text_;
    std::unique_ptr<Button> pPlayer1Button_;
    std::unique_ptr<Text>   pPlayer2Text_;
    std::unique_ptr<Button> pPlayer2Button_;

    // Grid
    std::unique_ptr<Sprite>  pNormalGrid_;
    std::unique_ptr<Sprite>  pHomeGrid_;
    std::vector< std::unique_ptr<Tile> > lTileList_;

    // Orbs
    std::vector< std::unique_ptr<Orb> > lOrbList_;
    Orb* pMouseOveredOrb_ = nullptr;
    Orb* pDraggedOrb_ = nullptr;
    Vector2D    mInitialDragPos_;
    bool bUpdateMovements_ = false;
    bool bDrag_ = false;

    Orb* pMovedOrb_ = nullptr;
    Slot        mInitialSlot_;
    Slot        mPreviousSlot_;
};

#endif
