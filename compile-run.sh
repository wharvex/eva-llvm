#!/usr/bin/zsh

# Compile main
clang++ -o eva-llvm `llvm-config --cxxflags --ldflags --system-libs --libs core` eva-llvm.cpp

# Run main
./eva-llvm

# Execute generated IR
lli ./out.ll

# Print result
echo $?

printf "\n"
