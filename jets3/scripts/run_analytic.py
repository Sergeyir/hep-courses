#!/usr/bin/env python3

from typing import Tuple
import os
import sys
import argparse
import time
import math
import yaml
import numpy as np
import lhapdf
import ROOT

# Globals
pdf = None  # LHAPDF PDF object

def get_dsigma_domega(id1: int, id2: int, pT: float, s: float, y: float) -> float:
    """
    To do: define mandelstam variables (t, u) and substitute lower random dummies for them
    (hint: it will be easier if you define cos(Theta) first
    """
    t = s/2.
    u = s/2. 

    # alpha_s at hard scale \mu_F = pT
    alpha_s = pdf.alphasQ2(pT * pT)

    """
    To do: implement other channels below
    """
    # only qq -> qq is shown as an example
    if id1 != 0 and id2 != 0 and id1 == id2:
        # source: Eq. 51.7 at https://pdg.lbl.gov/2025/reviews/contents_sports.html
        return (alpha_s * alpha_s / (9.0 * s)) * ((t * t + s * s) / (u * u) +
                                                (s * s + u * u) / (t * t) -
                                                2.0 * s * s / (3.0 * u * t))
    return 0.
    """
    To do: add other d\sigma / d\Omega processes here
    """


def get_dsigma_dpt_dy1_dy2(pT: float, y1: float, y2: float) -> float:

    """
    To do: fix get_x1 and get_x2 functions
    """
    x1 = get_x1(pT, sqrtSNN, y1, y2)
    x2 = get_x2(pT, sqrtSNN, y1, y2)

    """
    To do: introduce a check that tests if x1 and x2 have proper values
    """

    # s aka \hat{s} is the center of mass energy of 2 partons
    s = (sqrtSNN * sqrtSNN) * x1 * x2

    # Sum over all combinations id1=-5..5, id2=-5..5
    result = 0.0
    for id1 in range(-5, 6):
        for id2 in range(id1, 6):
            # LHAPDF xfxQ2 takes (id, x, Q2)
            fx1 = pdf.xfxQ2((21, id1)[id1 == 0], x1, pT * pT)
            fx2 = pdf.xfxQ2((21, id2)[id2 == 0], x2, pT * pT)
            dsigma_domega = get_dsigma_domega(id1, id2, pT, s, y1 - y2)
            """
            To do: determine measurement units for the following expression
            """
            result += 8.0 * math.pi * pT * fx1 * fx2 * dsigma_domega / s
            """
            To do: implement expression for the result for the case when particles are non-identical
            """
    return result


def get_x1(pT: float, sqrtSNN: float, y1: float, y2: float) -> float:
    """
    To do: fill in the formula for x2 in the current function
    """
    return 0.5 


def get_x2(pT: float, sqrtSNN: float, y1: float, y2: float) -> float:
    """
    To do: fill in the formula for x2 in the current function
    """
    return 0.5 

def get_dsigma_d_deltay(delta_y: float, sqrtSNN: float, absYMax: float, pTHatMin: float) -> Tuple[float, float]:
    """
    This is just a functiont prototype The intended purpose of which is to show how to 
    randomly choose pT. If the pT is chosen from the uniform distribution the cross section
    values will jump to much from point to point due to significant contribution coming from
    low pT region (with finite integration calculation steps only few points on this range will be
    selected which results in some points having larger contribution from low pT that the other
    thus the "jumpiness" of a histogram points arises). 
    """

    normalization = 0.
    result = 0.

    for _ in range(number_of_integration_steps):
        # To do : add correct formula for tau
        """
        This is one way to achieve this: to distrubute pT in a way so that 
        there are many more points on low pT
        """
        tau = 1.
        pT = pTHatMin + rnd.Exp(tau)
        """
        In order for the TRandom to work correctly and fast tau needs to be defined so that
        int_{pTHatMIn}^{pTMax} e^{-x/tau) dx = 1
        Where pTMax - maximum kinematicaly possible pT value
        Exponent is chosen since it resembles the form of dSigma/dpT. The closer the resemblance - 
        the better error estimation is performed for finite number of integration steps
        """

        """
        But since the distribution is non-uniform the normalization 
        needs to be explicitly calculated as sum of weig
        """
        normalization += math.exp(-(pT - pTHatMin)/tau) # here exp(-pT/tau) is weight

        # dummy value
        result += 1e-6

    # To do: add normalization by integration volume
    """
    To do: check for other sources of uncertainties and propagate 
    them with this error if you find any
    """
    err = result/sqrt(normalization)

    return result/normalization, err

def calculate_analytic(input_file_name : str) -> int:
    global pdf, number_of_integration_steps, rng

    with open(input_file_name, "r") as f:
        cfg = yaml.safe_load(f)

    pdfset = cfg["pdfset"]
    pTHatMin = float(cfg["pthatmin"])
    absMaxY = float(cfg["abs_max_eta"])
    sqrtSNN = float(cfg["energy"])

    pdf = lhapdf.mkPDF(pdfset)

    os.system('mkdir -p output')
    output_file = ROOT.TFile("output/analytic.root", "RECREATE")

    # To do: create histogram in which you will store data

    # To do: implement weighted MC integration using vegas for each pT bin of a histogram you created
    # Hint: use get_dsigma_dpt_dy1_dy2 function to pass to the integrator
    # see ndim_integration.py in hep-courses/examples
    # Hint: you can use number_of_integration_steps to set the number of calls for the integrator

    # To do: write a histogram you created in output_file before closing it
    output_file.Close()
    
    return 0

def main() :
    global number_of_integration_steps
    # parsing input arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--input", type=str)
    parser.add_argument("-n", "--nsteps", type=int)

    args = parser.parse_args()
    input_file_name = args.input
    number_of_integration_steps = args.nsteps

    if not input_file_name :
        print('\033[1;31mError:\033[0m input file must be specified; use option -i or --input')
        sys(exit(1))

    if number_of_integration_steps == 0 :
        print('\033[1;31mError:\033[0m number of integration steps must be specified; '\
              'use option -n or --nsteps')
        sys(exit(1))

    sys.exit(calculate_analytic(input_file_name))

if __name__ == "__main__":
    main()
