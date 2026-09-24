/**
 *  @file   RunPythia.cpp
 *  @brief  Contains simplest case that shows how the data can be generated with PYTHIA8+Angantyr and LHAPDF6
 *
 *  This file is a part of a project hep-courses/hadron_production (https://github.com/Sergeyir/hep-courses/hadron_production).
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
                   "Usage: bin/RunPythia inputFile.cmnd numberOfEvents" << std::endl;
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

   // number of events to be generated
   const unsigned long numberOfEvents = std::stoul(argv[2]);

   // creating Pythia8:Pythia object
   // full manual is avaliable at https://pythia8.web.cern.ch/manuals/pythia8317/
   // (switch the version if you have a newer one)
   Pythia8::Pythia pythia;

   // To do: determine whether HardQCD and/or SoftQCD needed in this analysis
   // Hint: look at SoftQCD:nonDiffractive definition (remind: you perform a minimum bias study)
   // HardQCD: https://pythia.org/latest-manual/QCDHardProcesses.html
   //pythia.readString("HardQCD:all = on");
   // SoftQCD: https://pythia8.web.cern.ch/manuals/pythia8317/QCDSoftProcesses.html
   pythia.readString("SoftQCD:nonDiffractive = on");
   // setting parameters from .cmnd input file
   pythia.readFile(argv[1]);
   // More information on some pythia flags:
   // (Collision and beam parameters: https://pythia.org/latest-manual/BeamParameters.html)
   // (PDF selection: https://pythia.org/latest-manual/PDFSelection.html)

   // random generator seed; main can be changed to have seed argument. Seed is needed if you intend to perform calculations on multiple threads via bash scripts
   const int seed = 1;
   // setting the random seed
   pythia.readString("Random:seed = " + std::to_string(seed));
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
   // To do: come up with idea on how to set unique names for output files, so that each 
   // calculation of different collision systems would not overwrite the same file
   // Hint: you can append unique system identifier to the file 
   // (for example by setting a unique comment in .cmnd file, then extracting its contents
   // with ifstream, and finally appending to the output TFile name)
   TFile outputFile("output/pp.root", "RECREATE");
   // if another file is created after this with option "RECREATE", "UPDATE", or "CREATE"
   // root will try to write TObject objects to the new defined file
   // if you have multiple TFile files you write in in yor program use
   // TFile::cd() to point root to the file you need

   // To do: declare histograms to store numner of event charged particles multiplicity an
   // pT multiplicity vs number of charged particles
   // 1-D histograms: https://root.cern.ch/doc/master/classTH1.html
   // 2-D histograms: https://root.cern.ch/doc/master/classTH2.html
   
   // example: pT multiplicity
   TH1D hist("pT multiplicity", "", 200, 0., 25.);

   // iterating over all events (we specified argv[2] to be the number of events we generate)
	for (unsigned long i = 0; i < numberOfEvents; i++)
   {
      // this tells pythia to generate next event
		if (!pythia.next()) continue;

      // to get information on cross sections, weights, etc. use info on
      // https://pythia.org/latest-manual/CrossSectionsAndWeights.html

      // weight for the current event
      const double eventWeight = pythia.info.weight();
      // Exercise: Can you say if the event weight is needed and why?

      // to get information of a pythia event you can use info on
      // https://pythia.org/latest-manual/EventRecord.html

      // iterating over all particles in a current event
		for (int j = 0; j < pythia.event.size(); j++)
      {
         // to get information of a particle you can use info on
         // https://pythia.org/latest-manual/ParticleProperties.html

         if (i == 0)
         {
            // prints current particle id (more info on id:
            // https://pdg.lbl.gov/2007/reviews/montecarlorpp.pdf)
            std::cout << pythia.event[j].id() << std::endl;
         }
         hist.Fill(pythia.event[j].pT(), eventWeight);
      }
   }

   // printing cross section (Can you deduce what the unit of measurement for this quantity is?)
   std::cout << pythia.info.sigmaGen() << std::endl;

   hist.Write();  

   // closing file; this is not required in the current case, however in a general case
   // it is better to close files when you are done working with them
   // so that there are no unintended writes/reads or assignment TObject file ownership
   outputFile.Close();

   return 0;
}

#endif /* RUN_PYTHIA_CPP */
