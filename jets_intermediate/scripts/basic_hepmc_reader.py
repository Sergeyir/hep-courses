#!/usr/bin/env python3

#  @file   basic_hepmc_reader.py
#  @brief  Contains simplest case that shows how the data can be read from .hepmc file. This file was obtained by rewriting and simplifying the example from HepMC repository (https://gitlab.cern.ch/hepmc/HepMC3/-/blob/master/examples/BasicExamples/HepMC2_reader_example.cc?ref_type=heads)
#
#  This file is a part of a project genjets-course (https://github.com/Sergeyir/genjets-course).
#
#  @author Sergei Antsupov (antsupov0124@gmail.com)

import os
import sys
import argparse

from pyHepMC3 import HepMC3

"""
To do: add the same restrictions as you use in your pythia generation code 
and add jet reconstructin with FastJet ; after that the cross section calculation
is no different from you pythia code
"""

def read_hepmc(input_file_name : str) -> int :

    input_file = HepMC3.ReaderAsciiHepMC2(input_file_name)

    events_parsed = 0

    while not input_file.failed() :
        # See HepMC3::GenEvent for how to obtain more info on event:
        # https://dayabay.bnl.gov/dox/HepMC/html/classHepMC_1_1GenEvent.html
        evt = HepMC3.GenEvent(HepMC3.Units.GEV, HepMC3.Units.MM)

		# Read event from input file
        input_file.read_event(evt)

        if events_parsed == 0 :
            print('First event')
            HepMC3.Print.listing(evt)
            HepMC3.Print.content(evt)

            print('Testing attribute reading for the first event:')

            cs = evt.cross_section()

            if cs :
                print('Has GenCrossSection: ')
                HepMC3.Print.line(cs)
            else :
                print('No GenCrossSection info')

            # See HepMC3::GenParticle class reference for more info on how to obtain particle properties:
            # https://dayabay.bnl.gov/dox/HepMC/html/classHepMC_1_1GenParticle.html
            for p in evt.particles() :
                # final particles have no vertex (no scattering and no interaction)
                if not p.end_vertex() :
                    # the following line shows how to obtain basic particles info
                    print(p.pid(), p.momentum().e(), p.momentum().px(), p.momentum().py(), p.momentum().pz())
            
        if events_parsed % 100 == 0 :
            print(f'Events parsed: {events_parsed}')

        events_parsed += 1

    return 0

if __name__ == "__main__":
    # parsing input arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--input", type=str)

    args = parser.parse_args()
    input_file_name = args.input

    if not input_file_name:
        print('\033[1;31mError:\033[0m .hepmc file name must be specified; use option -i or --input')
        sys(exit(1))

    sys.exit(read_hepmc(input_file_name))
