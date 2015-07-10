#include "objectlayer.h"

#include "collisionhelper.h"

void ObjectLayer::CollideWith(ObjectLayer& other) {
    for(auto& first : objects) {
        for(auto& second : other.objects) {
            Collide(first, second);
            // ... handle collision if necessary
        }
    }
}
