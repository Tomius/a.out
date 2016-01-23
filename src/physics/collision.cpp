#include "collision.hpp"
#include <glm/geometric.hpp>
#include "math.hpp"

bool CanCollide(const BoundingBox& a, const BoundingBox& b) {
    return std::max(a.min.x, b.min.x) < std::min(a.max.x, b.max.x) ||
           std::max(a.min.y, b.min.y) < std::min(a.max.y, b.max.y);
}

void Collide(std::vector<Contact>& contacts,
             const BoundingCircleSnapshot& a,
             const BoundingCircleSnapshot& b) {
    // Vector from a to b
    glm::vec2 n = b.center - a.center;

    float r = a.radius + b.radius;
    float sqr_r = r*r;

    if (glm::dot(n, n) > sqr_r)
        return;

    // Circles have collided, now compute manifold
    float d = glm::length(n);

    contacts.emplace_back();
    Contact& contact = contacts.back();
    // If distance between circles is not zero
    if (d != 0) {
        // Distance is difference between radius and distance
        contact.penetration = r - d;
        // the normal is n normalized
        contact.normal = n / d;
        contact.position = a.center + contact.normal*a.radius;
    } else { // Circles are on same position
        // Choose random (but consistent) values
        contact.penetration = a.radius;
        contact.normal = glm::vec2{1, 0};
        contact.position = a.center;
    }
}

static void CollideOneWay(std::vector<Contact>& contacts,
                          const OrientedBoundingBoxSnapshot& a,
                          const OrientedBoundingBoxSnapshot& b,
                          bool flip_normals) {
    glm::vec2 half_extent = a.extent / 2.0f;
    for (const glm::vec2& point : b.points) {
        float min_penetration;
        glm::vec2 min_normal;
        bool sat_fail = false;

        for (int i = 0; i < 2; i++) {
            float x = glm::dot(a.normals[i], point - a.min);
            // the point's projection is inside the a's projection
            if (0 < x && x < 2*half_extent[i]) {
                float dist_from_center = half_extent[i] - x;
                float penetration = half_extent[i] - std::abs(dist_from_center);
                if (i == 0 || penetration < min_penetration) {
                    min_penetration = penetration;
                    min_normal = dist_from_center < 0 ? a.normals[i] : -a.normals[i];
                }
            } else {
                // if the point is not inside of one projections,
                // then the point is surely not inside a.
                sat_fail = true;
                break;
            }
        }

        if (sat_fail) { continue; }

        contacts.emplace_back();
        Contact& contact = contacts.back();
        contact.position = point;
        contact.penetration = min_penetration;
        contact.normal = flip_normals ? -min_normal : min_normal;
    }
}

void Collide(std::vector<Contact>& contacts,
             const OrientedBoundingBoxSnapshot& a,
             const OrientedBoundingBoxSnapshot& b) {
    CollideOneWay(contacts, a, b, false);
    CollideOneWay(contacts, b, a, true);
}

void Collide(std::vector<Contact>& contacts,
             const OrientedBoundingBoxSnapshot& a,
             const BoundingCircleSnapshot& b,
             bool flip_normals /*= false*/) {

    // rotate the "world", so that a becomes an AABB
    glm::vec2 b_rot_center = Math::Rotate(b.center, a.min, -a.orient);

    // precull
    if (b_rot_center.x < a.min.x - b.radius ||
        b_rot_center.y < a.min.y - b.radius ||
        b_rot_center.x > a.min.x + a.extent.x + b.radius ||
        b_rot_center.y > a.min.y + a.extent.y + b.radius) {
        return;
    }

    glm::vec2 half_extent = a.extent / 2.0f;
    glm::vec2 a_center = a.min + half_extent;

    // Vector from a to b;
    glm::vec2 n = b_rot_center - a_center;

    // Closest point on the box to center of the circle
    // (Clamp direction vector to edges of the AABB)
    glm::vec2 closest = glm::clamp(n, -half_extent, half_extent);

    bool inside = false;

    // Circle is inside the AABB, so we need to clamp the circle's center
    // to the closest edge
    if (n == closest) {
        inside = true;

        // Find closest axis
        if (std::abs(n.x) > std::abs(n.y)) {
            // Clamp to closest half_extent
            if (closest.x > 0)
                closest.x = half_extent.x;
            else
                closest.x = -half_extent.x;
        } else { // y axis is shorter
            // Clamp to closest half_extent
            if (closest.y > 0)
                closest.y = half_extent.y;
            else
                closest.y = -half_extent.y;
        }
    }

    glm::vec2 normal = n - closest;
    float d = glm::dot(normal, normal);
    float r = b.radius;

    // Early out of the radius is shorter than distance to closest point and
    // Circle not inside the AABB
    if (d > r * r && !inside)
        return;

    d = sqrt(d);

    contacts.emplace_back();
    Contact& contact = contacts.back();
    if (d < Math::kEpsilon) {
        // if normal is nullvector, pick a random, but consistent one
        contact.normal = glm::vec2{1, 0};
        d = 1;
    } else {
        // Collision normal needs to be flipped to point
        // outside if circle is inside the AABB
        contact.normal = (inside ? -normal : normal) / d;
    }

    // rotate the "world" back
    contact.normal = Math::Rotate(contact.normal, a.orient);
    contact.penetration = r - d;
    // original center, not the rotated one
    contact.position = b.center - b.radius * glm::normalize(contact.normal);

    // flip normals only after contact position is calculated
    if (flip_normals) {
        contact.normal *= -1.0;
    }
}

