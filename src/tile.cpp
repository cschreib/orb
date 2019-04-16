#include "tile.h"

Tile::Tile(const Slot& mSlot, Type mType) : mSlot_(mSlot), mType_(mType)
{
}

const Slot& Tile::GetSlot() const
{
    return mSlot_;
}

bool Tile::IsOnSlot(const Slot& mSlot) const
{
    return (mSlot_ == mSlot);
}

void Tile::AddNeighbor(Tile* pTile)
{
    auto iter = std::find(lNeighborList_.begin(), lNeighborList_.end(), pTile);
    if (iter == lNeighborList_.end())
        lNeighborList_.push_back(pTile);
}

Tile* Tile::GetNeighbor(const Slot& mDirection) const
{
    for (auto& pTile : lNeighborList_)
    {
        if ((pTile->GetSlot() - mSlot_) == mDirection)
            return pTile;
    }

    return nullptr;
}

void Tile::SetOrb(Orb* pOrb)
{
    pOrb_ = pOrb;

    if (pOrb_)
        pOrb_->NotifyWellPlaced((uint_t)pOrb_->GetType() == (uint_t)mType_);
}

Orb* Tile::GetOrb() const
{
    return pOrb_;
}

bool Tile::IsOccupied()
{
    return pOrb_ != nullptr;
}

void Tile::ComputeAvailableMovements()
{
    lAvailableMovements_.clear();
    lAvailableMovements_.push_back(mSlot_);

    if (!pOrb_)
        return;

    std::vector< Tile* > lVisitedList;

    lVisitedList.push_back(this);

    // Visit adjacent tiles
    std::vector< Tile* >::const_iterator iterNeighbor;
    for (auto& pTile : lNeighborList_)
    {
        if (!pTile->IsOccupied())
        {
            lAvailableMovements_.push_back(pTile->GetSlot());
            lVisitedList.push_back(pTile);
        }
    }

    std::vector< Tile* > lDirectMovements = GetFirstStepNeighbors_(this, lVisitedList);
    while (!lDirectMovements.empty())
    {
        std::vector< Tile* > lTempMovements;
        for (auto& pTile : lDirectMovements)
        {
            std::vector< Tile* > lTempMovements2 = pTile->GetFirstStepNeighbors_(this, lVisitedList);
            for (auto& pTile2 : lTempMovements2)
            {
                lTempMovements.push_back(pTile2);
            }

            lAvailableMovements_.push_back(pTile->GetSlot());
            lVisitedList.push_back(pTile);
        }

        lDirectMovements = lTempMovements;
    }
}

std::vector< Tile* > Tile::GetFirstStepNeighbors_(Tile* pStart, const std::vector< Tile* >& lVisitedTiles) const
{
    std::vector< Tile* > lMovements;

    std::vector< Tile* >::const_iterator iter;
    for (auto& pTile : lNeighborList_)
    {
        Slot mDirection = pTile->GetSlot() - mSlot_;
        uint_t uiPreDistance = 1;
        Tile* pNewTile = pTile;
        while (pNewTile && !pNewTile->IsOccupied())
        {
            pNewTile = pNewTile->GetNeighbor(mDirection);
            ++uiPreDistance;
        }

        if (!pNewTile || pNewTile == pStart)
            continue;

        pNewTile = pNewTile->GetNeighbor(mDirection);
        uint_t uiPostDistance = 1;
        while (pNewTile && !pNewTile->IsOccupied() && (uiPostDistance != uiPreDistance))
        {
            pNewTile = pNewTile->GetNeighbor(mDirection);
            ++uiPostDistance;
        }

        if (!pNewTile || pNewTile->IsOccupied())
            continue;

        auto iter = std::find(lVisitedTiles.begin(), lVisitedTiles.end(), pNewTile);
        if (iter == lVisitedTiles.end())
            lMovements.push_back(pNewTile);
    }

    return lMovements;
}

const std::vector<Slot>& Tile::GetAvailableMovements() const
{
    return lAvailableMovements_;
}
