#pragma once

#include <vector>
#include "rigidbody.h"

class ObjectLayer {
public:
    std::vector<RigidBody> objects;
    void CollideWith(ObjectLayer&);
};
