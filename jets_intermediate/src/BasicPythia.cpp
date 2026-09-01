/** 
 *  @file   BasicPythia.cpp
 *  @brief  Contains simplest case that shows how the data can be generated with PYTHIA8 and LHAPDF6 and processed with FASTJET3 for jets selection 
 *
 *  This file is a part of a project genjets-course (https://github.com/Sergeyir/genjets-course).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef BASIC_PYTHIA_CPP
#define BASIC_PYTHIA_CPP

#include "BasicPythia.hpp"

int main(int argc, char **argv)
{
   // printing info on usage and exiting program if number of parameters is incorrect
   if (argc != 2) 
   {
      std::cout << "\033[1m\033[31mError:\033[0m " << "Expected 1 parameter while " + 
                   std::to_string(argc - 1) + " parameter(s) were provided.\n"\
                   "Usage: bin/BasicPythia numberOfEvents" << std::endl;
      return 1;
   }

   // collision CM energy per nucleon [GeV]
   const double energyCM = 7000.;
   // hard processes lower bound on pT [GeV/c]
   const double pTHatMin = 25.;
   // pdf set
   const std::string pdfSet = "LHAPDF6:NNPDF31_lo_as_0118";
   // fastjet R parameter
   const double fastjetR = 0.4;

   // creating Pythia8::Pythia object
   Pythia8::Pythia pythia;

   // setting pythia collision parameters
   // enabling hard QCD processes 
   // (more info on https://pythia.org/latest-manual/QCDHardProcesses.html)
   pythia.readString("HardQCD:all = on"); 
   // by default p+p collisions will be initialized since we do not set ids of incoming particles
   // (more info on https://pythia.org/latest-manual/BeamParameters.html)
   pythia.readString("Beams:eCM = " + std::to_string(energyCM));
   // setting hard process lower bound on pT [GeV/c]; processes with lower pT will not be generated
   // (more info on https://pythia.org/latest-manual/PhaseSpaceCuts.html)
   pythia.readString("PhaseSpace:pTHatMin = " + std::to_string(pTHatMin));
   // setting PDF set from LHAPDF6; pythia8 also has many predefined sets
   // (more info on https://pythia.org/latest-manual/PDFSelection.html)
   pythia.readString("PDF:pSet = " + pdfSet);
   // uncomment the next line if you do not need for pythia to print event info and banner
   //pythia.readString("Print:quiet = on");

   // initializing pythia; this step pythia applies parameters we set earlier and 
   // checks if there are problems with the current specification
	pythia.init();

   // creating directory in which the file outputFile will be written
   std::filesystem::create_directory("output");
   // file in which all histogram will be written; the following line will create 
   // (overwrite if exists) the file output/generated.root and root will point to it
   // so that TObject objects can be written in it by using method TObject::Write()
   TFile outputFile("output/basic_pythia.root", "RECREATE");
   // if another file is created after this with option "RECREATE", "UPDATE", or "CREATE"
   // root will try to write TObject objects to the new defined file
   // if you have multiple TFile files you write in in yor program use 
   // TFile::cd() to point root to the file you need

   // number of pT bins
   const int pTNBins = 1000 + static_cast<int>(pTHatMin);
   // more info on TH1D you can find on https://root.cern.ch/doc/master/classTH1.html
   // histogram in which reconstructed jets counts vs pT will be written
   TH1D distrJetsPT("Jet pT", "N(p_{T})", pTNBins, 0., 100. + pTHatMin);
   // kinematically pT can go to much higher values but for this example statistical significant result can be obtained only for this range within reasonable processing time

   // To do: change the distribution above to jet counts vs pT vs y and
   // change all following distrJetsPT instances accordingly
   // more info on TH2D you can find on https://root.cern.ch/doc/master/classTH2.html

   // you can read more about fastjet and its usage on 
   // https://indico.cern.ch/event/264054/contributions/592237/attachments/467910/648313/fastjet-doc-3.0.3.pdf
   fastjet::JetDefinition jetDef(fastjet::antikt_algorithm, fastjetR);

   // iterating over all events (we specified argv[2] to be the number of events we generate)
	for (long i = 0; i < std::stoi(argv[1]); i++)
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

         // checking if particle is stable and did not interact with anything
         if (pythia.event[j].isFinal()) 
         {
            // prints particle id (more info on id: 
            // https://pdg.lbl.gov/2007/reviews/montecarlorpp.pdf)
            
            // In experiments jets usually only consist of charged (+-1) final state particles.
            // Neutral particles are usually excluded although photons and neutral hadrons can 
            // be used and sometimes are included which is what we are going to do in this work

            // To do: add a check that only allows detectable 
            // particles to be used for jet reconstruction
            // To do: add a check that only allows particles within allowed pseudorapidity range

            // adding fastjet::PseudoJet entry (i.e. a particle) to the particles container
            particles.emplace_back(pythia.event[j].px(), // x component of momentum [GeV/c]
                                   pythia.event[j].py(), // y component of momentum [GeV/c]
                                   pythia.event[j].pz(), // z component of momentum [GeV/c]
                                   pythia.event[j].e()); // energy [GeV]
         }
      }

      if (particles.size() == 0) continue;

      // running jet clustering algorithm
      fastjet::ClusterSequence clusterSeq(particles, jetDef);
      // vector filled with jets reconstructed from vector particles
      std::vector<fastjet::PseudoJet> inclusiveJets = clusterSeq.inclusive_jets(pTHatMin);

      // iterating over reconstructed jets
      for (unsigned int j = 0; j < inclusiveJets.size(); j++)
      {
         // To do : add a check that tests whether jets are within the needed pseudorapidity range
         distrJetsPT.Fill(inclusiveJets[j].pt(), eventWeight);
      }
   }

   // printing cross section (Can you deduce what the unit of measurement for this quantity is?)
   std::cout << pythia.info.sigmaGen() << std::endl;

   // saving histograms in a TFile
   distrJetsPT.Write();

   // closing file; this is not required in the current case, however in a general case
   // it is better to close files when you are done working with them 
   // so that there are no unintended writes/reads or assignment TObject file ownership
   outputFile.Close();

   return 0;
}

#endif /* BASIC_PYTHIA_CPP */
