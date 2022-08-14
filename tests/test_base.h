#ifndef __TESTBASE_H_
#define __TESTBASE_H_

#include <gtest/gtest.h>
#include <filesystem>

class TestBase : public ::testing::Test
{
protected:
    static constexpr const double PRECISION = 1e-6;
    static const std::filesystem::path RESOURCES_DIR; 

};

#endif  // __TESTBASE_H_
