// Copyright 2024 TIER IV, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef AUTOWARE__TRAJECTORY__INTERPOLATOR__INTERPOLATOR_HPP_
#define AUTOWARE__TRAJECTORY__INTERPOLATOR__INTERPOLATOR_HPP_

#include "autoware/trajectory/interpolator/detail/interpolator_common_interface.hpp"

#include <memory>
#include <vector>

namespace autoware::experimental::trajectory::interpolator
{
/**
 * @brief Template class for interpolation.
 *
 * This class serves as the base class for specific interpolation types.
 *
 * @tparam T The type of the values being interpolated. (e.g. double, int, etc.)
 */
template <typename T>
class InterpolatorInterface : public detail::InterpolatorCommonInterface<T>
{
public:
  virtual std::shared_ptr<InterpolatorInterface<T>> clone() const = 0;
};

/**
 * @brief Specialization of Interpolator for double values.
 *
 * This class adds methods for computing first and second derivatives.
 */
template <>
class InterpolatorInterface<double> : public detail::InterpolatorCommonInterface<double>
{
protected:
  /**
   * @brief Compute the first derivative at the given point.
   *
   * This method should be overridden by subclasses to provide the specific logic.
   *
   * @param s The point at which to compute the first derivative.
   * @return The first derivative.
   */
  virtual double compute_first_derivative_impl(const double s) const = 0;

  /**
   * @brief Compute the second derivative at the given point.
   *
   * This method should be overridden by subclasses to provide the specific logic.
   *
   * @param s The point at which to compute the second derivative.
   * @return The second derivative.
   */
  virtual double compute_second_derivative_impl(const double s) const = 0;

public:
  /**
   * @brief Compute the first derivative at the given point.
   *
   * @param s The point at which to compute the first derivative.
   * @return The first derivative.
   */
  double compute_first_derivative(const double s) const
  {
    const double clamped_s = this->validate_compute_input(s);
    return compute_first_derivative_impl(clamped_s);
  }

  /**
   * @brief Compute the first derivative at the given points.
   *
   * @param s The points at which to compute the first derivative.
   * @return The first derivatives.
   */
  std::vector<double> compute_first_derivative(const std::vector<double> & ss) const
  {
    std::vector<double> d;
    for (const auto s : ss) {
      const double clamped_s = this->validate_compute_input(s);
      d.push_back(compute_first_derivative_impl(clamped_s));
    }
    return d;
  }

  /**
   * @brief Compute the second derivative at the given point.
   *
   * @param s The point at which to compute the second derivative.
   * @return The second derivative.
   */
  double compute_second_derivative(const double s) const
  {
    const double clamped_s = this->validate_compute_input(s);
    return compute_second_derivative_impl(clamped_s);
  }

  /**
   * @brief Compute the second derivative at the given points.
   *
   * @param s The points at which to compute the second derivative.
   * @return The second derivatives.
   */
  std::vector<double> compute_second_derivative(const std::vector<double> & ss) const
  {
    std::vector<double> d;
    for (const auto s : ss) {
      const double clamped_s = this->validate_compute_input(s);
      d.push_back(compute_second_derivative_impl(clamped_s));
    }
    return d;
  }

  virtual std::shared_ptr<InterpolatorInterface<double>> clone() const = 0;
};

}  // namespace autoware::experimental::trajectory::interpolator

#endif  // AUTOWARE__TRAJECTORY__INTERPOLATOR__INTERPOLATOR_HPP_
