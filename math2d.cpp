

#include "math2d.h"

const atl::size2f atl::size2f::Identity(1.f, 1.f);
const atl::bounds4f atl::bounds4f::EmptyGrowableBounds(-std::numeric_limits<float>::max(),
                                                       -std::numeric_limits<float>::max(),
                                                       std::numeric_limits<float>::max(),
                                                       std::numeric_limits<float>::max());