#pragma once

#include <vector>
#include "rigidbody.hpp"

class ObjectLayer {
public:
    std::vector<RigidBody> objects;
    void CollideWith(ObjectLayer&);
};
