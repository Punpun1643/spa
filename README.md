[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/B246QqbV)
# Team 30

> Please fill in this Readme with the required information, and delete this box.
>
> 1. Note that if you are using the `Windows Startup SPA Solution`, your target environment must be using either `Microsoft Visual Studio 2019` or `Microsoft Visual Studio 2022`.
>   If you are using `Cross-platform Startup SPA Solution`, you will need to decide on your target environment (see [Wiki](https://nus-cs3203.github.io/course-website/contents/tools/version-control-repository.html)).
>   Then, please enter the appropriate information below, giving details on the full toolchain (ie. OS, cmake and make versions.)
> 2. The `.gitigore` is already pre-configured, note that the included libraries files (ie. `Team00/Code00/lib`) must accompany this repository and not to be removed from the repository.
> 3. Put your test cases inside the correct directory (ie. `Team00/Tests00/`) not inside the scratch test directory (ie. `Team00/Code00/tests/`). The `.gitignore` is configured to prevent this.
> 4. If you encounter build issues, please visit the respective FAQ *before* reaching out for help:
>     1. [Windows-Startup-SPA-Solution FAQ](https://nus-cs3203.github.io/course-website/contents/tools/windows-spa.html#faq)
>     2. [Cross-platform-Startup-SPA-Solution FAQ](https://nus-cs3203.github.io/course-website/contents/tools/cross-platform-spa.html#faq)
> 5. Avoid using removed/deprecated features in C++, make sure you use features that exist in C++17. It is not recommended to use an earlier version of C++, if you want to proceed, please specify the C++ version specifically in the build configuration.
> 6. Please rename every reference of `00` (ie. `Team00/Code00`) to your own team number. Since the `.gitignore` is configured to ignore libs, remember to force add the autotester libs back to the repository by running `git add -f Team*/Code*/lib/` after you rename.

## Target Environment

Item | Version
-|-
OS | Mac, macOS
Toolchain | cmake 3.27.6
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
Lee Kang Wei | 83387981 | leekangwei@u.nus.edu            | **OS:** macOS Ventura 13.2.1 <br/> **IDE:** NA <br/> **Toolchain:** Apple clang version 14.0.3 (clang-1403.0.22.14.1)
Lim Wei Liang | 91551321 | lim.weiliang@u.nus.edu            | **OS:** macOS Ventura 13.5.2 <br/> **IDE:** CLion 2023.2 <br/> **Toolchain:** Apple clang version 14.0.3 (clang-1403.0.22.14.1)
Papattarada Apithanangsiri | 99887766 | papattarada.a@u.nus.edu | **OS:** macOS Ventura 13.2 <br/> **IDE:** CLion 2023.2 <br/> **Toolchain:** Apple clang version 14.0.3 (clang-1403.0.22.14.1)
Ryan Ong Ren-An | 90495351 | ryanongra@u.nus.edu            | **OS**: Windows 11 <br/> **IDE:** Microsoft Visual Studio 2022 <br/> **Toolchain:** C++ Clang tools for Windows (16.0.5 - x64/x86)
Tang Swen Yi | 99887766 | john@doe.com            |
Tay Yan Han | 96559670 | tayyanhan@u.nus.edu            | **OS**: Windows 11 <br/> **IDE:** Microsoft Visual Studio 2022 <br/> **Toolchain:** C++ Clang tools for Windows (16.0.5 - x64/x86)
