#include "board.h"
#include "button.h"
#include "tile.h"
#include "orb.h"
#include "sprite.h"
#include "text.h"
#include "inputmanager.h"
#include "application.h"

void Player1Button(Application& mApp)
{
    Board* pBoard = mApp.GetBoard();
    if (pBoard)
    {
        pBoard->SetState(Board::STATE_PLAYER2);
    }
}

void Player2Button(Application& mApp)
{
    Board* pBoard = mApp.GetBoard();
    if (pBoard)
    {
        pBoard->SetState(Board::STATE_PLAYER1);
    }
}

Board::Board( const Vector2D& mPos, Application& mApp ) : mApp_(mApp), mPosition_(mPos)
{
    mState_ = STATE_PLAYER1;

    std::string sPlayer;
    std::string sEndTurn;
    std::string sWaitPlayer;
    if (mApp_.GetLanguage() == "fr")
    {
        sPlayer = "Joueur";
        sEndTurn = "Fin du tour";
        sWaitPlayer = "Attente joueur";
    }
    else if (mApp_.GetLanguage() == "en")
    {
        sPlayer = "Player";
        sEndTurn = "End turn";
        sWaitPlayer = "Wait for player";
    }

    pWinText_ = std::unique_ptr<Text>(new Text("ravie.ttf", 36));
    pWinText_->SetAlignment(Text::ALIGN_CENTER);

    pPlayer1Text_ = std::unique_ptr<Text>(new Text("ravie.ttf", 24));
    pPlayer1Text_->SetText(sPlayer+" 1 : ");

    pPlayer2Text_ = std::unique_ptr<Text>(new Text("ravie.ttf", 24));
    pPlayer2Text_->SetText(sPlayer+" 2 : ");

    mRect_ = sf::RectangleShape(sf::Vector2f(mApp_.GetScreenWidth(), mApp_.GetScreenHeight()));
    mRect_.setPosition(sf::Vector2f(0.0, 0.0));
    mRect_.setFillColor(sf::Color(0, 0, 0, 128));

    pPlayer1Button_ = std::unique_ptr<Button>(new Button(Vector2D(130, 80), "menu_button", sEndTurn, &Player1Button, mApp_));
    pPlayer2Button_ = std::unique_ptr<Button>(new Button(Vector2D(130, 180), "menu_button", sWaitPlayer+" 1", &Player2Button, mApp_));
    pPlayer2Button_->Disable();

    CreateGrid_();
    CreateOrbs_();
}

Board::~Board()
{
}

void Board::CreateGrid_()
{
    pNormalGrid_ = std::unique_ptr<Sprite>(new Sprite("grid.png"));
    pNormalGrid_->SetHotSpot(32, 32);
    pHomeGrid_ = std::unique_ptr<Sprite>(new Sprite("grid_home.png"));
    pHomeGrid_->SetHotSpot(32, 32);

    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            if ((i < 2 && (j < 2 || j > 7)) || (i > 7  && (j < 2 || j > 7)))
                continue;

            if (1 < i && i < 8)
            {
                if (j < 2)
                    lTileList_.push_back(std::unique_ptr<Tile>(new Tile(Slot(i, j), Tile::HOME_RED)));
                else if (j > 7)
                    lTileList_.push_back(std::unique_ptr<Tile>(new Tile(Slot(i, j), Tile::HOME_BLUE)));
                else
                    lTileList_.push_back(std::unique_ptr<Tile>(new Tile(Slot(i, j), Tile::NORMAL)));
            }
            else if (1 < j && j < 8)
            {
                if (i < 2)
                    lTileList_.push_back(std::unique_ptr<Tile>(new Tile(Slot(i, j), Tile::HOME_PINK)));
                else if (i > 7)
                    lTileList_.push_back(std::unique_ptr<Tile>(new Tile(Slot(i, j), Tile::HOME_GREEN)));
                else
                    lTileList_.push_back(std::unique_ptr<Tile>(new Tile(Slot(i, j), Tile::NORMAL)));
            }
            else
                lTileList_.push_back(std::unique_ptr<Tile>(new Tile(Slot(i, j), Tile::NORMAL)));
        }
    }

    for (auto& tile1 : lTileList_)
    {
        for (auto& tile2 : lTileList_)
        {
            if (tile1 != tile2)
            {
                Slot mDist = tile1->GetSlot() - tile2->GetSlot();
                if (std::fabs(mDist.X()) <= 1 && std::fabs(mDist.Y()) <= 1)
                {
                    tile1->AddNeighbor(tile2.get());
                    tile2->AddNeighbor(tile1.get());
                }
            }
        }
    }
}

void Board::RenderGrid_()
{
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            if ((i < 2 && (j < 2 || j > 7)) || (i > 7  && (j < 2 || j > 7)))
                continue;

            if (i < 2 || i > 7 || j < 2 || j > 7)
                pHomeGrid_->Render(i*64 + mPosition_.X(), j*64 + mPosition_.Y());
            else
                pNormalGrid_->Render(i*64 + mPosition_.X(), j*64 + mPosition_.Y());
        }
    }
}

