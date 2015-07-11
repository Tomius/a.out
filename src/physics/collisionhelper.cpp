#include "collisionhelper.hpp"

Manifold Collide(RigidBody& first, RigidBody& second) {
    Manifold m;
    m.first = &first;
    m.second = &second;
    for(auto& bc : first.bcircles) {
        for(auto& sbc : second.bcircles) {
            if(&bc != &sbc) {
                auto contacts = Collide(bc, sbc);
                for(auto& contact : contacts) {
                    m.contacts.push_back(contact);
                }
            }
        }
        for(auto& sbc : second.bboxes) {
            auto contacts = Collide(bc, sbc);
            for(auto& contact : contacts) {
                m.contacts.push_back(contact);
            }
        }
    }
    for(auto& bc : first.bboxes) {
        for(auto& sbc : second.bboxes) {
            auto contacts = Collide(bc, sbc);
            for(auto& contact : contacts) {
                m.contacts.push_back(contact);
            }
        }
    }
    return m;
}

std::vector<Contact> Collide(BoundingCircle first, BoundingCircle second) {
    return std::vector<Contact>();
}

std::vector<Contact> Collide(BoundingBox first, BoundingBox second) {
    return std::vector<Contact>();
}

std::vector<Contact> Collide(BoundingCircle first, BoundingBox second) {
    return std::vector<Contact>();
}

std::vector<Contact> Collide(BoundingBox first, BoundingCircle second) {
    return Collide(second, first);
}
