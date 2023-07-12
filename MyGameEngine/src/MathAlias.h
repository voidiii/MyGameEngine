#pragma once

#include <Vector>

#include <Vector.hpp>
#include <Matrix.hpp>
#include <Quaternion.hpp>

namespace MGE
{
    using Vec2 = mathter::Vector<float, 2, false>;
    using Vec3 = mathter::Vector<float, 3, false>;
    using Vec4 = mathter::Vector<float, 4, false>;
    using Mat33 = mathter::Matrix<float, 3, 3, mathter::eMatrixOrder::FOLLOW_VECTOR, mathter::eMatrixLayout::ROW_MAJOR, false>;
    using Mat43 = mathter::Matrix<float, 4, 3, mathter::eMatrixOrder::FOLLOW_VECTOR, mathter::eMatrixLayout::ROW_MAJOR, false>;
    using Mat34 = mathter::Matrix<float, 3, 4, mathter::eMatrixOrder::FOLLOW_VECTOR, mathter::eMatrixLayout::ROW_MAJOR, false>;
    using Mat44 = mathter::Matrix<float, 4, 4, mathter::eMatrixOrder::FOLLOW_VECTOR, mathter::eMatrixLayout::ROW_MAJOR, false>;
    using Quat = mathter::Quaternion<float, false>;
}