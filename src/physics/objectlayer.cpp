#include "objectlayer.hpp"
#include "collisionhelper.hpp"

void ObjectLayer::CollideWith(ObjectLayer& other) {
    for(auto& first : objects) {
        for(auto& second : other.objects) {
            Collide(first, second);
            // ... handle collision if necessary
        }
    }
}
