#pragma once

#include "physics/rigidbody.hpp"

// Runge-kutta fourth order integrate state
void R4KIntegrate(RigidBody* body, float dt);
