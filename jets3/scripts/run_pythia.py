#!/usr/bin/env python3

import os
import sys
import argparse

import yaml
import ROOT

import pythia8
import fastjet

def run_pythia(input_file_name : str, n_events : int) -> int :

    # checking if specified input file exists
    if not os.path.isfile(input_file_name) :
        print(f"\033[1;31mError:\033[0m file {input_file_name} was not found")
        return 1

    # loading contents of input file
    with open(input_file_name, "r") as f :
        input_file = yaml.safe_load(f)

    # reading parameters from .yaml file
    # data from .yaml file can be accessed via index operator
    # casting is required as .yaml file does not store types
    pdfset = str(input_file["pdfset"])
    # To do: 
    pTHatMin = 0.
    energyCM = 0. 
    fastjet_r = 0.1

    # create and configure Pythia instance
    # full manual is avaliable at https://pythia8.web.cern.ch/manuals/pythia8317/
    # (switch the version if you have a newer one)
    pythia = pythia8.Pythia()

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
    # file in which all histogram will be written; the following line will create 
    # (overwrite if exists) the file output/generated.root and root will point to it
    # so that TObject objects can be written in it by using method TObject::Write()
    output_file = ROOT.TFile("output/generated.root", "RECREATE")
    # if another file is created after this with option "RECREATE", "UPDATE", or "CREATE"
    # root will try to write TObject objects to the new defined file
    # if you have multiple TFile files you write in in yor program use 
    # TFile::cd() to point root to the file you need

    # To do: declare histograms to store the needed data

    # Declaring fastjet::JetDEfinition instance using anti kt jet clustering algorithm
    # More on fastjet: https://indico.cern.ch/event/264054/contributions/592237/attachments/467910/648313/fastjet-doc-3.0.3.pdf
    jetdef = fastjet.JetDefinition(fastjet.antikt_algorithm, fastjet_r)

    # iterating over all events
    for i in range(n_events) :

        # this tells pythia to generate next event
        # skipping if problems occur
        if not pythia.next() :
            continue

        # list that will be filled with all final state particles that passed all cuts
        particles = []

        # to get information on cross sections, weights, etc. use info on
        # https://pythia.org/latest-manual/CrossSectionsAndWeights.html

        # weight for the current event
        eventWeight = pythia.infoPython().weight()

        # Exercise: Can you determine whether the event weight is needed in p+p and why?

        # to get information of a pythia event you can use info on
        # https://pythia.org/latest-manual/EventRecord.html

        # iterating over all particles in a current event
        for j in range(pythia.event.size()) :

            # to get information of a particle you can use info on 
            # https://pythia.org/latest-manual/ParticleProperties.html
            part = pythia.event[j]

            # prints particle id (more info on id: 
            # https://pdg.lbl.gov/2007/reviews/montecarlorpp.pdf)
            
            # printing id of particles in a first event (as an example)
            if (i == 0) :
                print(pythia.event[j].id())

            # in experiments jets usually measured from charged (+-1q) final state particles
            # sometimes neutral particles are also employed in jet reconstruction algorithms
            # in this work all particles will be used

            # To do: add restrictions for particles selected for jets

            # creating 4-vector with fastjet::PseudoJet
            pj = fastjet.PseudoJet(part.px(), part.py(), part.pz(), part.e())
            # adding fastjet::PseudoJet 4-vector to the particle list
            particles.append(pj)

            # To do: add a condition in this loop that checks whether a particle
            # is a parton outgoing from a hard process so that you can get their pT and y

        # running jet clustering algorithm
        cs = fastjet.ClusterSequence(particles, jetdef)
        # list filled with jets reconstructed from list of particle 4-vectors
        inclusive_jets = cs.inclusive_jets(pTHatMin)

        # iterating over reconstructed jets
        for jet in inclusive_jets :
            # printing pT of reconstructed jets in a first event (as an example)
            if (i == 0) :
                print(jet.pt())
            # To obtain rapidity of a jet use jet.rap()

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
