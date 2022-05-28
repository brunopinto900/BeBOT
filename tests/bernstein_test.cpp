#include "bebot/bernstein.hpp"
#include "bebot/algorithm.hpp"

#include <Eigen/Dense>
#include <gtest/gtest.h>

TEST(algorithms, deCasteljau_1d_linear)
{
    // Two control points
    Eigen::VectorXd control_points{ { 1, 2 } };
    double out = 1.5;
    double t = 0.5;
    // Equivalent to lerp(1,2, t=0.5)
    EXPECT_FLOAT_EQ(deCasteljau_1d(control_points, t), out);
}

TEST(algorithms, deCasteljau_Nd_linear)
{
    // Two control points, 2D
    // Lerp from
    // [1]   -->  [2]
    // [3]        [4]
    Eigen::MatrixXd control_points{ { 1, 2 }, { 3, 4 } };
    Eigen::VectorXd out{ { 1.5, 3.5 } };
    EXPECT_TRUE(deCasteljau_Nd(control_points, 0.5).isApprox(out));
}

TEST(algorithms, deCastlejau_Nd_cubic)
{
    // Four control points in 2D
    Eigen::MatrixXd control_points{ { 1, 2, 3, 4 }, { 2, 3, 4, 5 } };

    double t_1 = 0.5;
    double t_2 = 0.7;

    Eigen::VectorXd out_t_1{ { 2.5, 3.5 } };
    Eigen::VectorXd out_t_2{ { 3.1, 4.1 } };

    EXPECT_TRUE(deCasteljau_Nd(control_points, t_1).isApprox(out_t_1));
    EXPECT_TRUE(deCasteljau_Nd(control_points, t_2).isApprox(out_t_2));
}

TEST(bernstein, call)
{
    // Four control points in 2D
    Eigen::MatrixXd control_points{ { 1, 2, 3, 4 }, { 2, 3, 4, 5 } };

    double t_i = 10;
    double t_f = 20;
    double t = t_i + (t_f - t_i) / 2;
    bebot::bernstein::Bernstein poly{ control_points, t_i, t_f };

    Eigen::VectorXd out{ { 2.5, 3.5 } };

    EXPECT_TRUE(poly(t).isApprox(out));
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
