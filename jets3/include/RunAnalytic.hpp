/** 
 *  @file   RunAnalytic.hpp
 *  @brief  Contains simplest case that shows how the analytic calculation can be performed for the estimation of the cross section
 *
 *  This file is a part of a project hep-courses/jets3 (https://github.com/Sergeyir/hep-courses/jets3).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef RUN_ANALYTIC_HPP
#define RUN_ANALYTIC_HPP

#include <iostream>
#include <vector>
#include <filesystem>
#include <cmath>
#include <chrono>

#include "LHAPDF/LHAPDF.h"

#include "yaml-cpp/yaml.h"

#include "TFile.h"
#include "TH1D.h"
#include "TRandom.h"

/* @brief Returns random number that can be used as seed for TRandom
 */
unsigned int GetRandomSeed();
/* @brief Calculates d\sigma / d\Omega for all processes id1+id2 -> X + X for the given pT, \sqrt{s'_{NN}}, and y
 *
 * @param[in] id1 id of a 1st parton
 * @param[in] id2 id of a 2nd parton
 * @param[in] pT transverse momentum [GeV/c]
 * @param[in] s square root of s'_{NN} (' denotes parton COM frame) [GeV]
 * @param[in] y = y1 - y2
 * @param[out] d\sigma / dp_T dy_1 dy_2
 */
double GetDSigmaDOmega(const int id1, const int id2, const double pT, 
                       const double s, const double y);
/* @brief Calculates d\sigma / dp_T dy_1 dy_2 for the given pT, \sqrt{s'_{NN}}, y1, y2, x1, and x2
 *
 * @param[in] pT transverse momentum [GeV/c]
 * @param[in] s square root of s'_{NN} (' denotes parton COM frame) [GeV]
 * @param[in] y1 1st parton rapidity
 * @param[in] y2 2nd parton rapidity
 * @param[in] x1 1st parton x
 * @param[in] x2 2nd parton x 
 * @param[out] d\sigma / dp_T dy_1 dy_2
 */
double GetDSigmaDPTDY1DY2(const double pT, const double sqrtSNN, 
                          const double y1, const double y2, double &err);
/* @brief Calculates x (parton momentum fraction alongside Z axis) for the first parton for the given pT, \sqrt{s_{NN}}, y1, y2
 *
 * @param[in] pT transverse momentum [GeV/c]
 * @param[in] sqrtSNN square root of s_{NN} [GeV]
 * @param[in] y1 rapidity of a first parton
 * @param[in] y2 rapidity of a second parton
 * @param[out] x
 */
double GetX1(const double pT, const double sqrtSNN, const double y1, const double y2);

#endif /* RUN_ANALYTIC_HPP */
