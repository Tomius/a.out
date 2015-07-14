#include "objectlayer.hpp"
#include "manifold.hpp"

void ObjectLayer::CollideWith(ObjectLayer& other) {
    for(auto& first : objects) {
        for(auto& second : other.objects) {
            Manifold m(first, second);
            m.ApplyImpulse();
        }
    }
}
