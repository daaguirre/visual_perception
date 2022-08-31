/**
 * @file assignment4.h
 * @author daaguirre
 * @brief
 * @date 2022-08-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __ASSIGNMENT4_H_
#define __ASSIGNMENT4_H_

#include <Eigen/Eigen>
#include <filesystem>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "vp/types.h"

namespace fs = std::filesystem;

static const fs::path lab_data_dir_path = fs::path(WORKSPACE_DIR) / "tests/resources";

struct AssignmentData
{
    vp::Matrix33 K;
    vp::Matrix33 R2;
    vp::Vector3 C2;
    vp::ConstPtr<vp::Matrix3X> x1;
    vp::ConstPtr<vp::Matrix3X> x2;
    vp::ConstPtr<vp::Matrix3X> x3;
    cv::Mat img1;
    cv::Mat img2;
    cv::Mat img3;
};

/**
 * @brief implementation of Robotics Perception Assignment 4
 *
 */
void assignment4();

/**
 * @brief loads assignment data
 *
 * @return LabData struct
 */
AssignmentData load_data();

/**
 * @brief estimates fundamental matrix from assignment data
 *
 * @param assignment_data
 * @return vp::Matrix33
 */
vp::Matrix33 estimate_fundamental_matrix(const AssignmentData& assignment_data);

/**
 * @brief estimates essential matrix from fundamental matrix
 *
 * @param fundamental_matrix
 * @param K camera intrinsic parameters
 */
void estimate_essential_matrix(const vp::Matrix33& fundamental_matrix, const vp::Matrix33& K);

/**
 * @brief implementation of a structure from motion scene.
 * Includes correspondences and poin cloud visualization
 *
 * @param assignment_data
 */
void run_sfm_scene(const AssignmentData& assignment_data);

#endif  // __ASSIGNMENT4_H_
