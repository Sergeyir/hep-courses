/** 
 *  @file   ReadMG5LHE.hpp
 *  @brief  Contains simplest example on how to read data from .lhe file obtained from MadGraph with HepMC3
 *
 *  This file is a part of a project hep-courses/jets3 (https://github.com/Sergeyir/hep-courses/jets3).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef READ_MG5_LHE_HPP
#define READ_MG5_LHE_HPP

#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>

#include "HepMC3/LHEFAttributes.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"

#include "TFile.h"
#include "TH2.h"

/// Returns the total cross section from the specified .lhe file
double GetTotCrossSection(const std::string& fileName);

#endif /* READ_MG5_LHE_HPP */
