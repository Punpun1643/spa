![Master]
(https:://github.com/nus-cs3203/23s1-cp-spa-team-30/actions/workflows/master.yaml/badge.svg)

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

### Testing

Unit tests and autotester are ran for each PR, and for each push to master. Additionally, you can view the up-to-date detailed xml autotester output for the master branch [here](https://nus-cs3203.github.io/23s1-cp-spa-team-30/).

#### Editing autotester workflwo and/or output
To edit the autotester workflow and/or the displayed autotester output for the master branch, go to the .github/actions/autotest/action.yml file.

To edit the autotester workflow, edit the part under this comment:
```html
# For developers: Edit autotester output here
```

To edit the displayed autotester output for the master branch, edit the part under this comment:
```html
# For developers: Edit autotester output to be displayed on Github Pages, here
```

### clang-format
#### Requirements
- Requirement: `clang-format` installed on your local environment ([macs](https://formulae.brew.sh/formula/clang-format), [windows](https://clang.llvm.org/docs/ClangFormat.html) - apparently there's a VS plugin)
- For mac, can try run `clang-format --version` to see if it's installed correctly

##### Details
- Auto formatter based on the specified config defined in `.clang-format` at the root directory
- To format all files: `find . -name '*.cpp' -o -name '*.h' | xargs clang-format -i`
- To format specific file: `clang-format -i File_To_Format.cpp`

# Testing

The following are the system test cases in the following format:     
- `<TEST-DESCRIPTION>_source.txt`, the SIMPLE program
- `<TEST-DESCRIPTION>_queries.txt`, its corresponding query file

| Source File                 | Query File                        | Purpose                                         | Results    |
|-----------------------------|-----------------------------------|-------------------------------------------------|------------|
| test1-modifies-source.txt   | test1-modifies-queries.txt        | Modifies                                        | All passed |
| test1-uses-source.txt         | test1-uses-queries.txt            | Uses                                            | All passed |
| test1-source.txt            | test1-parent-queries.txt          | Parent/*                                        | All passed |
| test1-source.txt            | test1-pattern-queries.txt         | Pattern                                         | All passed |
| test2-source.txt            | test2-follows-queries.txt         | Follows/*                                       | All passed |
| test2-source.txt            | test2-uses-queries.txt            | Uses                                            | All passed |
| test2-source.txt            | test2-syntax-invalid-queries      | Invalid Syntax<br>Invalid Semantics             | All passed |
| nesting-source.txt          | nesting-queries.txt               | Deep nesting                                    | All passed |
| multiconditional-source.txt | multiconditional-queries.txt      | condExpr with more than one condition           | All passed |
| test-3-source.txt           | test-3-complex-queries.txt        | 3 clause queries                                | All passed |


# Team Members

|                       Name |  Mobile  | Email                   | Development OS/Toolchain                                                                                                  |
|---------------------------:|:--------:|:------------------------|---------------------------------------------------------------------------------------------------------------------------|
|               Lee Kang Wei | 83387981 | leekangwei@u.nus.edu    | **OS:** macOS Ventura 13.2.1 <br/> **IDE:** NA <br/> **Toolchain:** CMake 3.27.6 + Apple clang 14.0.3                     |
|              Lim Wei Liang | 91551321 | lim.weiliang@u.nus.edu  | **OS:** macOS Ventura 13.5.2 <br/> **IDE:** CLion 2023.2 <br/> **Toolchain:** CMake 3.26.4 + Apple clang 14.0.3           |
| Papattarada Apithanangsiri | 96147377 | papattarada.a@u.nus.edu | **OS:** macOS Sonoma 14.0 <br/> **IDE:** CLion 2023.2 <br/> **Toolchain:** CMake 3.27.4 + Apple clang 14.0.3              |
|            Ryan Ong Ren-An | 90495351 | ryanongra@u.nus.edu     | **OS**: Windows 11 <br/> **IDE:** Microsoft Visual Studio 2022 <br/> **Toolchain:** CMake 3.26 + MSVC 19.37.32822.0       |
|               Tang Swen Yi | 83832882 | e0532612@u.nus.edu      | **OS:** macOS Ventura 13.4.1 <br/> **IDE:** CLion 2023.2.1 <br/> **Toolchain:** CMake 3.26.3 + Apple clang version 14.0.3 |
|                Tay Yan Han | 96559670 | tayyanhan@u.nus.edu     | **OS**: Windows 11 <br/> **IDE:** Microsoft Visual Studio 2022 <br/> **Toolchain:** CMake 3.26 + MSVC 19.37.32822.0       |
