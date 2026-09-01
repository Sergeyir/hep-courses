/** 
 *  @file   BasicHepMCReader.hpp
 *  @brief  Contains simplest case that shows how the data can be read from .hepmc file. This file was obtained by simplifying example from HepMC repository (https://gitlab.cern.ch/hepmc/HepMC3/-/blob/master/examples/BasicExamples/HepMC2_reader_example.cc?ref_type=heads)
 *
 *  This file is a part of a project genjets-course (https://github.com/Sergeyir/genjets-course).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef BASIC_HEPMC_READER_CPP
#define BASIC_HEPMC_READER_CPP

#include "BasicHepMCReader.hpp"

#include <iostream>

// To do: add the same restrictions as you use in your pythia generation code 
// and add jet reconstructin with FastJet ; after that the cross section calculation
// is no different from you pythia code

int main(int argc, char **argv) 
{
	if (argc != 2) 
   {
      std::cout << "\033[1m\033[31mError:\033[0m " << "Expected 1 parameter while " + 
                   std::to_string(argc - 1) + " parameter(s) were provided.\n"\
                   "Usage: bin/BasicHepMCReader inputFileName" << std::endl;
      return 1;
	}

   HepMC3::ReaderAsciiHepMC2 inputFile(argv[1]);

	int eventsParsed = 0;

	while (!inputFile.failed()) 
   {
      // See HepMC3::GenEvent for how to obtain more info on event:
      // https://dayabay.bnl.gov/dox/HepMC/html/classHepMC_1_1GenEvent.html
      HepMC3::GenEvent evt(HepMC3::Units::GEV, HepMC3::Units::MM);

		// Read event from input file
		inputFile.read_event(evt);

		if (eventsParsed == 0) 
      {
			std::cout << " First event: " << std::endl;
         HepMC3::Print::listing(evt);
         HepMC3::Print::content(evt);

			std::cout << " Testing attribute reading for the first event: " << std::endl;

			std::shared_ptr<HepMC3::GenCrossSection> cs = 
            evt.attribute<HepMC3::GenCrossSection>("GenCrossSection");
			std::shared_ptr<HepMC3::GenHeavyIon> hi = 
            evt.attribute<HepMC3::GenHeavyIon>("GenHeavyIon");
			std::shared_ptr<HepMC3::GenPdfInfo> pi = 
            evt.attribute<HepMC3::GenPdfInfo>("GenPdfInfo");

			if (cs) 
         {
				std::cout << " Has GenCrossSection: ";
            HepMC3::Print::line(cs);
			}
			else std::cout << " No GenCrossSection " << std::endl;

			if (pi) 
         {
				std::cout << " Has GenPdfInfo: ";
            HepMC3::Print::line(pi);
			}
			else std::cout << " No GenPdfInfo " << std::endl;

			if (hi) 
         {
				std::cout << " Has GenHeavyIon: ";
            HepMC3::Print::line(hi);
			}
			else std::cout << " No GenHeavyIon " << std::endl;

         // See HepMC3::GenParticle class reference for more info on how to obtain particle properties:
         // https://dayabay.bnl.gov/dox/HepMC/html/classHepMC_1_1GenParticle.html
         for (const auto& p : evt.particles())
         {
            // final particles have no vertex (no scattering and no interaction)
            if (p->end_vertex() == nullptr) 
            {
               // the following line shows how to obtain basic particles info
               std::cout << p->pid() << " " << p->momentum().e() << " " << p->momentum().px() << 
                            " " << p->momentum().py() << p->momentum().pz() << std::endl;
            }
         }
		}

		if (eventsParsed % 100 == 0) std::cout << "Events parsed: "<< eventsParsed << std::endl;

		++eventsParsed;
	}

	inputFile.close();

	return 0;
}

#endif /* BASIC_HEPMC_READER_CPP */
