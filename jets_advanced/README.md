# Table of contents

- [Overview](#overview)
- [Requirements](#requirements)
- [Analytic calculation](#analytic-calculation)
- [Installation](#installation)
- [How to use](#how-to-use)
    - [Generating with pythia8](#generating-with-pythia8)
    - [Running analytic calculation](#running-analytic-calculation)
- [Sources](#sources)

# Overview

This is a simple projects that helps students in particle physics to study hard processes in p+p collisions at high energy and to learn high energy physics software. The goal is for the students to perform a numerical calculation of $d \sigma/dp_{T}$ and $d \sigma / d \Delta y$ of partons that originate from LO and NLO hard processes. The calculation is performed in 3 different ways: 

- **Analytic**: calculation via the factorization formula for LO hard p+p collision
- **PYTHIA8**: estimation of cross section from PYTHIA8 outgoint parton data
- **PYTHIA8+FASTJET3**: estimation of cross section via reconstruction of jets with FASTJET3 from PYTHIA8 final state particles
- **MadGraph**: calculation of NLO cross sections in MadGraph with NLO enabled

# Requirements

- GCC(>=7) (for C++17 support)
- python3 + python3-dev (python-is-python3 package may also be required)
- [ROOT6](https://root.cern/)
- [LHAPDF6](https://lhapdf.hepforge.org/)
- [PYTHIA8](https://pythia.org/) compiled with LHAPDF6
- [FASTJET3](https://fastjet.fr/) 
- [PyYAML](https://pypi.org/project/PyYAML/) if you intend to use python3 code (can be installed with pip or with package manager - python3-yaml)

ROOT6, LHAPDF6, PYTHIA8, and FASTJET3 have to be compiled with python3 for the python interface. For more information see [Installation tutorial](../INSTALLATION_TUTORIAL.md).

# Analytic calculation

Using the factorization formula for $d \sigma / dy_{1} dy_{2} dp^2_{T}$ for LO hard $p+p$ process from [2](#sources) (Eq.51.42) and considering quarks massless, one can obtain

```math
\frac{d \sigma}{dp_T dy_1 dy_2} = \frac{8 \pi p_T}{\hat{s}} \sum_{ijkl} x_{1} x_{2} \left[ f_{i}(x_{1}, \mu_F^2) f_{j}(x_{2}, \mu_F^2) \frac{d \sigma_{ij \rightarrow kl}}{d \Omega} (\hat{s}, \hat{t}, \hat{u}) +  f_{i}(x_{2}, \mu_F^2) f_{j}(x_{1}, \mu_F^2) \frac{d \sigma_{ij \rightarrow kl}}{d \Omega} (\hat{s}, \hat{u}, \hat{t}) \right]
```

If i and j are identical particle types, the second term $d \sigma_{ij \rightarrow kl} / d\Omega$ should be dropped.

For this project the hard scale is implemented i.e. $\mu_F \approx p_T$

$d \sigma_{ij \rightarrow kl}/d \Omega$ can be found in  in [2](#sources) (Eq. 51.4 - 51.12)

The formulas for the needed cross section can be obtained with kinematic equations from [1](#sources) (Eq.206-211)

```math
\frac{d \sigma}{dp_T} = \int dy_1 \int dy_2 \frac{d \sigma}{dp_T dy_1 dy_2}
```

```math
\frac{d \sigma}{d \Delta y} = \int dp_T \int dy_1 \int dy_2 \frac{d \sigma}{dp_T dy_1 dy_2} \delta(\Delta y - |y_1 - y_2|)
```

# Installation

```sh
git clone https://github.com/Sergeyir/HardProcessesLO --depth=1 --recurse-submodules
```

Install all requirements and configure environment variables described in [Installation tutorial](../INSTALLATION_TUTORIAL.md)

Python environment should work fine after the last step. If you need to use C++ for this project run

```sh
cmake .
make -j
```

To update the repository to the newest version run in its root

```sh
git pull --recurse-submodules
```

<details>
<summary> Installing PDF sets</summary>

You can install the needed pdf set with lhapdf command (if you installed it with python):

```sh
lhapdf install name_of_pdf_set
```

Or by installing it in .tar.gz format from https://www.lhapdf.org/pdfsets.html and extracting it into $LHAPDF_PATH/share/LHAPDF directory.

In the current repository example NNPDF31_lo_as_0118 pdf set is used, so install it as well to test if everything works fine.
</details>

<details>
<summary> Implementing your changes</summary>

Since the project may be updated you may need to pull the changes. This way implementing your changes to the code may cause conflict in git version. To circumvent this you can create a branch of this repo or just copy the contents of this repo to your directory and remove CMake files and cache:

```sh
rm -r CMakeFiles cmake_install.cmake CMakeCache.txt
```

</details>

# How to use

## Generating with pythia8

Both python and C++ show examples on calculation of single jet multiplicities (direct calculation and fastjet reconstruction) from which $d \sigma/dp_T$ can be obtained. Similarly you can find the way to calculate the needed multiplicity for $d \sigma/d \Delta y$.

<details>
<summary>C++</summary>

Run the compiled executable of a simple example to generate 1000 events with pythia using pythia specification from file input/pp7TeV.yaml. You can change input file and number of events if needed

```sh
bin/GenerateData input/pp7TeV.yaml 1000
```
</details>

<details>
<summary>python</summary>

Run the python script showing simple example to generate 1000 events with pythia using pythia specification from file input/pp7TeV.yaml. You can change input file and number of events if needed

```sh
python scripts/generate_data.py -i input/pp7TeV.yaml -n 1000
```

</details>

## Running analytic calculations

Both C++ and python code are **incomplete examples** that only show how to calculate $d \sigma/dp_T$. The code was intended for the students to finish so look for keyphrase "to do" in the comments of the code. Until all formulas are corrected/completed the compiled running the executable/script will not give the correct result.

<details>
<summary>C++</summary>
Check the code include/CalculateAnalytic.hpp and src/CalculateAnalytic.cpp and finish it. After adding updates, compile the code with make and you can run it with

```sh
bin/CalculateAnalytic input/pp7TeV.yaml 100
```

Where the first argument provides the name of the file with all important specifications, and the second one provides the number of integration steps for Monte Carlo integration.
</details>

<details>
<summary>python</summary>
Check the code srcipts/calculate_analytic.py and finish it. After adding updates you can run it with

```sh
python scripts/calcylate_analytic.py -i input/pp7TeV.yaml -n 100
```

Where the "-i" argument provides the name of the file with all important specifications, and "-n" provides the number of integration steps for Monte Carlo integration.
</details>

# Sources

1. [Michelangelo L. Mangano "Introduction to QCD"](https://cds.cern.ch/record/454171/files/p53.pdf)
2. [S. Navas et al. (Particle Data Group), Phys. Rev. D 110, 030001 (2024) and 2025 update](https://pdg.lbl.gov/2025/reviews/contents_sports.html)
