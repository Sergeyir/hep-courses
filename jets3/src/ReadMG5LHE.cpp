/** 
 *  @file   ReadMG5LHE.cpp
 *  @brief  Contains simplest example on how to read data from .lhe file obtained from MadGraph with HepMC3
 *
 *  This file is a part of a project hep-courses/jets3 (https://github.com/Sergeyir/hep-courses/jets3).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef READ_MG5_LHE_CPP
#define READ_MG5_LHE_CPP

#include "ReadMG5LHE.hpp"

int main(int argc, char **argv)
{
   // printing info on usage and exiting the program if number of parameters is incorrect
   if (argc != 3) 
   {
      std::cout << "\033[1m\033[31mError:\033[0m Expected 2 parameters while " + 
                   std::to_string(argc - 1) + " parameter(s) were provided.\n"\
                   "Usage: bin/ReadMG5LHE input_file_name output_file_name" << std::endl;
      return 1;
   }

   // Checking if the specified file exist
   if (!std::filesystem::exists(argv[1]))
   {
      std::cout << "\033[1m\033[31mError:\033[0m File " << argv[1] << " does not exist" << std::endl;
      return 1;
   }

   std::cout << "Reading from file " << argv[1] << std::endl;

   // To do: create histograms that you will fill and TFile using argv[2] for the name for it
   // (similar to what you have done in RunPythia.cpp)

   // Initializing reader for LHE file. Reader is used to access information stored in .lhe file
   LHEF::Reader reader(argv[1]);

   unsigned long numberOfEvents = 0;
   // iterating over events
   // I intentionally ommited the weight calcualtions as events are unweighted
   while (reader.readEvent())
   {
      numberOfEvents++;

      if (numberOfEvents % 1000 == 0) std::cout << "Event " << numberOfEvents << "\r" << std::endl;

      // HepMC needs this object to store the event, tags, XML, and other data into
      std::shared_ptr<HepMC3::HEPEUPAttribute> hepe = std::make_shared<HepMC3::HEPEUPAttribute>();

      // Storing reader event data into HepMC3::HEPEUPAttribute object
      hepe->hepeup = reader.hepeup;

      // To do: come up with an idea on how to process only outgoing partons
      // Iterating over every particles in the current event
      for (int i = 0; i < hepe->hepeup.NUP; i++)
      {
         // https://dayabay.bnl.gov/dox/HepMC/html/classHepMC_1_1GenParticle.html
         HepMC3::GenParticlePtr particle = 
            std::make_shared<HepMC3::GenParticle>(hepe->momentum(i), 
                                                  hepe->hepeup.IDUP[i], 
                                                  hepe->hepeup.ISTUP[i]);

         // creating 4-vector for a current particle
         HepMC3::FourVector fourVec = particle->momentum();

         // printing id and 4-momentum components of each particle in the first event
         if (numberOfEvents == 1)
         {
            std::cout << particle->pid() << " " << fourVec.e() << " " << 
                         fourVec.px() << " " << fourVec.py() << " " << fourVec.pz() << std::endl;
         }
      }
   }
   
   // Find the measurement units for the total cross section
   std::cout << "Total cross section: " << GetTotCrossSection(argv[1]) << std::endl;

   return 0;
}

double GetTotCrossSection(const std::string& fileName)
{
   std::ifstream file(fileName);

   // usning regular expression to set the pattern that would match the line we want to read
   std::regex re(R"(^#\s*Integrated weight \(pb\)\s*:\s*([0-9.+-eE]+))");

   std::string line;
   // iterating over all lines in a file and trying to find a match
   while (std::getline(file, line))
   {
      std::smatch m;
      // if match found return the result
      if (std::regex_search(line, m, re)) return std::stod(m[1].str());
      // stopping if header was passed
      if (line.find("<event") != std::string::npos) break;
   }

   std::cout << "\033[1m\033[31mError:\033[0m cross-section comment line was "\
                "not found in file" + fileName << std::endl;
   exit(1);
}

#endif /* READ_MG5_LHE_CPP */
