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

This is a simple projects that helps students in particle physics to study hard processes and jets in p+p collisions at high energy and to learn high energy physics software. The goal is for the students to perform a numerical calculation of $d \sigma/d p_T$ of partons/jets that originate from LO and NLO hard processes. The calculation is performed in 4 different ways:

- **PYTHIA8 LO**: estimation of LO cross section from PYTHIA8 outgoing parton data
- **PYTHIA8+FASTJET3 LO**: estimation of LO cross section via reconstruction of jets with FASTJET3 from PYTHIA8 final state particles
- **Analytic LO**: calculation via the factorization formula for LO hard p+p collision
- **MadGraph NLO**: calculation of NLO cross sections in MadGraph with NLO enabled

Additional calculation $d \sigma / d \Delta y$ is performed for above methods as an advanced task (where $\Delta y = y_1 - y_2)$.

# Required packages

[Installation tutorial](../INSTALLATION_TUTORIAL.md) contains installation instruction for the following packages.

- [ROOT6](https://root.cern/)
- [LHAPDF6](https://lhapdf.hepforge.org/)
- [PYTHIA8](https://pythia.org/) compiled with LHAPDF6
- [FASTJET3](https://fastjet.fr/) 
- [PyYAML](https://pypi.org/project/PyYAML/) if you intend to use python3 code (can be installed with pip or with package manager - python3-yaml or similar)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp) if you intend to use C++ code (see instructions below)

ROOT6, LHAPDF6, PYTHIA8, and FASTJET3 have to be compiled with python3 for the python interface.

<details>
<summary>yaml-cpp instructions</summary>
yaml-cpp is already present as a submodule in this repository. From the directory of this project run in terminal to update yaml-cpp

```sh
git submodule update --init --recursive yaml-cpp
```

CMakeLists.txt and rootlogon.C are already configured for yaml-cpp, but you need to compile yaml-cpp libraries first. To do this run in terminal in the directory of this project

```sh
cmake . && make -j
```

</details>

# Task guide

Your task is to calculate cross section $d \sigma/d p_T$ vs $p_T$ within your pseudorapidity region (and $d \sigma / d \Delta y$ vs $p_T$ if specified) using 4 different methods (pythia8 LO, pythia8+fastjet LO, analytic LO, Madgraph NLO). The output of your task must be the following:

1. $\alpha_{S}$ vs $p_T$ graph
2. Differential cross sections from 4 different methods on the same picture

It is recommended to save the calculated cross sections in .root files, so that it would be easier to draw them on the same picture.

<details>
<summary>Assignment options</summary>

There are 3 assignment options for $s_{\sqrt{NN}}$ and $\hat{p}_{T}^{min}$

1. $s_{\sqrt{NN}} = 2.36$ TeV, $\hat{p}_{T}^{min}$ = 15 GeV
2. $s_{\sqrt{NN}} = 7$ TeV, $\hat{p}_{T}^{min}$ = 25 GeV
3. $s_{\sqrt{NN}} = 13.6$ TeV, $\hat{p}_{T}^{min}$ = 25 GeV

6 assignment options for PDF sets (LO and NLO respectively):

1. NNPDF31_lo_as_0118 and NNPDF31_nlo_as_0118
2. NNPDF40_lo_as_01175 and NNPDF40_nlo_as_01175
3. NNPDF40_lo_as_01180 and NNPDF40_nlo_as_01180
4. NNPDF40_lo_as_01185 and NNPDF40_nlo_as_01185

And 2 assignment options for the pseudorapidity region limitation

1. $\left| \eta \right| < 0.5$
2. $\left| \eta \right| < 1$

If your task number is 321 then your option is $s_{\sqrt{NN}} = 13.6$ TeV, NNPDF40_lo_as_01175 for LO and NNPDF40_nlo_as_01175 for NLO, $\left| \eta \right| < 0.5$.

Install the needed pdf sets. Edit the configuration of .yaml file in input for your assignment option.

</details>

## Transforming differential cross sections

You are not integrating for the whole $y_1$ and $y_2$ region for $d \sigma / d p_T$, thus for finite rapidity bins $y_min < y_1, y_2 < y_max$

```math
\frac{d \sigma}{d p_T} \approx \int_{y_min}^{y_max} \frac{d \sigma}{dp_T dy_1 dy_2} \Delta y_1 \Delta y_2 (Eq.1)
```

Where $\Delta y_1$, $\Delta y_2$ - kinematic regions of $y_1$ and $y_2$. This kinematic region may not equal to $y_max - y_min$ due to kinematic constrains for the given $p_T$. Therefore you have to determine $\Delta y_1$ and $\Delta y_2$.

Similarly, for $d \sigma / d \Delta y$ you have to determine $\Delta p_T$ for each $\Delta y$ bin.

## PYTHIA8 LO

1. Load configuration from the .yaml file using yaml-cpp or PyYAML
2. Create pythia instance and pass the needed flags via pythia.readString method so that pythia can generate p+p collision events relevant to this task
3. Create $d \sigma/d p_T dy_1 dy_2$ vs $p_T$ vs $y_1$ histogram (no need to store $y_2$ since it can be obtained from $p_T$ and $y_1$)
3. Start event generation
4. Select partons from hard interactions, limit them within your pseudorapidity range, and store their $p_T$ and $y_1$ in the histogram to obtain multiplicity vs $p_T$ vs $y_1$
5. Convert multiplicity vs $p_T$ of partons into invariant cross-section $d \sigma/d p_T dy_1 dy_2$
6. Check whether the rapidity coverage throughout different $p_T$ bins stays the same and use it as $\Delta y_1$ estimate (which also equals to $\Delta y_2$) to calculate $d \sigma/d p_T$ using Eq.1. If rapidity coverage changes, approximate it and use approximation as $\Delta y_1$ estimate.

As a foundation, for C++ use include/RunPythia.hpp and src/RunPythia.cpp, while for python use scripts/run_pythia.py. Look for "To do" in the comments for hints.

To run C++ code use compiled binaries in bin directory after compilation, while python code can be executed by python system binary

<details>
<summary>How to run C++ executable</summary>

Run the compiled executable of a simple example to generate 1000 events with pythia using configuration from file input/pp7TeV.yaml as an example. You can change input file and number of events if needed

```sh
bin/RunPythia input/pp7TeV.yaml 1000
```
</details>

<details>
<summary>How to execute python code</summary>

Run the python script showing simple example to generate 1000 events with pythia using pythia specification from file input/pp7TeV.yaml as an example. You can change input file and number of events if needed

```sh
python scripts/run_pythia.py -i input/pp7TeV.yaml -n 1000
```

Or alternatively 

```sh
./scripts/run_pythia.py -i input/pp7TeV.yaml -n 1000
```

</details>

## PYTHIA8+FASTJET3 LO

Use the same code and setup as for pythia8 calculations. Additionally

1. Create and fill the vector of final state particles for each event
2. After filling the vector, create the fastjet:ClusterSequence object and pass the filled vector in the constructor
3. Retrieve the inclusive jets for the event using fastjet::ClusterSequence::inclusive_jets method and fill their $p_T$ in the histogram to obtainn multiplicity vs $p_T$
5. Convert multiplicity vs $p_T$ of jets into invariant cross-section $d \sigma/d p_T$

## Analytic LO

Using the factorization formula for $d \sigma / dy_{1} dy_{2} dp^2_{T}$ for LO hard $p+p$ process from [2](#sources) (Eq.51.42) and considering quarks massless, one can obtain

```math
\frac{d \sigma}{dp_T dy_1 dy_2} = \frac{8 \pi p_T}{\hat{s}} \sum_{ijkl} x_{1} x_{2} \left[ f_{i}(x_{1}, \mu_F^2) f_{j}(x_{2}, \mu_F^2) \frac{d \sigma_{ij \rightarrow kl}}{d \Omega} (\hat{s}, \hat{t}, \hat{u}) +  f_{i}(x_{2}, \mu_F^2) f_{j}(x_{1}, \mu_F^2) \frac{d \sigma_{ij \rightarrow kl}}{d \Omega} (\hat{s}, \hat{u}, \hat{t}) \right] \ \ (Eq. 2)
```

If i and j are identical particle types, the second term $d \sigma_{ij \rightarrow kl} / d\Omega$ should be dropped.

For this project the hard scale is implemented i.e. $\mu_F \approx p_T$

$d \sigma_{ij \rightarrow kl}/d \Omega$ can be found in  in [2](#sources) (Eq. 51.4 - 51.12)

The formulas for the needed cross section can be obtained with kinematic equations from [1](#sources) (Eq.206-211)

```math
\frac{d \sigma}{dp_T} = \int dy_1 \int dy_2 \frac{d \sigma}{dp_T dy_1 dy_2} \ \ (Eq. 3)
```

```math
\frac{d \sigma}{d \Delta y} = \int dp_T \int dy_1 \int dy_2 \frac{d \sigma}{dp_T dy_1 dy_2} \delta(\Delta y - |y_1 - y_2|) \ \ (Eq. 4)
```

What you should do (for $d\sigma / d p_T$):

As a foundation, for C++ use include/RunAnalythic.hpp and src/RunAnalythic.cpp, while for python use scripts/run_analythic.py. Look for "To do" in the comments for hints.

<details>
<summary>How to run C++ executable</summary>

Check the code include/RunAnalytic.hpp and src/RunAnalytic.cpp and finish it. After adding updates, compile the code with make and you can run it with

```sh
bin/AddAnalytic input/pp7TeV.yaml 100
```

Where the first argument provides the name of the file with all important specifications, and the second one provides the number of integration steps for Monte Carlo integration.
</details>

<details>
<summary>How to execute python code</summary>

Check the code srcipts/run_analytic.py and finish it. After adding updates you can run it with

```sh
python scripts/calcylate_analytic.py -i input/pp7TeV.yaml -n 100
```

Or alternatively 

```sh
./scripts/run_pythia.py -i input/pp7TeV.yaml -n 1000
```

Where the "-i" argument provides the name of the file with all important specifications, and "-n" provides the number of integration steps for Monte Carlo integration.
</details>

Lower details for $d \sigma / d\hat{t}$ are described. Similarly, $d \sigma / d \Delta y$ can be calculated after minor adjustments.

### Preparing the code:

1. Convert $d\sigma / d\hat{t}$ to $d \sigma / d \Omega$ and obtain obtain Eq.2 from Eq.51.42 in [2](#sources)
2. Create LHAPDF::PDF object, invoke LHAPDF::PDF::mkPDF for this object while passing the pdf, and use it later to obtain $\alpha_{S}$ and $x f(x, \mu_{F})$ values)
3. Declare and/or define a function that returns $x_1$ and $x_2$ for the given $p_T$, $\sqrt{s_{NN}}$, $y_1$, and $y_2$
4. Deduce the formulas to calculate $\hat{s}$, $\hat{t}$, $\hat{u}$
5. Declare and/or define a function to calculate $d \sigma / d \Omega$ for each 2->2 hard interaction ([2](#sources) (Eq. 51.4 - 51.12)) for the given $\hat{s}$, $\hat{t}$, and $\hat{u}$

### MC integration:

1. Create 2-D histogram containing $d \sigma / dp_T dy_1$ vs $p_T$ vs $y_1$ (you don't need to fill in $y_2$ as you can obtain it from $y_1$ and $p_T$)
2. Iterate over $pT$ bins of the $y_1$ vs $p_T$ histogram. For each $p_T$ bin create a cycle (recommended minimal number of iterations: 10000) in which $y_1$ and $y_2$ generate uniformly within kinematic region. For each iteration of this cycle
    1. Calculate $x_1$ and $x_2$ and check whether these values lie within physically possible kinematic region 
    2. Calculate $\hat{s}$, $\hat{t}$, and $\hat{u}$
    3. Calculate $d \sigma / d \hat{t}$ for each hard interaction
    4. Calculate $d \sigma / dp_T dy_1 dy_2$ by summing over all hard processes interactions 
    5. Apply sampling re-weight to $y_1$ (see explanation below) as $y_2$ can be obtained with $y_1$ and $p_T$ and perform steps 1-4 again. Algorithm in steps 1-5 is recommended to be performed at least 10 times.
    6. Fill $d \sigma / dp_T dy_1$ vs $y_1$ vs $p_T$ histogram with obtained result
3. For each $p_T$ bin divide the resulting histogram (for all $y_1$ values) by the number of successful integration steps to obtain normalized #d \sigma / dp_T dy_1 dy$ distribution

<details>
<summary>Sampling re-weight</summary>

Let's assume you have an integral over N-dimensional function

```math
I = \int_{x_1, x_2, ..., x_N} f(x_1, x_2, ... x_N) dx_1 dx_2 ... dx_N
```

Unweighted MC samples each $x_i$ uniformly. However this approach is unreliable (without extensive computations) for functions that have most of the integral within a narrow $x_i$ region and/or extreme slopes (for example invariant $p_T$ spectra). The higher the dimensionality of the function the more unreliable unweighted MC becomes as the biggest part of the integral is concentrated on the thin N-1 dimensional surface which is much smaller than N-dimensional cube we sample over.

Weighted MC sampling makes the choice of $x_i$ to be concentrated proportionally to the function significance in $x_i$ region. This can be achieved by iteratively performing the following algorithm:

1. Separate the N-dimensional sampling region into large number N-dimensional cubes which contain the same integrand over $f(x_1, x_2, ... x_N)$ (for the first iteration separation is uniform, i.e the sampling is uniform, as we haven't calculated the $I$ yet)
2. Perform the integration in each cube by sampling $x_i$ uniformly. The full integration is then a sum of of integrals over each cube.
3. Perform steps 1 and 2 until convergence is achieved (number of cubes you divide the sampling region can also vary, and it is better to increase it for each iteration).

For this work 10 iterations would be enough as the integration is 1-dimensional for $d\sigma / dp_T$ (since $y_2$ can be obtained from $y_1$ and $p_T$) and 2-dimensional for $d\sigma / d \Delta y$.
Minimum recommended value of the sampling region divisions: 100 for $d \sigma / dp_T$; 1000 for $d\sigma / d \Delta y$ (at least 100 along $p_T$ and at least 10 along $y_1$)

</details>

Finally, $d \sigma / dp_T$ can be obtained by estimating $\Delta y_1$ as bin rapidity coverage (or as an approximation of different $p_T$ bin rapidity coverages) for each $p_T$ bin.

## MadGraph NLO

Instructions will appear soon

# Sources

1. [Michelangelo L. Mangano "Introduction to QCD"](https://cds.cern.ch/record/454171/files/p53.pdf)
2. [F. Takahashi et al. (Particle Data Group), Int. J. Mod. Phys. A 41 , 2630011 (2026)](https://pdg.lbl.gov/2026/reviews/contents_sports.html)