void Board::CreateOrbs_()
{
    for (int i = 2; i < 8; ++i)
    {
        for (int j = 0; j < 2; ++j)
            AddOrb_(Slot(i, j), Orb::BLUE);

        for (uint_t j = 8; j < 10; ++j)
            AddOrb_(Slot(i, j), Orb::RED);
    }

    for (int j = 2; j < 8; ++j)
    {
        for (int i = 0; i < 2; ++i)
            AddOrb_(Slot(i, j), Orb::GREEN);

        for (int i = 8; i < 10; ++i)
            AddOrb_(Slot(i, j), Orb::PINK);
    }

    bUpdateMovements_ = true;
}

void Board::RenderOrbs_()
{
    for (auto& pOrb : lOrbList_)
    {
        pOrb->Render();
    }
}


void Board::Render()
{
    RenderGrid_();
    RenderOrbs_();

    pPlayer1Text_->Render(10, 30);
    pPlayer2Text_->Render(10, 130);

    pPlayer1Button_->Render();
    pPlayer2Button_->Render();

    if ( (mState_ == STATE_VICTORY1) || (mState_ == STATE_VICTORY2) )
    {
        mApp_.GetRenderWindow()->draw(mRect_);
        pWinText_->Render(512, 384);
    }
}

void Board::Update(float fDelta)
{
    InputManager* pInputMgr = InputManager::GetSingleton();
    Vector2D mMouse = Vector2D(pInputMgr->GetMousePosX(), pInputMgr->GetMousePosY());

    pPlayer1Button_->Update(fDelta, mMouse, pInputMgr->MouseIsDown(MOUSE_LEFT), pInputMgr->MouseIsReleased(MOUSE_LEFT));
    pPlayer2Button_->Update(fDelta, mMouse, pInputMgr->MouseIsDown(MOUSE_LEFT), pInputMgr->MouseIsReleased(MOUSE_LEFT));

    pMouseOveredOrb_ = nullptr;
    for (auto& pOrb : lOrbList_)
    {
        if (pDraggedOrb_ == pOrb.get())
            continue;

        if (pOrb->Contains(mMouse))
        {
            pOrb->NotifyMouseOver(true);
            pMouseOveredOrb_ = pOrb.get();
        }
        else
            pOrb->NotifyMouseOver(false);
    }

    if (pInputMgr->MouseIsPressed(MOUSE_LEFT))
    {
        if (pMouseOveredOrb_ && (!pMovedOrb_ || pMovedOrb_ == pMouseOveredOrb_))
        {
            if ((mState_ == STATE_PLAYER1 &&
                  ((pMouseOveredOrb_->GetType() == Orb::RED) ||
                   (pMouseOveredOrb_->GetType() == Orb::BLUE))) ||
                (mState_ == STATE_PLAYER2 &&
                  ((pMouseOveredOrb_->GetType() == Orb::GREEN) ||
                   (pMouseOveredOrb_->GetType() == Orb::PINK))))
            {
                bDrag_ = true;
                pDraggedOrb_ = pMouseOveredOrb_;
                pDraggedOrb_->NotifyDragged(true);
                mInitialDragPos_ = pDraggedOrb_->GetPosition();

                if (!pMovedOrb_)
                    mInitialSlot_ = pDraggedOrb_->GetSlot();
            }

        }
    }

    if (pInputMgr->MouseIsPressed(MOUSE_RIGHT))
    {
        if (pMouseOveredOrb_ && pMouseOveredOrb_== pMovedOrb_)
        {
            GetTile_(mInitialSlot_)->SetOrb(pMovedOrb_);
            GetTile_(pMovedOrb_->GetSlot())->SetOrb(nullptr);

            pMovedOrb_->SetPosition(
                Vector2D(64.0f*mInitialSlot_.X(), 64.0f*mInitialSlot_.Y()) + mPosition_,
                mInitialSlot_
            );

            pMovedOrb_ = nullptr;
            bUpdateMovements_ = true;
        }
    }

    if (bDrag_)
    {
        pDraggedOrb_->SetTempPosition(mMouse);
    }

    if (pInputMgr->MouseIsReleased(MOUSE_LEFT))
    {
        if (bDrag_)
        {
            Vector2D mNormalized = (mMouse - mPosition_ + Vector2D(32, 32))/64.0f;
            mNormalized.X() = std::floor(mNormalized.X());
            mNormalized.Y() = std::floor(mNormalized.Y());

            Slot mSlot;
            mSlot.X() = int(mNormalized.X());
            mSlot.Y() = int(mNormalized.Y());

            bool bObstructed_ = false;

            // Check we're still on the board
            if ((!IsInRange(mSlot.X(), 0, 9) || !IsInRange(mSlot.Y(), 0, 9)) ||
                (!IsInRange(mSlot.X(), 2, 7) && !IsInRange(mSlot.Y(), 2, 7)))
                bObstructed_ = true;

            // Check if there is already an orb here
            if (!bObstructed_)
            {
                for (auto& pOrb : lOrbList_)
                {
                    if (pOrb->IsOnSlot(mSlot))
                        bObstructed_ = true;
                }
            }

            if (bObstructed_ || !pDraggedOrb_->CanMoveTo(mSlot))
            {
                mNormalized = (mInitialDragPos_ - mPosition_)/64.0f;
                mSlot.X() = int(mNormalized.X());
                mSlot.Y() = int(mNormalized.Y());
                pDraggedOrb_->SetPosition(mInitialDragPos_, mSlot);
            }
            else
            {
                mPreviousSlot_ = pDraggedOrb_->GetSlot();

                Vector2D mNewPos = mNormalized*64.0f + mPosition_;
                pDraggedOrb_->SetPosition(mNewPos, mSlot);

                if (mSlot == mInitialSlot_)
                    pMovedOrb_ = nullptr;
                else
                    pMovedOrb_ = pDraggedOrb_;

                GetTile_(mSlot)->SetOrb(pDraggedOrb_);
                mNormalized = (mInitialDragPos_ - mPosition_)/64.0f;
                mSlot.X() = int(mNormalized.X());
                mSlot.Y() = int(mNormalized.Y());
                GetTile_(mSlot)->SetOrb(nullptr);

                std::array<uint_t, 4> lOrbCount;
                for (auto& pOrb : lOrbList_)
                {
                    if (pOrb->IsWellPlaced())
                        ++lOrbCount[pOrb->GetType()];
                }

                if ( (lOrbCount[Orb::GREEN] == 12) && (lOrbCount[Orb::PINK] == 12) )
                {
                    SetState(STATE_VICTORY2);
                    pMouseOveredOrb_->NotifyMouseOver(false);
                }
                else if ( (lOrbCount[Orb::RED] == 12) && (lOrbCount[Orb::BLUE] == 12) )
                {
                    SetState(STATE_VICTORY1);
                    pMouseOveredOrb_->NotifyMouseOver(false);
                }

                bUpdateMovements_ = true;
            }

            bDrag_ = false;
            pDraggedOrb_->NotifyDragged(false);
            pDraggedOrb_ = nullptr;
        }
    }

    if (bUpdateMovements_)
    {
        for (auto& pTile : lTileList_)
        {
            if (pTile->IsOccupied())
            {
                Orb* pOrb = pTile->GetOrb();
                if (pOrb != pMovedOrb_)
                {
                    pTile->ComputeAvailableMovements();
                    pOrb->NotifyAvailableMovements(pTile->GetAvailableMovements());
                }
            }
        }

        bUpdateMovements_ = false;
    }
}

