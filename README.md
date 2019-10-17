# Molecular Dynamics Engine

A simple molecular dynamics (MD) engine that runs for any potential energy
funtional with a flexible parser

## Prerequisites
* [GNU libmatheval](https://www.gnu.org/software/libmatheval/)
* [GNU Make](https://www.gnu.org/software/make/)
* [Intel C++ Compiler](https://software.intel.com/en-us/c-compilers) (>=2018)

## Compilation
### Preparation

Prepare the correct Intel compiler fully supporting **C++11** and the GNU libmatheval library.
        
### Run the following commands
```bash
cd src/
make clean
make -j4
```

### The binary will be ready for use

## Test and Examples

See the example/ folder
