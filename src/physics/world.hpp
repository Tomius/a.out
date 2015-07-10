#pragma once

#include <vector>
#include <map>
#include "objectlayer.h"
#include "rigidbody.h"


template <unsigned layerno>
class World {
    std::vector<ObjectLayer> layers[layerno];
public:
    std::map<unsigned, unsigned> colliding;
    void addObject(RigidBody& rb) {
        layers[layerno / 2].objects.push_back(rb);
    }
    void addToLayer(RigidBody& rb, unsigned num) {
        if(num >= layerno) {
            throw "Invalid layer number specified.";
        }
        layers[num].objects.push_back(rb);
    }
    void findCollisions() {
        for(auto& layer : layers) {
            layer.CollideWith(layer);
        }
        for(auto& coll : colliding) {
            layers[coll.first].CollideWith(layers[coll.second]);
        }
    }
};
