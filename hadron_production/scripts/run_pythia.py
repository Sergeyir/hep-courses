#!/usr/bin/env python3

import os
import sys
import argparse

import ROOT

import pythia8

def run_pythia(input_file_name : str, n_events : int) -> int :

    # checking if specified input file exists
    if not os.path.isfile(input_file_name) :
        print(f"\033[1;31mError:\033[0m file {input_file_name} was not found")
        return 1

    # creating and configuring Pythia instance
    # full manual is avaliable at https://pythia8.web.cern.ch/manuals/pythia8317/
    # (switch the version if you have a newer one)
    pythia = pythia8.Pythia()

    # To do: determine whether HardQCD and/or SoftQCD needed in this analysis
    # HardQCD: https://pythia.org/latest-manual/QCDHardProcesses.html
    pythia.readString("HardQCD:all = on");
    # SoftQCD: https://pythia8.web.cern.ch/manuals/pythia8317/QCDSoftProcesses.html
    pythia.readString("SoftQCD:all = on");
    # setting parameters from .cmnd input file
    pythia.readFile(input_file_name)

    # random generator seed; main can be changed to have seed argument. Seed is needed if you intend to perform calculations on multiple threads via bash scripts
    seed = 1;
    # setting the random seed
    pythia.readString("Random:seed = " + str(seed));

    # To do: enable HardQCD and/or SoftQCD (determine first which one is required for the current task), set collision energy, pTHatMin, pdf set using pythia.readString method
    # (HardQCD: https://pythia.org/latest-manual/QCDHardProcesses.html)
    # (SoftQCD: https://pythia8.web.cern.ch/manuals/pythia8317/QCDSoftProcesses.html)
    # (Collision and beam parameters: https://pythia.org/latest-manual/BeamParameters.html)
    # (Phase space cuts: https://pythia.org/latest-manual/PhaseSpaceCuts.html)
    # (PDF selection: https://pythia.org/latest-manual/PDFSelection.html)

    # uncomment the next line if you do not need for pythia to print event info and banner
    # pythia.readString("Print:quiet = on")

    # initializing pythia; this step pythia applies parameters we set earlier and 
    # checks if there are problems with the current specification
    pythia.init()

    # creating directory in which the file outputFile will be written
    os.makedirs("output", exist_ok=True)
    # To do: set a new name to the output file so that for each collision system output file will not be overwritten
    # file in which all histogram will be written; the following line will create 
    # (overwrite if exists) the file output/generated.root and root will point to it
    # so that TObject objects can be written in it by using method TObject::Write()
    output_file = ROOT.TFile("output/pp.root", "RECREATE")
    # if another file is created after this with option "RECREATE", "UPDATE", or "CREATE"
    # root will try to write TObject objects to the new defined file
    # if you have multiple TFile files you write in in yor program use 
    # TFile::cd() to point root to the file you need

    # To do: declare histograms to store the needed data
    # To do: declare histograms to store the needed data
    # 1-D histograms: https://root.cern.ch/doc/master/classTH1.html
    # 2-D histograms: https://root.cern.ch/doc/master/classTH2.html

    # iterating over all events
    for i in range(n_events) :

        # this tells pythia to generate next event
        # skipping if problems occur
        if not pythia.next() :
            continue

        # to get information on cross sections, weights, etc. use info on
        # https://pythia.org/latest-manual/CrossSectionsAndWeights.html

        # weight for the current event
        eventWeight = pythia.infoPython().weight()

        # Exercise: Can you determine whether the event weight is needed and why?

        # to get information of a pythia event you can use info on
        # https://pythia.org/latest-manual/EventRecord.html

        # iterating over all particles in a current event
        for j in range(pythia.event.size()) :

            # to get information of a particle you can use info on 
            # https://pythia.org/latest-manual/ParticleProperties.html

            # prints particle id (more info on id: 
            # https://pdg.lbl.gov/2007/reviews/montecarlorpp.pdf)
            
            # printing id of particles in a first event (as an example)
            if (i == 0) :
                print(pythia.event[j].id())

    # printing cross section (Can you deduce what the unit of measurement for this quantity is?)
    print(pythia.infoPython().sigmaGen())

    # closing file; this is not required in the current case, however in a general case
    # it is better to close files when you are done working with them 
    # so that there are no unintended writes/reads or assignment TObject file ownership
    output_file.Close()

    return 0

if __name__ == "__main__" :
    # parsing input arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--input", type=str)
    parser.add_argument("-n", "--nevents", type=int)

    args = parser.parse_args()
    input_file_name = args.input
    n_events = args.nevents

    # requiring for the input file to be passed
    if not input_file_name :
        print('\033[1;31mError:\033[0m input file must be specified; use option -i or --input')
        sys(exit(1))

    # requiring for the number of events to be passed
    if not n_events :
        print('\033[1;31mError:\033[0m number of events must be specified; use option -n or --nevents')
        sys(exit(1))

    sys.exit(run_pythia(input_file_name, n_events))
