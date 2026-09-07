# Table of contents

- [ROOT6](#root6)
- [LHAPDF6](#lhapdf6)
- [PYTHIA8](#pythia8)
- [FASTJET3](#fastjet3)
- [HepMC](#hepmc)
- [HERWIG7](#herwig7)
- [POWHEG-BOX](#powheg-box)
- [MadGraph5_aMC@NLO](#madgraph)

# Overview

This .md document shows concise instructions for minimal installation of different packages used in [hep-courses](https://github.com/Sergeyir/hep-courses) repository. You don't need to install all packages listed here, but only packages that are required for your project. For further detail or more possibilities of installation consult the respective package website, documentation, and README file.

# Installation tutorial

First install python3-dev via package manager. On debian-based systems python-is-python3 (can also be installed via package manager) is also required.

For simplicity we will consider arbitrary path $PACKAGE_PATH for the directory path where we will place all packages required for this project. This consideration is not a requirement and you can configure the paths yourself the way it is convenient for you.

**Recommended $PACKAGE_PATH**: ~/Packages

**Important:**Later if you see $PACKAGE_PATH replace it with your path. Or add it in your profile (~/.bashrc or ~/.zshrc, or other)
And don't forget to **create** this path first

<details>
<summary> How to determine which profile you have </summary>

Run in terminal

```sh
echo $SHELL
```

Which will either return /usr/bin/bash or /usr/bin/zsh, and your profile is ~/.bashrc or ~/.zshrc respectively (this is true for most of modern linux distributions).
</details>

<summary> How to update profile </summary>

For bash
```sh
source ~/.bashrc
```
For zsh
```sh
source ~/.zshrc
```

</details>

## ROOT6

Install the [required dependencies](https://root.cern/install/dependencies/).

Then install ROOT6 via package manager (if available, [see here](https://root.cern/install/)) or pre-compiled release for your platform [here](https://root.cern/install/all_releases/).

<details>
<summary> Installing the pre-compiled release </summary>

If your option is the installation of pre-compiled release, after downloading place the .tar.gz file in $PACKAGE_PATH. After heading in $PACKAGE_PATH substitute the "name_of_file.tar.gz" for your .tar.gz file name containing ROOT6 package and run

```sh
tar -xvzf "name_of_file.tar.gz"
```

Now extracted files are located in $PACKAGE_PATH/root which you should export in your profile (usually ~/.bashrc or ~/.zshrc; or other) to add search option in CMake or for python3 support. To do this as well as add easily accessible root executable add the following lines to your profile

```sh
export ROOT_PATH=$PACKAGE_PATH/root
source $ROOT_PATH/bin/thisroot.sh
```
</details>

<details>
<summary> Building ROOT from source </summary>

Alternatively, you can also compile ROOT6 yourself, however it will take quite a lot of processing power and might take a long time to compile, especially on low-end or old machines. This step is **not recommended** if you do not have enough experience compiling packages from source. But if you are determined see [Building ROOT from source](https://root.cern/install/#build-from-source)

</details>

<details>
<summary> Checking the installation </summary>

```sh
root --version
```

or alternatively in python

```py
import root
print(ROOT.gROOT.GetVersion())
```

With succesfull intallation you would get the version of ROOT package in both cases without any errors

</details>

## LHAPDF6

LHAPDF6 is available for installation via some package managers (see "System package managers" [here](https://www.lhapdf.org/install.html)). If installation with package manager fails, and the following instructions don't work, or you want to configure installation for yourself consult the [LHAPDF6 official installation tutorial](https://www.lhapdf.org/install.html)

<details>
<summary> Installation with package manager </summary>

If you installed lhapdf with the package manager set the environmental variable in your profile of pdf directory to avoid saving pdfs in root directory (recommended to save in $PACKAGE_PATH/lhapdfsets, but you can change it to the directory you want):

```sh 
export LHAPDF_DATA_PATH=$PACKAGE_PATH/lhapdfsets
``` 

After that run to load index in the pdf directory

```sh
lhapdf update
```
</details>

If the package manager installation option is unavailable to you, follow the instructions below.

<details>
<summary> Manual installation and compilation </summary>

Download the source code from [here](https://lhapdf.hepforge.org/downloads/). Place the downloaded .tar.gz file in $PACKAGE_PATH. After heading there run the following command while substituting the "name_of_file.tar.gz" for your .tar.gz file

```sh
tar -xvzf "name_of_file.tar.gz"
```

Then head into LHAPDF-6.X.X directory (6.X.X is the version you downloaded, replace X for your version). First create the build directory and run configure

```sh
mkdir build ; ./configure --prefix=`pwd`/build
```
This step would generate the Makefile, which you can then use for compillation by calling

```sh
make install -j
```

Lastly add the following lines to your profile (replace 6.X.X with your LHAPDF6 version and python3.XX with the version of python you compiled LHAPDF with - look for $PACKAGE_PATH/build/lib/python* directory)

```sh
export LHAPDF6_PATH=$PACKAGE_PATH/LHAPDF-6.X.X/build/
export LD_LIBRARY_PATH=$LHAPDF6_PATH/lib:$LD_LIBRARY_PATH
export PATH=$LHAPDF6_PATH/bin:$PATH
```

Also add the path to \_\_init.py\_\_ located somewhere in $LHAPDF6_PATH/lib/python3.XX/. This path might change with the distro, python, and LHAPDF versions. The resulting line you need to add might look like the following

```sh
export PYTHONPATH=$LHAPDF6_PATH/lib/python3.XX/site-packages:$PYTHONPATH
```

or

```sh
export PYTHONPATH=$LHAPDF6_PATH/lib/python3.XX/dist-packages/lhapdf:$PYTHONPATH
```

</details>

<details>
<summary> Checking the installation </summary>

Run in terminal

```sh
lhapdf --version
```

You can also check whether lhapdf works in python (which is useful for simple installation of pdf sets)

```python
import lhapdf
print(lhapdf.version())
```

With succesfull compilation you would get the version of lhapdf package in both cases without any errors

</details>

## PYTHIA8

PYTHIA8 is available for installation via some package managers (such as pacman, dnf, portage, etc.). If your system package manager doesn't have pythia8 repository you can follow instructions below (additionally python-pythia8 may be required if available or similar package). Also you need to check whether pythia8 package depends on lhapdf package, otherwise you may need to install it manually since libpythia8lhapdf6.so library might not be installed.

<details>
<summary> Manual installation </summary>

Download the source code from [here](https://pythia.org/). Place the downloaded .tgz file in $PACKAGE_PATH. After heading there substitute the "name_of_file.tgz" for your .tgz file name containing PYTHIA8 package and run

```sh
tar -xvzf "name_of_file.tgz"
```

Then head into pythia8XXX directory (8XXX is the version you downloaded, replace X for your version). If you have troubles with the following instructions or you want to configure the installation for yourself consult the README file in pythia8XXX directory. Otherwise, first run to configure (you might need to change /bin/python-config to /bin/python3-config), which will generate Makefile

```sh
./configure --with-python-config=/bin/python-config --with-lhapdf6-config=$LHAPDF6_PATH/bin/lhapdf-config
```

Now compile the source code with the following command

```sh
make
```

After the compilation is finished successfully, add the following lines to your profile (replace X to match your pythia8 version number)

```sh
export PYTHIA8_PATH=$PACKAGE_PATH/pythia8XXX
export PYTHONPATH=$PYTHIA8_PATH/lib:$PYTHONPATH
export LD_LIBRARY_PATH=$PYTHIA8_PATH/lib:$LD_LIBRARY_PATH
```
</details>

<details>
<summary> Checking the installation </summary>

Run in python

```sh
import pythia8
pythia8.Pythia()
```
With succesfull compilation you would get the banner of pythia package without any errors

</details>

## FASTJET3

FASTJET3 can be installed via some package managers (such as pacman, dnf, portage, etc.). If your system package manager doesn't have fastjet repository you can follow instructions below. And if the following instructions don't work or you want to configure it for yourself follow the [FatJet3 official manual instructions](https://fastjet.fr/repo/fastjet-doc-3.5.1.pdf#page=58). For debian try installing packages listed [here](https://packages.debian.org/source/sid/fastjet)

<details>
<summary> Manual installation </summary>

Install [swig](https://www.swig.org/) package first (it can be installed with package manager). After, download the source code from [here](https://fastjet.fr/). Place the .tar.gz file in $PACKAGE_PATH. After heading there run to extract files

```sh
tar -xvzf fastjet-3*.tar.gz
```

Then head into fastjet-3.X.X directory (3.X.X is the version you downloaded, replace X for your version). First run to generate Makefile


```sh
mkdir build ; ./configure --prefix=`pwd`/build --enable-pyext --enable-allplugins
```

After compile the code by running

```sh
make install -j
```

Lastly add to your profile (replace 3.X.X and 3.XX with your fastjet and python version numbers respectively)

```sh
export FASTJET3_PATH=$PACKAGE_PATH/fastjet-3.X.X/build
export PYTHONPATH=$FASTJET3_PATH/lib/python3.XX/site-packages:$PYTHONPATH
export LD_LIBRARY_PATH=$FASTJET3_PATH/lib:$LD_LIBRARY_PATH
```

</details>

<details>
<summary> Checking the installation </summary>

Run in python

```py
import fastjet
print(fastjet.pi)
```

With succesfull intallation you would get the pi number without any errors

</details>

## HepMC

First find the package for your distribution containing VDT library (can be cern-vdt, libvdt-dev) and install it via package manager. This library is a requirement for HepMC. 

<details>
<summary> Intalling via package manager </summary>

HepMC can be installed via package manager on some distributions. See if your package manager contain hepmc package [here](https://gitlab.cern.ch/hepmc/HepMC3). Also note, that libraries for python need to be installed separately, which can be done with package manager or pip, if available. 

</details>

<details>
<summary> Manual installation </summary>

First I recommend first heading into $PACKAGE_PATH directory and then you clone the repository:


```sh
git clone https://gitlab.cern.ch/hepmc/HepMC3 --depth=1 --recurse-submodules
```

Then head into HepMC3 directory and run cmake to generate a Makefile

```sh
cmake . 
```

```sh
make -j2
```

You can set higher number of threads for compilation (with -jN option), if you have sufficient RAM (more than ~2.5 GB per thread) otherwise you may run out of it.

Finally set environmental variables. Note that 3.XX.XX in $PYTHONPATH corresponds to your python version. To find what version HepMC3 was compiled with check out $HEPMC3\_PATH/python/A/ directory

```sh
export HEPMC3_PATH=$PACKAGE_PATH/HepMC3
export LD_LIBRARY_PATH=$HEPMC3_PATH/outputs/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$HEPMC3_PATH/python/A/3.XX.XX/pyHepMC3:$PYTHONPATH
```

</details>

<details>
<summary> Checking the installation </summary>

Run in python

```py
import pyHepMC3
print(pyHepMC3.HepMC.version())
```

With succesfull intallation you would get the version of HepMC package without any errors

</details>

## HERWIG7

Herwig requiers fortran compiler (gfortran), boost library, cython (python3-cython or python-cython or install it via pip), and autoreconf (automake package) for installation.

In order to avoid compling boost library that is required by Herwig7, install it via package manager. Create the directory for Herwig7 (prefferably in your $PACKAGE_PATH), for example:

```sh
mkdir herwig && cd herwig
```

Then run to install herwig-boostrap script

```sh
wget https://herwig.hepforge.org/downloads/herwig-bootstrap
```

Herwig also requires some pdfs in LHAPDF already downloaded at the compilation step. To download them run

```sh
lhapdf install CT14lo CT14nlo
```

If you have succesfully finished the installation of previous packages and specified the corresponding environmental variables, you can run to compile everything needed for Herwig7. Even though many unnecessary packages are excluded and the lite option is chosen, the compilation can take a long time and it is recommended to avoid using -jN option (where N - number of threads) unless you are sure that your machine can handle it (at least 2GB of RAM per thread and enough thermal and cooling capacity of your CPU).

```sh
./herwig-bootstrap --without-boost --with-fastjet=`$FASTJET3_PATH/bin/fastjet-config --prefix` \
                   --with-lhapdf=`$LHAPDF6_PATH/bin/lhapdf-config --prefix` \
                   --with-hepmc=`$HEPMC3_PATH/HepMC3-config.in --prefix` \
                   --without-evtgen --without-fastjet_contrib --without-yoda --lite .
```

Set the following environmental variables so it would be easier to use Herwig:

```sh
export HERWIG7_PATH=$PACKAGE_PATH/herwig
export PATH=$HERWIG7_PATH/bin:$PATH
```

## POWHEG-BOX

Clone the POWHEG-BOX-V2 repository in the preffered directory ($PACKAGE_PATH is recommended)

```sh
git clone https://gitlab.com/POWHEG-BOX/V2/POWHEG-BOX-V2 --depth=1
```

Then head in POWHEG-BOX-V2 directory. Powheg contains many submodules, and in each submodule there is a code for the MC calculation of the specific process. Below as an example the "dijet" submodule is presented, but you can substitute it by the name of the submodule you need.

```sh
git submodule update --remote --init "dijet"
```

PowHeg cannot find headers by itself (although it can find configs). So you would need to add the following lines to your profile if you installed LHAPDF6 and/or FastJet3 manually

```sh
export CPATH=$LHAPDF6_PATH/include:$CPATH # if lhapdf was installed manually
export CPATH=$PACKAGE_PATH/fastjet-3.X.X/include:$CPATH # if fastjet was installed manually
```

Then head into dijet directory and run to compile all needed for our purposes code

```sh
make
```

The installation is succesfull if no errors occured during the compilation.

Compilation will also generate executable pwhg_main and testrun- directories. Head into one of them - testrun-lhc and check if everything works

```sh
../phwg_main
```

## MadGraph

For MadGraph (i.e. MadGraph5_aMC@NLO) no compilation is needed. The code instead is compiled for the process you choose to generate. To install MadGraph clone its repository to after heading to the desired directory (recommended directory - your $PACKAGE_PATH).

```sh
git clone https://github.com/mg5amcnlo/mg5amcnlo --depth=1
```

Then add $PATH variable in your profile in order to use mg5_aMC executable from any directory:

```sh
export PATH=$PACKAGE_PATH/mg5amcnlo/bin:$PATH
```

To test whether MadGraph is working run in the terminal to launch MadGraph interface after updating your profile

```sh
mg5_aMC
```

If you already have MadGraph installed, try to pull the latest version by heading to the $PACKAGE_PATH/mg5amcnlo and running

```sh
git pull
```
