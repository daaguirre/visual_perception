#include "utils.h"

namespace vp
{

Matrix33 vector_to_skew(const VectorX& v)
{
    size_t rows_to_copy = v.rows() > 3? 3: v.rows();
    Vector3 tmp_v = Vector3::Ones();
    tmp_v.topRows(rows_to_copy) = v.topRows(rows_to_copy);  

    Matrix33 skew_matrix;
    skew_matrix << 0, -tmp_v(2), tmp_v(1), 
                   tmp_v(2), 0, -tmp_v(0), 
                   -tmp_v(1), tmp_v(0), 0;
    return skew_matrix;
}

}  // namespace vp