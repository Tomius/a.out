#pragma once

#include "circle.hpp"
#include "rectangle.hpp"

namespace GameObjects
{

// this will be probably useful for something
class Character : public GameObject {
public:
    PhysicsState state;

    void Draw() override {

    }
};

}