void Board::SetState(State mState)
{
    mState_ = mState;

    if (mState_ == STATE_PLAYER1 || mState_ == STATE_PLAYER2)
    {
        if (pMovedOrb_)
        {
            Tile* pTile = GetTile_(pMovedOrb_->GetSlot());
            pTile->ComputeAvailableMovements();
            pMovedOrb_->NotifyAvailableMovements(pTile->GetAvailableMovements());
            pMovedOrb_ = nullptr;
        }
    }

    std::string sWin;
    std::string sEndTurn;
    std::string sWaitPlayer;
    if (mApp_.GetLanguage() == "fr")
    {
        sWin = "Le joueur [PLAYER] gagne la partie !";
        sEndTurn = "Fin du tour";
        sWaitPlayer = "Attente joueur";
    }
    else if (mApp_.GetLanguage() == "en")
    {
        sWin = "Player [PLAYER] won the game!";
        sEndTurn = "End turn";
        sWaitPlayer = "Wait for player";
    }

    switch (mState_)
    {
        case STATE_PLAYER1 :
            pPlayer1Button_->SetCaption(sEndTurn);
            pPlayer1Button_->Enable();
            pPlayer2Button_->SetCaption(sWaitPlayer+" 1");
            pPlayer2Button_->Disable();
            break;
        case STATE_PLAYER2 :
            pPlayer1Button_->SetCaption(sWaitPlayer+" 2");
            pPlayer1Button_->Disable();
            pPlayer2Button_->SetCaption(sEndTurn);
            pPlayer2Button_->Enable();
            break;
        case STATE_VICTORY1 :
            pWinText_->SetText(Replace(sWin, "[PLAYER]", "1"));
            break;
        case STATE_VICTORY2 :
            pWinText_->SetText(Replace(sWin, "[PLAYER]", "2"));
            break;
    }
}

void Board::AddOrb_(const Slot& mSlot, uint_t uiType)
{
    lOrbList_.push_back(std::unique_ptr<Orb>(new Orb(
        Vector2D(mSlot.X()*64 + mPosition_.X(), mSlot.Y()*64 + mPosition_.Y()),
        mSlot, (Orb::Type)uiType
    )));

    GetTile_(mSlot)->SetOrb(lOrbList_.back().get());
}

Tile* Board::GetTile_(const Slot& mSlot)
{
    for (auto& pTile : lTileList_)
    {
        if (pTile->IsOnSlot(mSlot))
        {
            return pTile.get();
        }
    }

    return nullptr;
}
