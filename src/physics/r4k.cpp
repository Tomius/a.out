#include "r4k.hpp"

struct State { // generalized physics state
    glm::vec3 x;
    glm::vec3 v;

    State() {}
    State(const PhysicsState& s)
        : x{s.position, s.orientation}
        , v{s.velocity, s.angular_velocity} {}

    operator PhysicsState() const {
        return {glm::vec2{x[0], x[1]}, glm::vec2{v[0], v[1]}, x[2], v[2]};
    }
};

static glm::vec3 GetAcceleration(RigidBody* body, const State& state) {
    return glm::vec3{body->GetAcceleration(state), body->GetAngularAcceleration(state)};
}

static State Evaluate(RigidBody* body, const State& state,
                      float dt, const State& d) {
    State state2;
    state2.x = state.x + d.x*dt;
    state2.v = state.v + d.v*dt;

    State d2;
    d2.x = state.v;
    d2.v = GetAcceleration(body, state2);
    return d2;
}

void R4KIntegrate(RigidBody* body, float dt) {
    if (body->inverse_mass == 0.0f) { return; }

    State state = *body;
    State a = Evaluate(body, state, 0.0f, {});
    State b = Evaluate(body, state, dt*0.5f, a);
    State c = Evaluate(body, state, dt*0.5f, b);
    State d = Evaluate(body, state, dt, c);

    State d_dt;
    d_dt.x = 1.0f / 6.0f * (a.x + 2.0f*(b.x + c.x) + d.x);
    d_dt.v = 1.0f / 6.0f * (a.v + 2.0f*(b.v + c.v) + d.v);

    state.x += d_dt.x * dt;
    state.v += d_dt.v * dt;
    *static_cast<PhysicsState*>(body) = state;
}
