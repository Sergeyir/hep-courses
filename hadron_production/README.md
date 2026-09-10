# Table of contents

- [Overview](#overview)
- [Required packages](#required_packages)
- [Task guide](#task_guide)
    - [Transforming differential cross sections](#transforming_differential_cross_sections)
    - [PYTHIA8 LO](#pythia8_lo)
    - [PYTHIA8+FASTJET3 LO](#pythia8+fastjet3_lo)
    - [Analytic LO](#analytic_lo)
    - [MadGraph NLO](#madgraph_nlo)
- [Sources](#sources)

# Overview

This is a simple projects that helps students in particle physics to study hadron production at high energy and to learn high energy physics software. The goal is for the students to perform measurements of invariant $p_T$ spectra and nuclear modification factors in various collision systems.

# Required packages

[Installation tutorial](../INSTALLATION_TUTORIAL.md) contains installation instruction for the following packages.

- [ROOT6](https://root.cern/)
- [LHAPDF6](https://lhapdf.hepforge.org/)
- [PYTHIA8](https://pythia.org/) compiled with LHAPDF6

ROOT6, LHAPDF6, and PYTHIA8 have to be compiled with python3 for the python interface.

# Task guide

Your task is to calculate invariant $p_T$ spectra $1/(2\pi p_T) d^2 \sigma / dpT dy$ in $p+p$, $p$+A, and A+B (where A, B - generalized notation for nuclei) and nuclear modification factors $R_{pA}$, $R_{AB}$, $R_{CP}$. The output of your task must be the following:

1. $1/(2\pi p_T) d^2 \sigma / dpT dy$ for different centralities on the same picture (for each collision system)
2. $R_{AB}$ for different collision systems (for each centrality)
2. $R_{CP}$ for different collision systems

<details>
<summary>Assignment options</summary>

There are 6 assignment options for the particle species (see [1](#sources) to find MC id of particles, [2](#sources) for particle listings):

1. $pi^\pm$ 
2. $pi^0$ 
3. $K^\pm$
4. $p_\bar{p}$
5. $\varphi(1020)$
6. $\Lambda(1115)$
7. $D^0$
8. $B^0$
9. $J/\psi(1S)$

3 assignment options for collision systems:

1. (LHC) $p+p$, $p$+Pb, Pb+Pb, $\sqrt{s_{NN}} = 5.36$ TeV
2. (RHIC) $p+p$, $p$+Au, $Au+Au$, $\sqrt{s_{NN}} = 200$ GeV

If your task number is 21 then your option is measurements of $pi^0$ in $p+p$, $p$+Pb, and Pb+Pb at $\sqrt{s_{NN}} = 5.36$ TeV

</details>

# Sources

1. [Monte Carlo numbering scheme](https://pdg.lbl.gov/2007/reviews/montecarlorpp.pdf)
2. [F. Takahashi et al. (Particle Data Group), Int. J. Mod. Phys. A 41 , 2630011 (2026)](https://pdg.lbl.gov/2026/listings/contents_listings.html)
