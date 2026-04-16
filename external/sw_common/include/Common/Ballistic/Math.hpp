#pragma once

#include <cmath>
#include <iostream>
#include <optional>

#include "Common/OpenCV/Common.hpp"
#include "Common/Type/Angle.hpp"

/**
 * @brief Computes the interception time for a bullet to hit a moving target.
 *
 * Solves the equation |targetPos + targetVel * t| = bulletSpeed * t for t.
 * If no valid positive solution exists, returns std::nullopt.
 *
 * @param targetPos   Initial position of the target relative to the turret at the origin.
 * @param targetVel   Velocity vector of the target.
 * @param bulletSpeed Speed of the bullet.
 * @return The smallest positive interception time in seconds, or std::nullopt if no solution.
 */
std::optional< double >
solveInterceptTime(const cv::Point3d& targetPos, const cv::Point3d& targetVel, double bulletSpeed);

/**
 * @brief Calculates the pitch and yaw angles required to lead a turret to hit a moving target.
 *
 * Uses SolveInterceptTime to find the intercept time, computes the intercept position,
 * and derives yaw and pitch angles based on that position.
 *
 * @param targetPos   Initial position of the target relative to the turret at the origin.
 * @param targetVel   Velocity vector of the target.
 * @param bulletSpeed Speed of the bullet in meter/seconds.
 * @param[out] outPitch Calculated pitch angle (radians).
 * @param[out] outYaw   Calculated yaw angle (radians).
 * @return true if a valid interception solution was found; false otherwise.
 */
std::optional< Angle2 >
calculateLeadAngles(const cv::Point3d& targetPos, const cv::Point3d& targetVel, double bulletSpeed);