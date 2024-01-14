/**
 * Eva LLVM executable.
 */

#include "./src/EvaLLVM.h"

#include <string>
int main(int argc, char *argv[]) {

  /**
   * Program to execute
   * TODO: why is this a raw string?
   */
  std::string program = R"(
  (printf "Value: %d\n" 42)
  )";

  /**
   * Compiler instance.
   */
  EvaLLVM vm;

  /**
   * Generate LLVM IR.
   */
  vm.exec(program);

  return 0;
}