void Collide(std::vector<Contact>& contacts,
             const BoundingCircleSnapshot& a,
             const OrientedBoundingBoxSnapshot& b) {
    Collide(contacts, b, a, true);
}

RayTravelDistance Collide(const BoundingCircleSnapshot& circ, const Ray& ray) {
    glm::vec2 d = ray.origin - circ.center;
    float a = glm::dot (ray.direction, ray.direction);
    float b = 2 * glm::dot (ray.direction, d);
    float c = glm::dot (d, d) - circ.radius*circ.radius;

    Math::QuadraticSolitions solutions = Math::SolveQuadratic(a, b, c);
    if (solutions.num_roots == 0) {
        return RayTravelDistance{};
    } else if (solutions.num_roots == 1) {
        return solutions.roots[0];
    } else {
        return ClosestValidIntersection(solutions.roots[0], solutions.roots[1]);
    }
}

// static RayTravelDistance CollideLine(const glm::vec2& p1,
//                                      const glm::vec2& p2,
//                                      const Ray& ray) {
//     glm::vec2 origin2p1 = p1 - ray.origin;
//     glm::vec2 line_dir = glm::normalize(p2 - p1);
//     glm::vec2 normal {-line_dir.y, line_dir.x};
//     float distance_of_line_and_ray_origin = glm::dot(origin2p1, normal);
//     float dir_towards_line = glm::dot(ray.direction, normal);
//     if (std::abs(dir_towards_line) < Math::kEpsilon) { // avoid division with zero
//         return RayTravelDistance{};
//     }

//     RayTravelDistance r = distance_of_line_and_ray_origin / dir_towards_line;
//     if (r < Math::kEpsilon) {
//         return RayTravelDistance{};
//     }

//     glm::vec2 intersection = ray.origin + ray.direction * r;
//     float projection = glm::dot(intersection, line_dir);
//     float p1_proj = glm::dot(p1, line_dir);
//     float p2_proj = glm::dot(p2, line_dir);
//     if (p1_proj <= projection && projection <= p2_proj) {
//         return r;
//     } else {
//         return RayTravelDistance{};
//     }
// }

// https://rootllama.wordpress.com/2014/06/20/ray-line-segment-intersection-test-in-2d/
static RayTravelDistance CollideLine(const glm::vec2& p1,
                                     const glm::vec2& p2,
                                     const Ray& ray) {
    glm::vec2 v1 = ray.origin - p1;
    glm::vec2 v2 = p2 - p1;
    glm::vec2 v3 {-ray.direction.y, ray.direction.x};

    float t1 = Math::Cross2D(v2, v1) / glm::dot(v2, v3);
    float t2 = glm::dot(v1, v3) / glm::dot(v2, v3);

    if (0.0 <= t1 && (0.0 <= t2 && t2 <= 1.0)) {
        return t1;
    }

    return RayTravelDistance{};
}



RayTravelDistance Collide(const OrientedBoundingBoxSnapshot& bbox, const Ray& ray) {
    RayTravelDistance closest{};
    for (int i = 0; i < 4; ++i) {
        closest = ClosestValidIntersection(closest, CollideLine(bbox.points[i], bbox.points[(i+1)%4], ray));
    }
    return closest;
}
