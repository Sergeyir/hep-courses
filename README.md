# Overview

This repository contains my courses in HEP software packages and programming. Courses/projects are organized into separate directories.

# Installation

You can clone this repository in the desired location by running

```sh
git clone https://github.com/Sergeyir/hep-courses --depth=1
```

After heading into the directory of a desired project you can either use C++ and/or python. The code for C++ is located in src and include directories and can be compiled by first generating Makefile with CMake and then running make:

```sh
cmake .
make -j
```

Python code is located in scripts directories. It can be directly run by passing the .py file as an argument for python binary.

If you need to update the repository to the newest version, run

```sh
git pull
```

Since the project may be updated you may need to pull the changes. This way implementing your changes to the code may cause version conflict within git. To circumvent this you can create a branch of this repo or just copy the contents of this repository to your directory and remove CMake files and cache:

```sh
rm -r CMakeFiles cmake_install.cmake CMakeCache.txt
```

</details>
