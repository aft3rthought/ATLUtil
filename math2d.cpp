

#include "math2d.h"

const atl::size2f atl::size2f::Identity(1.f, 1.f);
const atl::point2f atl::point2f::AxisX(1.f, 0.f);
const atl::point2f atl::point2f::AxisY(0.f, 1.f);
const atl::point2f atl::point2f::Zero(0.f, 0.f);
const atl::rangef atl::rangef::Max(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
const atl::rangef atl::rangef::InvertedMax(std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
const atl::box2f atl::box2f::MaxInvertedBounds(atl::rangef::InvertedMax, atl::rangef::InvertedMax);
const atl::box2f atl::box2f::MaxBounds(atl::rangef::Max, atl::rangef::Max);