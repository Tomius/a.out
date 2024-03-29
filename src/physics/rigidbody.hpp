#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <glm/vec2.hpp>
#include <glm/matrix.hpp>
#include "bounding_box.hpp"
#include "bounding_circle.hpp"

extern glm::vec2 kGravity;

class PhysicsScene;
struct GenereralizedPhysicsState;

struct PhysicsState {
    glm::vec2 position;
    glm::vec2 velocity;

    float orientation = 0; // in radians
    float angular_velocity = 0;
};

struct ModelSpaceBounder {
    std::vector<OrientedBoundingBox> boxes;
    std::vector<BoundingCircle> circles;
};

struct WorldSpaceBounderSnapshot {
    std::vector<OrientedBoundingBoxSnapshot> boxes;
    std::vector<BoundingCircleSnapshot> circles;

    BoundingBox aabb;
};

class RigidBody : public PhysicsState {
public:
    float inverse_mass = 0; // infitine mass == environment
    float inverse_inertia = 0;

    float static_friction = 0.6f;
    float dynamic_friction = 0.5f;
    float rolling_friction = 0.03f;
    float restitution = 0.4f;

    ModelSpaceBounder bounder;
    mutable WorldSpaceBounderSnapshot bounder_snapshot;

    void UpdateAabb () const;

public:
    virtual ~RigidBody() {}
    glm::mat3 GetMatrix() const;

    void AddBounder(OrientedBoundingBox);
    void AddBounder(BoundingCircle);

    void ApplyImpulse(glm::vec2 impulse);
    void ApplyImpulse(glm::vec2 impulse, glm::vec2 contactVector);

    void ApplyTorqueImpulse(float impulse);

    void UpdateBounderCache() const;

    virtual glm::vec2 GetAcceleration(const PhysicsState& state) const;
    virtual float GetAngularAcceleration(const PhysicsState& state) const;

    glm::vec2 GetAcceleration() const { return GetAcceleration(*this); }
    float GetAngularAcceleration() const { return GetAngularAcceleration(*this); }

    PhysicsScene& GetPhysicsScene() { return *physics_scene; }
    PhysicsScene const& GetPhysicsScene() const { return *physics_scene; }

private:
    friend class PhysicsScene;
    PhysicsScene* physics_scene = nullptr;
};
