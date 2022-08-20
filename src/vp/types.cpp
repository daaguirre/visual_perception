#include "types.h"

namespace vp
{

Matrix3d vector_to_skew(const Vector2d& v)
{
    Matrix3d skew_matrix;
    skew_matrix << 0, -1, v(1), 
                   1, 0, -v(0), 
                   -v(1), v(0), 0;
    return skew_matrix;
}

Matrix3d vector_to_skew(const Vector3d& v)
{
    Matrix3d skew_matrix;
    skew_matrix << 0, -v(2), v(1), 
                   v(2), 0, -v(0), 
                   -v(1), v(0), 0;
    return skew_matrix;
}

}