#include "Grid.h"

Grid::Grid() {
    clear();
}

bool Grid::inBounds(const Position& p) const {
    return p.x >= 0 && p.x < width && p.y >= 0 && p.y < height;
}

WorldObject* Grid::get(const Position& p) const {
    if(!inBounds(p)) {
        return nullptr;
    }
    return cells[p.y][p.x];
}

void Grid::set(const Position& p, WorldObject* obj) {
    if (!inBounds(p)) {
        return;
    }
    cells[p.y][p.x] = obj;
}

void Grid::remove(const Position& p) {
    if (!inBounds(p)) {
        return;
    }
    cells[p.y][p.x] = nullptr;
}

void Grid::clear() {
    for (auto& row: cells) {
        for (auto& cell: row) {
            cell = nullptr;
        }
    }
}