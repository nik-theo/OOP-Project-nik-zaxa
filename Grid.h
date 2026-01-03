#ifndef GRID_H
#define GRID_H

#include <array>
#include "WorldObjects.h"

class Grid {
    public:
        static constexpr int width = 40;
        static constexpr int height = 40;
    private:
        std::array<std::array<WorldObject*, width>, height> cells{};
    public:
        Grid();
        bool inBounds(const Position& p) const;
        WorldObject* get(const Position& p) const;
        void set(const Position& p, WorldObject* obj);
        void remove(const Position& p);

        void clear();
};

#endif