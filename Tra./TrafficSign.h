#ifndef TRAFFICSIGN_H
#define TRAFFICSIGN_H

#include <string>
#include "StaticObject.h"

class TrafficSign : public StaticObject {
    private:
        std::string text;
    public:
        TrafficSign(std::string id, Position p, std::string text)
            : StaticObject(std::move(id), 'S', p),
            text(std::move(text)) {}
        const std::string& getText() const { return text; }
};

#endif
