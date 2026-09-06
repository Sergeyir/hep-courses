# Overview

This repository contains my courses in HEP software packages and programming. Courses/projects are organized into separate directories. Each project README.md contains list of required packages and instructions in README.md and code that can be used as a foundation for your code.

Instructions for installing all HEP packages are provided in [Installation tutorial](../INSTALLATION_TUTORIAL.md). Each project' README.md contains a list of required packages. If you are new to HEP packages It is recommended for you to follow the instructions. But if you are experienced and don't want to re-install packages and/or create/redefine environmental variables, see $"PACKAGE"_PATH in CMakeLists.txt and in INSTALLATION_TUTORIAL.md (where "PACKAGE" can be interchanged with the package name, e.g. ROOT, HEPMC, etc.) so you can reconfigure it yourself.

# Installation

You can clone this repository to the current location by running (though you have to install git first via package manager)

```sh
git clone https://github.com/Sergeyir/hep-courses --depth=1
```

Head into the cloned repository and then into the directory of a desired project and check whether packages required for it are installed in README.md. For each project you can either use C++ and/or python. The code for C++ is located in src and include directories. It can be compiled by first generating Makefile with CMake and then running make:

```sh
cmake .
make -j
```

In each project directory rootlogon.C file is present to provide an option for ROOT Cling usage instead of g++ for C++ (after removing/renaiming main function and adding function with the same name as the macro to the code).

Python code is located in scripts directories.

If you need to update the repository to the newest version, run

```sh
git pull
```

<details>
<summary> Implementing your changes</summary>
Since the project may be updated you may need to pull the changes. This way implementing your changes to the code may cause version conflict within git. To circumvent this you can create a branch of this repo or just copy the contents of this repository to your directory and remove CMake files and cache:

```sh
rm -r CMakeFiles cmake_install.cmake CMakeCache.txt
```
<\details>
