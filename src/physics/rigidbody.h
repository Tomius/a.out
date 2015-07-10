#pragma once

#include <glm/vec2.hpp>
#include <vector>
#include <memory>
#include "boundingbox.h"
#include "boundingcircle.h"

class RigidBody {
public:
    RigidBody();
    float weight;
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 acceleration;
    std::vector<BoundingBox> bboxes;
    std::vector<BoundingCircle> bcircles;
    
    void AddBounder(BoundingBox);
    void AddBounder(BoundingCircle);
};
