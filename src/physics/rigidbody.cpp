#include "rigidbody.h"

RigidBody::RigidBody() {
    weight = -1.;
}

void RigidBody::AddBounder(BoundingBox bbox) {
    bboxes.push_back(bbox);
}

void RigidBody::AddBounder(BoundingCircle bcircle) {
    bcircles.push_back(bcircle);
}
