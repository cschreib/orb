#ifndef TILE_H
#define TILE_H

#include "utils.h"
#include "orb.h"

using Vector2D = Point<float>;
using Slot = Point<int>;

class Orb;

class Tile
{
public :

    enum Type
    {
        NORMAL = -1,
        HOME_BLUE = 0,
        HOME_RED,
        HOME_GREEN,
        HOME_PINK
    };

    Tile(const Slot& mSlot, Type mType);

    const Slot& GetSlot() const;

    bool IsOnSlot(const Slot& mSlot) const;
    void AddNeighbor(Tile* pTile);
    Tile* GetNeighbor(const Slot& mDirection) const;
    void SetOrb(Orb* pOrb);
    Orb* GetOrb() const;
    bool IsOccupied();
    void ComputeAvailableMovements();

    const std::vector<Slot>& GetAvailableMovements() const;

private :

    std::vector< Tile* > GetFirstStepNeighbors_(Tile* pStart, const std::vector< Tile* >& lVisitedTiles) const;

    Slot mSlot_;
    Type mType_;
    Orb* pOrb_ = nullptr;

    std::vector<Slot> lAvailableMovements_;
    std::vector< Tile* > lNeighborList_;
};

#endif
