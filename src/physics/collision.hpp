#pragma once

#include <vector>
#include "ray.hpp"
#include "physics/contact.hpp"
#include "physics/bounding_box.hpp"
#include "physics/bounding_circle.hpp"

bool CanCollide(const BoundingBox& a, const BoundingBox& b);

void Collide(std::vector<Contact>& contacts,
             const BoundingCircleSnapshot& a,
             const BoundingCircleSnapshot& b);

void Collide(std::vector<Contact>& contacts,
             const OrientedBoundingBoxSnapshot& a,
             const OrientedBoundingBoxSnapshot& b);

void Collide(std::vector<Contact>& contacts,
             const OrientedBoundingBoxSnapshot& a,
             const BoundingCircleSnapshot& b,
             bool flip_normals = false);

void Collide(std::vector<Contact>& contacts,
             const BoundingCircleSnapshot& a,
             const OrientedBoundingBoxSnapshot& b);

RayTravelDistance Collide(const BoundingCircleSnapshot& circ, const Ray& ray);
RayTravelDistance Collide(const OrientedBoundingBoxSnapshot& box, const Ray& ray);
