/** 
 *  @file   RunPythia.cpp
 *  @brief  Contains simplest case that shows how the data can be generated with PYTHIA8 and LHAPDF6 and processed with FASTJET3 for jets selection 
 *
 *  This file is a part of a project hep-courses/jets3 (https://github.com/Sergeyir/hep-courses/jets3).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef RUN_PYTHIA_CPP
#define RUN_PYTHIA_CPP

#include "RunPythia.hpp"

int main(int argc, char **argv)
{
   // printing info on usage and exiting program if number of parameters is incorrect
   if (argc != 3) 
   {
      std::cout << "\033[1m\033[31mError:\033[0m Expected 2 parameters while " <<
                   std::to_string(argc - 1) << " parameter(s) were provided \n"\
                   "Usage: bin/RunPythia inputFileName.yaml numberOfEvents" << std::endl;
      std::cout << "[\033[1m\033[32mINFO\033[0m] " << 
                   " input file example is located in input directory" << std::endl;
      return 1;
   }

   // checking if that file exists; if it doesn't exist error is printed and the program is stopped
   if (!std::filesystem::exists(argv[1]))
   {
      std::cout << "\033[1m\033[31mError:\033[0m file " << argv[1] << " was not found" << std::endl;
      return 1;
   }

   // YAML::Node object reads all data from the .yaml file
   YAML::Node inputFileContents = YAML::LoadFile(argv[1]);

   // number of events to be generated
   const unsigned long numberOfEvents = std::stoul(argv[2]);

   // reading parameters from .yaml file
   // lower we can use YAML::Node object data via array index operator[]
   const std::string pdfset = inputFileContents["pdfset"].as<std::string>();
   // To do: read and assign below the collision energy, pthatmin, pseudorapidity range, and R from the .yaml file
   const double pTHatMin = 0.;
   const double absEtaMax = 0.;
   const double energy = 0.;
   const double R = 0.1;

   // creating Pythia8:Pythia object
   // full manual is avaliable at https://pythia8.web.cern.ch/manuals/pythia8317/
   // (switch the version if you have a newer one)
   Pythia8::Pythia pythia;

   // To do: enable HardQCD and/or SoftQCD, set collision energy, pTHatMin, pdf set using pythia.readString method
   // (HardQCD: https://pythia.org/latest-manual/QCDHardProcesses.html)
   // (SoftQCD: https://pythia8.web.cern.ch/manuals/pythia8317/QCDSoftProcesses.html)
   // (Collision and beam parameters: https://pythia.org/latest-manual/BeamParameters.html)
   // (Phase space cuts: https://pythia.org/latest-manual/PhaseSpaceCuts.html)
   // (PDF selection: https://pythia.org/latest-manual/PDFSelection.html)

   // uncomment the next line if you do not need for pythia to print event info and banner
   //pythia.readString("Print:quiet = on");

   // initializing pythia; this step pythia applies parameters we set earlier and 
   // checks if there are problems with the current specification
	pythia.init();

   // creating directory in which output files will be written
   std::filesystem::create_directory("output");
   // file in which all histogram will be written; the following line will create 
   // (overwrite if exists) the file output/generated.root and root will point to it
   // so that TObject objects can be written in it by using method TObject::Write()
   TFile outputFile("output/pythia.root", "RECREATE");
   // if another file is created after this with option "RECREATE", "UPDATE", or "CREATE"
   // root will try to write TObject objects to the new defined file
   // if you have multiple TFile files you write in in yor program use 
   // TFile::cd() to point root to the file you need

   // To do: declare histograms to fill with data

   // Declaring fastjet::JetDEfinition instance using anti kt jet clustering algorithm
   // More on fastjet: https://indico.cern.ch/event/264054/contributions/592237/attachments/467910/648313/fastjet-doc-3.0.3.pdf
   fastjet::JetDefinition jetDef(fastjet::antikt_algorithm, R);

   // iterating over all events (we specified argv[2] to be the number of events we generate)
	for (unsigned long i = 0; i < numberOfEvents; i++)
   {
      // this tells pythia to generate next event
		if (!pythia.next()) continue;

      // vector that will be filled with all final state particles that passed all cuts
      std::vector<fastjet::PseudoJet> particles;
      
      // to get information on cross sections, weights, etc. use info on
      // https://pythia.org/latest-manual/CrossSectionsAndWeights.html

      // weight for the current event
      const double eventWeight = pythia.info.weight();
      // Exercise: Can you say if the event weight is needed in p+p and why?

      // to get information of a pythia event you can use info on
      // https://pythia.org/latest-manual/EventRecord.html

      // iterating over all particles in a current event
		for (int j = 0; j < pythia.event.size(); j++)
      {
         // to get information of a particle you can use info on 
         // https://pythia.org/latest-manual/ParticleProperties.html

         // prints particle id (more info on id: 
         // https://pdg.lbl.gov/2007/reviews/montecarlorpp.pdf)
         
         // in experiments jets usually only consist of charged (+-1q) final state particles
         // neutral particles need to be excluded 
         // (although photons and neutral hadrons can be used and sometimes are included)
         // if (abs(pythia.event[j].charge()) != 1) continue;
         // but for this work all particles will be included

         // adding fastjet::PseudoJet entry to the particles vector
         particles.emplace_back(pythia.event[j].px(), // x component of momentum [GeV/c]
                                pythia.event[j].py(), // y component of momentum [GeV/c]
                                pythia.event[j].pz(), // z component of momentum [GeV/c]
                                pythia.event[j].e()); // energy [GeV]
      }

      // running jet clustering algorithm
      fastjet::ClusterSequence clusterSeq(particles, jetDef);
      // vector filled with jets reconstructed from vector particles
      std::vector<fastjet::PseudoJet> inclusiveJets = clusterSeq.inclusive_jets(pTHatMin);

      // iterating over reconstructed jets
      for (unsigned int j = 0; j < inclusiveJets.size(); j++)
      {
         // To do : add a check that tests whether jets are within the needed pseudorapidity range
         // To obtain pT of a jet use inclusiveJets[j].pt()
      }
   }

   // printing cross section (Can you deduce what the unit of measurement for this quantity is?)
   std::cout << pythia.info.sigmaGen() << std::endl;

   // closing file; this is not required in the current case, however in a general case
   // it is better to close files when you are done working with them 
   // so that there are no unintended writes/reads or assignment TObject file ownership
   outputFile.Close();

   return 0;
}

#endif /* RUN_PYTHIA_CPP */
