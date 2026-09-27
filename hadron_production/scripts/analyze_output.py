#!/usr/bin/env python3

import os
import sys
import argparse
import numpy

import ROOT

def analyze_output() :
    # make ROOT not to draw histogram in new window when Draw method is invoked
    ROOT.gROOT.SetBatch(True)
    # do not draw statbox of a histogram
    ROOT.gStyle.SetOptStat(False)

    # opening and loading the file
    file = ROOT.TFile.Open("output/pp.root")

    # retrieving the histogram; by default TFile::Get return TObject pointer
    # so you need to cast it to the type you have written 
    multPT = ROOT.TH1D(file.Get("pT multiplicity"))

    # To scale all contents of the histogram use
    multPT.Scale(1./(2.*numpy.pi))

    # Iterating over x axis of a histogram (bin numbering starts at 1 at ends at number of bins)
    for i in range(1, multPT.GetXaxis().GetNbins()) :
        # retrieving ith bin content of the histogram
        binContent = multPT.GetBinContent(i)
        # retrieving ith bin error of the histogram
        binError = multPT.GetBinError(i)
        # setting pT as the center of the ith bin
        pT = multPT.GetXaxis().GetBinCenter(i)
        # delta pT can be calculated with TAXIS::GetBinWidth(int)
        # changing the bin content for each bin individually
        multPT.SetBinContent(i, binContent)

    # To do: add scaling by total cross section and divide each bin by the 
    # bin width to obtain the invariant differential cross section

    # Setting an empty title
    multPT.SetTitle("")
    # More info on tex syntax in ROOT: 
    # https://root.cern.ch/doc/master/classTLatex.html
    # Set the X axis title
    multPT.GetXaxis().SetTitle("p_{T}")
    # Set the Y axis title
    # To do: change the 
    multPT.GetYaxis().SetTitle("#frac{d #sigma}{d p_{T}}")

    # You will also need to save the picture in .png and/or .pdf
    # Use ROOT TCanvas to draw on the canvas and write it as a picture
    # https://root.cern.ch/doc/master/classTCanvas.html
    canv = ROOT.TCanvas("canv", "", 800, 800)

    # gPad - current pad (TPad) on the canvas
    # setting log y scale
    ROOT.gPad.SetLogy()

    # Graphical adjustments to the canvas: setting canvas margins
    # To do: improve these margins if needed
    ROOT.gPad.SetLeftMargin(0.1)
    ROOT.gPad.SetRightMargin(0.05)
    ROOT.gPad.SetTopMargin(0.05)
    ROOT.gPad.SetBottomMargin(0.1)

    # Graphical adjustments to the histogram axis: setting axis titles offsets
    # To do: improve these offsets if needed
    multPT.GetXaxis().SetTitleOffset(1.)
    multPT.GetYaxis().SetTitleOffset(1.2)

    # Drawing the histogram on the current pad
    multPT.Draw()

    # saving canvas as .pdf picture
    os.makedirs("pictures", exist_ok=True)
    canv.SaveAs("pictures/cs_pp.pdf")
    # it can also be saved as .png
    #canv.SaveAs("output/cs_pp.png")
    
    # The example from above is for pp only
    # To do: after performing MC for p+A, A+B collisions, read the N_{ch} multiplicity and pT vs 
    # N_{ch} multiplicity histograms, divide N_{ch} multiplicity histogram into centrality regions.
    # For each centrality region take projection of pT vs N_{ch} multiplicity on pT within the needed 
    # N_{ch} range and calculate the cross section. Use TH2::ProjectionX or TH2::ProjectionY 
    # (depending on which axis you have chosen for N_{ch} and pT)

if __name__ == "__main__" :
     sys.exit(analyze_output())
