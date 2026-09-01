# Table of contents

- [Overview](#overview)
- [Requirements](#requirements)
- [Analytic calculation](#analytic-calculation)
- [Installation](#installation)
- [How to use](#how-to-use)
    - [PYTHIA8+FASTJET](#pythia8+fastjet)
    - [HERWIG7+FASTJET](#herwig7+fastjet)
    - [POWHEGBOX+PYTHIA8+FASTJET](#powhegbox+pythia8+fastjet)
- [Using this project for teaching](#using-this-project-for-teaching)
- [Sources](#sources)

# Overview

This is a simple projects of a course that helps students in particle physics to study hard processes in p+p collisions at high energy and to learn high energy physics software. The goal is for the students to perform a numerical calculation of $d^2 \sigma/dp_{T} dy$ of jets reconstructed from particles within $\left|\eta\right| < 1$ for $\left|y\right| < 0.5$ and $0.5 < \left|y\right| < 1$. The calculation is performed in several ways:

- **PYTHIA8+FASTJET3**: reconstruction with FASTJET3 from PYTHIA8 final state particles
- **HERWIG7+FASTJET3**: reconstruction with FASTJET3 from HERWIG7 final state particles
- **POWHEGBOX+PYTHIA8+FASTJET3**: reconstruction with FASTJET3 from PYTHIA8 final particles with scattering amplitudes for PYTHIA8 adjusted with POWHEGBOX

# Requirements

- GCC(>=7) (for C++17 support)
- python3 + python3-dev (python-is-python3 package may also be required)
- [ROOT6](https://root.cern/)
- [LHAPDF6](https://lhapdf.hepforge.org/)
- [PYTHIA8](https://pythia.org/) compiled with LHAPDF6
- [FASTJET3](https://fastjet.fr/) 
- [HERWIG7](https://herwig.hepforge.org/) 
- [POWHEGBOX](https://powhegbox.mib.infn.it/) 

ROOT6, LHAPDF6, PYTHIA8, and FASTJET3 have to be compiled with python3 if you want to use them in python interface. For more information see [Installation tutorial](INSTALLATION_TUTORIAL.md).

# Installation

```sh
git clone https://github.com/Sergeyir/genjets-course --depth=1
```

Install all requirements and configure environment variables described in [Installation tutorial](INSTALLATION_TUTORIAL.md)

Python environment should work fine after the last step. If you need to use C++ for this project run

```sh
cmake .
make -j
```

To update the repository to the newest version run in its root

```sh
git pull
```

<details>
<summary> Installing PDF sets</summary>

You can install the needed pdf set with lhapdf command (if you installed it with python):

```sh
lhapdf install name_of_pdf_set
```

Or by installing it in .tar.gz format from https://www.lhapdf.org/pdfsets.html and extracting it into $LHAPDF_PATH/share/LHAPDF directory.

In the current repository example NNPDF31_lo_as_0118 and NNPDF31_lo_as_0118 pdf sets are used, so install them as well to test if the code works fine.
</details>

<details>
<summary> Implementing your changes</summary>

Since the project may be updated you may need to pull the changes. This way implementing your changes to the code may cause conflict in git version. To circumvent this you can create a branch of this repo or just copy the contents of this repository to your directory and remove CMake files and cache:

```sh
rm -r CMakeFiles cmake_install.cmake CMakeCache.txt
```

</details>

# Code examples and how to use them

The examples listed in this section are incomplete and only consist of simple code that show how the algorithm can be implemented with comprehensive explanations and links. Look for "To do:" in comments to see what needs to be completed/added. 

In general in examples distribution of quantities are obtained which then have to be used to obtain differential cross-section with the use of the following formula:

```math
\frac{1}{2 \pi p_T} \frac{d^2 \sigma}{dp_T dy} = \frac{1}{2 \pi p_T \Delta y} \frac{d N}{d p_T} \sigma_{tot}
```

Where $d^2N/dp_T dy$ - yields (i.e. obtained distributions), $\sigma_{tot}$ - event total cross section.
Where $\Delta y$ - rapidity range for the given $p_T$ bin (this course case: $y \approx \eta$)

## PYTHIA8+FASTJET3

<details>
<summary>C++</summary>

Run the compiled executable of a simple example to generate 1000 events with pythia. You can change number of events if needed.

```sh
bin/BasicPythia 1000
```

The code is located in src and include directories
</details>

<details>
<summary>python</summary>

Run the python script showing simple example to generate 1000 events with pythia. You can change number of events if needed

```sh
python scripts/basic_pythia.py -n 1000
```

The code located in scripts directory

</details>

## HERWIG+FASTJET

In this repository only the simplest example is shown. For more information see [Herwig event generation workflow tutorial](https://herwig.hepforge.org/tutorials/workflow/eventgeneration.html#standard-event-generation).

For the purpose of this course the simplest example for Herwig input file is shown in herwig directory located in this repository. Head there and run to read this input file

```sh
Herwig read BasicPP.in
```

Herwig will generate BasicPP.run file with all parameters important for event generation. You then can just run to generate 100 events:

```sh
Herwig run BasicPP.run -N 100
```

You can specify any number of events you want but be sure you have enough space (10000 events take ~1.5 GB of space).

After running Herwig, .hepmc file will be generated. And if you followed the example above, BasicPP.hepmc will appear in herwig directory. Head into the root of the repository and see the following instruction on how to read this file.

<details>
<summary> C++ </summary>
Check code for reading .hepmc file in files include/BasicHepMCReader.hpp and src/BasicHepMCReader.cpp and see "To do" instructions. After compiling with make you can run 

```sh
./bin/BasicHepMC3Reader herwig/BasicPP.hepmc
```

You can substitute "herwig/BasicPP.hepmc" argument for whatever file you want to read. 
</details>

<details>
<summary> python </summary>
Check code for reading .hepmc file in file scripts/basic_hepmc_reader.py and see "To do" instructions. You can run it with 

```sh
python scripts/basic_hepmc_reader.py -i herwig/BasicPP.hepmc
```

You can substitute "herwig/BasicPP.hepmc" argument for whatever file you want to read. 
</details>

## POWHEGBOX+PYTHIA8+FASTJET

<details>
<summary> Calculating scattering amplitudes with PowHegBox </summary>

After downloading and compiling code for dijets submodule, head into testrun-lhc directory. There you need to change file powheg.input by commenting out default PDF implementation and adding LHAPDF (LHA numbering corresponds to the LHAPDF ID in [LHAPDF pdf sets table](https://lhapdf.hepforge.org/pdfsets.html)

```
ndns1 131            ! pdf set for hadron 1 (mlm numbering) -> ! ndns1 131            ! pdf set for hadron 1 (mlm numbering)
ndns2 131            ! pdf set for hadron 2 (mlm numbering) -> ! ndns2 131            ! pdf set for hadron 2 (mlm numbering)
```

```
! lhans1  10050      ! pdf set for hadron 1 (LHA numbering) -> lhans1  303400      ! pdf set for hadron 1 (LHA numbering)
! lhans2  10050      ! pdf set for hadron 2 (LHA numbering) -> lhans2  303400      ! pdf set for hadron 2 (LHA numbering)
```

The following line is but a comment but I recommend to change it so it would not confuse you later

```
! 10050 cteq6m -> ! 303400 NNPDF31_nlo_as_0118
```

Also change other important parameters if needed (for example ebeam1, ebeam2). And since we specified NNPDF31_nlo_as_0118 pdf set, don't forget to download it via lhapdf or manually.

Then run the pwgh_main executable, which will calculate "numevts" (specified in powheg.input) number of events. Calculation as well as initialization will take some time (as stated in manual 50000 events should take around 3 hours). But for the purpose of this course you can set it to 5000, as it may take much longer time on older machines or laptops.

```
../pwhg_main
```

After the executable finishes running you will get pwgevents.lhe file which you can use in the following instructions for pythia8 event generation.

More info about dijet production can be found in Docs/manual-BOX-Dijet.pdf located in dijet directory or [6](#sources).

</details>

<details>
<summary> Using PowHegBox output for pythia8 event generation </summary>

You can use the same code you did for generating pythia8 events before, however change the pdf set, and set the following parameters to pythia (more info on beam parameters can be found [here](https://pythia.org/latest-manual/BeamParameters.html))

```
Beams:frameType = 4
Beams:LHEF = path_to_powheg_output_file.lhe
```

</details>

<!--
# Using this project for teaching

This is an open source public project which anyone can use. If you are teaching students programming/software in particle physics and would like to incorporate this project or a part of it, you can reach me at [antsupov0124@gmail.com](mailto:antsupov0124@gmail.com) so that I can send you a complete version of this repository. Complete version contains fully finished code which can be used to check the results obtained by students. Please do send the request from you university email address while stating the position, department, and university, so it would be easier for me to confirm your status.

Also this project by itself is not enough for someone to learn the underlying physics, programming, and HEP software. Supervision and assistance is required for students to have a fullest grasp of the subject and experience development.
-->

# Sources

1. C. Bierlich et al. ["A comprehensive guide to the physics and usage of PYTHIA 8.3"](https://arxiv.org/abs/2203.11601)
2. Matteo Cacciari, Gavin P. Salam, Gregory Soyez ["FastJet user manual"](https://arxiv.org/abs/1111.6097)
3. Johannes Bellm et al. ["Herwig 7.0 / Herwig++ 3.0 Release Note"](https://arxiv.org/abs/1512.01178)
4. Andy Buckley et al. ["LHAPDF6: parton density access in the LHC precision era"](https://arxiv.org/abs/1412.7420)
5. Simone Alioli, Paolo Nason, Carlo Oleari, Emanuele Re ["A general framework for implementing NLO calculations in shower Monte Carlo programs: the POWHEG BOX"](https://arxiv.org/abs/1002.2581)
6. Simone Alioli, Keith Hamilton, Paolo Nason, Carlo Oleari, Emanuele Re ["The POWHEG BOX user manual: dijet production"](https://usermanual.wiki/Pdf/manualBOXDijet.1397496255.pdf)
7. Michael E. Peskin Daniel V. Schroeder "An Introduction to Quantum Field Theory"
8. Silvia Ferreres-Solé, Torbjörn Sjöstrand ["The Space-Time Structure of Hadronization in the Lund Model"](https://arxiv.org/abs/1808.04619)
