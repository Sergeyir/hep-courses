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

def calculate_analytic(input_file_name : str, nsteps : int) -> int:
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

if __name__ == "__main__":
    global number_of_integration_steps
    # parsing input arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("input", type=str, help=".yaml file name located in input "\
                                                  "(e.g. input/pp7TeV.yaml")
    parser.add_argument("nsteps", type=int, help="number of integration steps")

    args = parser.parse_args()
    input_file_name = args.input
    nsteps = args.nsteps

    sys.exit(calculate_analytic(input_file_name, nsteps))
