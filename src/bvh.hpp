#pragma once

#include <common.hpp>
#include <hit.hpp>
#include <hittable_list.hpp>

#include <algorithm>

struct BvhNode : public HittableI {
public:
    BvhNode() = default;
    BvhNode(hittable_list const & hittables, TimeInterval times);
    BvhNode(std::vector<std::shared_ptr<HittableI>> hittables,
            TimeInterval times);

    // HittableI
    Aabb bounding_box(TimeInterval times) const override;
    HitRecord hit(Ray const & r, double t_min, double t_max) const override;

    std::shared_ptr<HittableI> left;
    std::shared_ptr<HittableI> right;
    Aabb aabb{AabbBounds{vec3{0.0}, vec3{-1.0}}};
};

// compare HittableI's bounding_boxes minimum corners along a given axis
template <int axis>
inline bool compare_along_axis(std::shared_ptr<HittableI> const a, std::shared_ptr<HittableI> const b) {
    // assume that every HittableI will return a valid bounding box
    return a->bounding_box(TimeInterval{}).min()[axis] < b->bounding_box(TimeInterval{}).min()[axis];
}

inline bool compare_along_x_axis(std::shared_ptr<HittableI> const a, std::shared_ptr<HittableI> const b) {
    return compare_along_axis<0>(a, b);
}

inline bool compare_along_y_axis(std::shared_ptr<HittableI> const a, std::shared_ptr<HittableI> const b) {
    return compare_along_axis<1>(a, b);
}

inline bool compare_along_z_axis(std::shared_ptr<HittableI> const a, std::shared_ptr<HittableI> const b) {
    return compare_along_axis<2>(a, b);
}

inline Aabb BvhNode::bounding_box(TimeInterval times) const {
    return aabb;
}

inline BvhNode::BvhNode(hittable_list const & hittables, TimeInterval times)
    : BvhNode{hittables.objects, times} {}

inline BvhNode::BvhNode(std::vector<std::shared_ptr<HittableI>> hittables,
                        TimeInterval times) {
    if (hittables.empty())
        return;
    else if (hittables.size() <= 2) {
        left = hittables.front();
        right = hittables.back();
    }
    else {
        auto const axis = random_int(0, 2);
        auto const compare = axis == 0 ? compare_along_x_axis :
                             axis == 1 ? compare_along_y_axis :
                                         compare_along_z_axis;
        std::sort(std::begin(hittables), std::end(hittables), compare);
        auto const middle = std::cbegin(hittables) + hittables.size() / 2;
        left = std::make_shared<BvhNode>(std::vector(std::cbegin(hittables), middle), times);
        right = std::make_shared<BvhNode>(std::vector(middle, std::cend(hittables)), times);
    }

    // assume that every HittableI will return a valid bounding box
    aabb = surrounding_box(left->bounding_box(times), right->bounding_box(times));
}

std::ostream & operator<<(std::ostream & out, Aabb const & aabb) {
    return out << aabb.min() << ", " << aabb.max();
}

HitRecord BvhNode::hit(Ray const & ray, double t_min, double t_max) const {
    if (!aabb.hit(ray, TimeInterval(t_min, t_max)))
        return HitRecord::miss();

    auto const hit_left = left->hit(ray, t_min, t_max);
    auto const hit_right = right->hit(ray, t_min, hit_left ? hit_left.t : t_max);
    return hit_right ? hit_right : hit_left;
}
