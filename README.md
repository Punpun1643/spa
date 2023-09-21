[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/B246QqbV)
# Team 30
## Target Environment

Item | Version
-|-
OS | Mac, macOS
Toolchain | CMake 3.27.6
C++ Standard | C++17

### Additional Build Instructions

No additional instructions required.

## Continuous Integration

### clang-format
#### Requirements
- Requirement: `clang-format` installed on your local environment ([macs](https://formulae.brew.sh/formula/clang-format), [windows](https://clang.llvm.org/docs/ClangFormat.html) - apparently there's a VS plugin)
- For mac, can try run `clang-format --version` to see if it's installed correctly

##### Details
- Auto formatter based on the specified config defined in `.clang-format` at the root directory
- To format all files: `find . -name '*.cpp' -o -name '*.h' | xargs clang-format -i`
- To format specific file: `clang-format -i File_To_Format.cpp`

# Team Members

Name | Mobile | Email                   | Development OS/Toolchain
-:|:-:|:------------------------|-|
Lee Kang Wei | 83387981 | leekangwei@u.nus.edu            | **OS:** macOS Ventura 13.2.1 <br/> **IDE:** NA <br/> **Toolchain:** CMake 3.27.6
Lim Wei Liang | 91551321 | lim.weiliang@u.nus.edu            | **OS:** macOS Ventura 13.5.2 <br/> **IDE:** CLion 2023.2 <br/> **Toolchain:** CMake 3.26.4
Papattarada Apithanangsiri | 99887766 | papattarada.a@u.nus.edu | **OS:** macOS Ventura 13.2 <br/> **IDE:** CLion 2023.2 <br/> **Toolchain:**  CMake 3.27.4
Ryan Ong Ren-An | 90495351 | ryanongra@u.nus.edu            | **OS**: Windows 11 <br/> **IDE:** Microsoft Visual Studio 2022 <br/> **Toolchain:** C++ Clang tools for Windows (16.0.5 - x64/x86)
Tang Swen Yi | 99887766 | john@doe.com            |
Tay Yan Han | 96559670 | tayyanhan@u.nus.edu            | **OS**: Windows 11 <br/> **IDE:** Microsoft Visual Studio 2022 <br/> **Toolchain:** C++ Clang tools for Windows (16.0.5 - x64/x86)
