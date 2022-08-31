/**
 * @file pnp_algorithm.h
 * @author daaguirre
 * @brief implements the Perspective-N-point algorithm for estimating
 * the camera pose (R, t) from 3D-2D corresponces
 * @date 2022-08-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __VP_PNP_ALGORITHM_H_
#define __VP_PNP_ALGORITHM_H_

#include "types.h"

namespace vp
{

class PNPAlgorithm
{
public:
    CameraPose run(const Matrix4X& world_points, const Matrix3X& pixels, const Matrix33& K);

private:
    /**
     * @brief factors out the camera intrinsic parameters from the pixel values
     *
     * @param K camera intrinsic parameters matrix
     * @param pixels pixel values
     * @return Matrix
     */
    MatrixX factor_out_intrinsic_parameters(const Matrix33& K, const Matrix3X& pixels);
};

}  // namespace vp

#endif  // __VP_PNP_ALGORITHM_H_
