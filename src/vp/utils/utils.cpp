#include "utils.h"

namespace vp
{

Matrix3d vector_to_skew(const VectorX& v)
{
    size_t rows_to_copy = v.rows() > 3? 3: v.rows();
    Vector3d tmp_v = Vector3d::Ones();
    tmp_v.topRows(rows_to_copy) = v.topRows(rows_to_copy);  

    Matrix3d skew_matrix;
    skew_matrix << 0, -tmp_v(2), tmp_v(1), 
                   tmp_v(2), 0, -tmp_v(0), 
                   -tmp_v(1), tmp_v(0), 0;
    return skew_matrix;
}

}  // namespace vp