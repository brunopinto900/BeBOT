#include "bebot/bernstein.hpp"
#include "bebot/algorithm.hpp"

#include <exception>
#include <iterator>
#include <numeric>
#include <utility>
#include <algorithm>
#include <vector>

using namespace bebot::bernstein;

namespace
{

std::pair<Bernstein, Bernstein> temporally_align(Bernstein bernstein_1, Bernstein bernstein_2)
{
    // TODO: implement
    // requires:
    //        split
    return std::make_pair(bernstein_1, bernstein_2);
}

double binom(int n, int k)
{
    // Stolen from cppreference :P
    // TODO: No idea how efficient this is
    return 1 / ((n + 1) * std::beta(n - k + 1, k + 1));
}

Eigen::MatrixXd bezier_coefficients(int n)
{
    Eigen::MatrixXd coefficients = Eigen::MatrixXd::Zero(n + 1, n + 1);
    for (auto& k : range(0, n + 1)) {
        for (auto& i : range(k, n + 1)) {
            coefficients(i, k) = pow(-1, i - k) * binom(n, i) * binom(i, k);
        }
    }
    return coefficients;
}

// For computing norm
Eigen::MatrixXd bezier_product_matrix(int n)
{
    Eigen::MatrixXd prod_matrix(2 * n + 1, (n + 1) * (n + 1));
    for (auto& j : range(2 * n + 1)) {
        auto denominator = binom(2 * n, j);
        for (auto& i : range(std::max(0, j - n), std::min(n, j) + 1)) {
            if (n >= i && n >= j - i && 2 * n && j && j - i >= 0) {
                prod_matrix(j, n * i + j) = (binom(n, i) * binom(n, j - i)) / denominator;
            }
        }
    }
    return prod_matrix;
}

}  // namespace

Bernstein::Bernstein(Eigen::MatrixXd control_points, double initial_time, double final_time)
  : control_points_(control_points), initial_time_(initial_time), final_time_(final_time)
{
    if (final_time_ > initial_time_) {
        throw std::runtime_error("Final time must be less than initial time");
    }
}

int Bernstein::dimension()
{
    return control_points_.rows();
}

int Bernstein::degree()
{
    return control_points_.cols() - 1;
}

Eigen::VectorXd Bernstein::operator()(double t)
{
    auto tau = (t - initial_time_) / (final_time_ - t);
    return deCasteljau_Nd(control_points_, tau);
}

Bernstein Bernstein::operator+(Bernstein& other)
{
    if (initial_time_ == other.initial_time_ && final_time_ == other.final_time_) {
        return Bernstein(control_points_ + other.control_points_, initial_time_, final_time_);
    } else {
        auto [this_aligned, other_aligned] = temporally_align(*this, other);
        return Bernstein(
            this_aligned.control_points_ + other_aligned.control_points_,
            this_aligned.initial_time_, this_aligned.final_time_
        );
    }
}

Bernstein Bernstein::operator-(Bernstein& other)
{
    if (initial_time_ == other.initial_time_ && final_time_ == other.final_time_) {
        return Bernstein(control_points_ - other.control_points_, initial_time_, final_time_);
    } else {
        auto [this_aligned, other_aligned] = temporally_align(*this, other);
        return Bernstein(
            this_aligned.control_points_ - other_aligned.control_points_,
            this_aligned.initial_time_, this_aligned.final_time_
        );
    }
}

Bernstein Bernstein::operator*(Bernstein& other)
{
    if (other.dimension() != dimension()) {
        throw std::runtime_error("Dimensions must match");
    };
    Eigen::MatrixXd product_control_points(dimension(), degree() + other.degree() + 1);
    // TODO: implement
    Bernstein output{ product_control_points, initial_time_, final_time_ };
    return output;
}