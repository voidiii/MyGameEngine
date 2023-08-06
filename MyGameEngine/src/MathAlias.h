#pragma once

#include <Vector>

#include <Vector.hpp>
#include <Matrix.hpp>
#include <Quaternion.hpp>

namespace MGE
{
    using Vec2 = mathter::Vector<float, 2, true>;
    using Vec2_Physics = mathter::Vector<float, 2, true > ;
    using Vec3 = mathter::Vector<float, 3, true>;
    using Vec4 = mathter::Vector<float, 4, true>;
    using Mat22 = mathter::Matrix<float, 2, 2, mathter::eMatrixOrder::PRECEDE_VECTOR, mathter::eMatrixLayout::COLUMN_MAJOR, false>;
    using Mat33 = mathter::Matrix<float, 3, 3, mathter::eMatrixOrder::PRECEDE_VECTOR, mathter::eMatrixLayout::COLUMN_MAJOR, false>;
    using Mat43 = mathter::Matrix<float, 4, 3, mathter::eMatrixOrder::PRECEDE_VECTOR, mathter::eMatrixLayout::COLUMN_MAJOR, false>;
    using Mat34 = mathter::Matrix<float, 3, 4, mathter::eMatrixOrder::PRECEDE_VECTOR, mathter::eMatrixLayout::COLUMN_MAJOR, false>;
    using Mat44 = mathter::Matrix<float, 4, 4, mathter::eMatrixOrder::PRECEDE_VECTOR, mathter::eMatrixLayout::COLUMN_MAJOR, false>;
    using Quat = mathter::Quaternion<float, false>;

    inline int32_t f_toint32(float x)
    {
        //取得符号位，设置掩码
        uint32_t n = ((*(uint32_t*)&x) & 0x80000000) ? 0xFFC00000 : 0;
        x += 12582912.0f;
        return ((*(uint32_t*)&x) & 0x3FFFFF) | n;
    }

}