#!/usr/bin/env python3

#  @file   basic_pythia.py
#  @brief  Contains simplest case that shows how the data can be generated with PYTHIA8 and LHAPDF6 and processed with FASTJET3 for jets selection 
#
#  This file is a part of a project genjets-course (https://github.com/Sergeyir/genjets-course).
#
#  @author Sergei Antsupov (antsupov0124@gmail.com)

import os
import sys
import argparse

import ROOT

import pythia8
import fastjet

def generate_data(n_events : int) -> int :

    # collision energy [GeV] in CM; 
    energyCM = 7000
    # hard process lower bound on pT [GeV/c]
    pTHatMin = 25
    # pdf set
    pdfset = "LHAPDF6:NNPDF31_lo_as_0118"
    # fastjet R parameter
    fastjet_r = 0.4

    # create and configure Pythia instance
    pythia = pythia8.Pythia()
    # enabling hard QCD processes 
    # (more info on https://pythia.org/latest-manual/QCDHardProcesses.html)
    pythia.readString("HardQCD:all = on")
    # by default p+p collisions will be initialized since we do not set ids of incoming particles
    # (more info on https://pythia.org/latest-manual/BeamParameters.html)
    pythia.readString(f"Beams:eCM = {energyCM}")
    # setting hard process lower bound on pT [GeV/c]; processes with lower pT will not be generated
    # (more info on https://pythia.org/latest-manual/PhaseSpaceCuts.html)
    pythia.readString(f"PhaseSpace:pTHatMin = {pTHatMin}")
    # setting PDF set from LHAPDF6; pythia8 also has many predefined sets
    # (more info on https://pythia.org/latest-manual/PDFSelection.html)
    pythia.readString(f"PDF:pSet = {pdfset}")
    # pythia.readString("Print:quiet = on")

    # initializing pythia; this step pythia applies parameters we set earlier and 
    # checks if there are problems with the current specification
    pythia.init()

    # creating directory in which the file outputFile will be written
    os.makedirs("output", exist_ok=True)
    # file in which all histogram will be written; the following line will create 
    # (overwrite if exists) the file output/generated.root and root will point to it
    # so that TObject objects can be written in it by using method TObject::Write()
    output_file = ROOT.TFile("output/basic_pythia.root", "RECREATE")
    # if another file is created after this with option "RECREATE", "UPDATE", or "CREATE"
    # root will try to write TObject objects to the new defined file
    # if you have multiple TFile files you write in in yor program use 
    # TFile::cd() to point root to the file you need

    # number of bins for histograms below
    nbins = 1000 + int(pTHatMin)
    # more info on TH1D you can find on https://root.cern.ch/doc/master/classTH1.html
    # histogram in which reconstructed jets pT will be written
    distr_jets_pt = ROOT.TH1D("Jet pT", "N(p_{T})", nbins, 0.0, 100. + pTHatMin)
    # kinematically pT can go to much higher values but for this example statistical significant result can be obtained only for this range within reasonable processing time

    # To do: change the distribution above to jet counts vs pT vs y and change all 
    # following distr_jets_pt instances accordingly

    # you can read more about fastjet and its usage on 
    # https://indico.cern.ch/event/264054/contributions/592237/attachments/467910/648313/fastjet-doc-3.0.3.pdf
    jetdef = fastjet.JetDefinition(fastjet.antikt_algorithm, fastjet_r)

    # iterating over all events (we specified argv[2] to be the number of events we generate)
    for i in range(n_events):

        # this tells pythia to generate next event
        if not pythia.next():
            continue

        # vector that will be filled with all final state particles that passed all cuts
        particles = []

        # to get information on cross sections, weights, etc. use info on
        # https://pythia.org/latest-manual/CrossSectionsAndWeights.html

        # weight for the current event
        eventWeight = pythia.infoPython().weight()

        # Exercise: Can you say if the event weight is needed in p+p and why?

        # to get information of a pythia event you can use info on
        # https://pythia.org/latest-manual/EventRecord.html

        # iterating over all particles in a current event

        for j in range(pythia.event.size()):
            # to get information of a particle you can use info on 
            # https://pythia.org/latest-manual/ParticleProperties.html
            part = pythia.event[j]

            # checking if particle is stable and did not interact with anything
            if part.isFinal():
                # prints particle id (more info on id: 
                # https://pdg.lbl.gov/2007/reviews/montecarlorpp.pdf)
                
                # in experiments jets usually only consist of charged (+-1q) final state particles
                # neutral particles are usually excluded although photons and neutral hadrons can 
                # be used and sometimes are included which is what we are going to do in this work
                
                # To do: add a check that only allows stable detectable 
                # particles to be used for jet reconstruction

                # To do: add a check that only allows particles withing allowed pseudorapidity range

                # adding fastjet::PseudoJet entry to the particles vector
                particles.append(fastjet.PseudoJet(part.px(), part.py(), part.pz(), part.e()))

        if len(particles) == 0:
            continue

        # running jet clustering algorithm
        cs = fastjet.ClusterSequence(particles, jetdef)
        # vector filled with jets reconstructed from vector particles
        inclusive_jets = cs.inclusive_jets(pTHatMin)

        # iterating over reconstructed jets
        for jet in inclusive_jets:
            distr_jets_pt.Fill(jet.pt(), eventWeight)

    # printing cross section (Can you deduce what the unit of measurement for this quantity is?)
    print(pythia.infoPython().sigmaGen())

    # saving histograms in a TFile
    distr_jets_pt.Write()

    # closing file; this is not required in the current case, however in a general case
    # it is better to close files when you are done working with them 
    # so that there are no unintended writes/reads or assignment TObject file ownership
    output_file.Close()

    return 0

if __name__ == "__main__":
    # parsing input arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("-n", "--nevents", type=int)

    args = parser.parse_args()
    n_events = args.nevents

    if not n_events:
        print('\033[1;31mError:\033[0m number of events must be specified; use option -n or --nevents')
        sys(exit(1))

    sys.exit(generate_data(n_events))
