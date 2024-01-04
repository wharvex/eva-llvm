/**
 * Eva to LLVM IR compiler.
 */

#ifndef EvaLLVM_h
#define EvaLLVM_h

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include <iostream>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <memory>

class EvaLLVM {
public:
  EvaLLVM() { moduleInit(); }

  /**
   * Executes a program.
   */
  void exec(const std::string &program) {
    // 1. Parse the program
    // auto ast = parser->parser(program);

    // 2. Compile to LLVM IR:
    compile();

    // Print generated code.
    module->print(llvm::outs(), nullptr);

    std::cout << "\n";

    // 3. Save module IR to file:
    saveModuleToFile("./out.ll");
  }

private:
  /**
   * Compiles an expression.
   */
  void compile(/* TODO: ast */) {
    // 1. Create main function
    auto fn = createFunction(
        "main", llvm::FunctionType::get(/* return type */ builder->getInt32Ty(),
                                        /* vararg */ false));

    // 2. Compile main body
    auto result = gen(/* ast */);

    // 3. Cast to i32 to return from main
    auto i32Result =
        builder->CreateIntCast(result, builder->getInt32Ty(), true);

    builder->CreateRet(i32Result);
  }

  /**
   * Main compile loop.
   */
  llvm::Value *gen(/* exp */) { return builder->getInt32(47); }

  /**
   * Creates a function.
   */
  llvm::Function *createFunction(const std::string &fnName,
                                 llvm::FunctionType *fnType) {
    // Function prototype might already be defined
    auto fn = module->getFunction(fnName);

    // If not, allocate the function
    if (fn == nullptr) {
      fn = createFunctionProto(fnName, fnType);
    }

    createFunctionBlock(fn);
    return fn;
  }

  /**
   * Creates function prototype (defines the function, but not the body)
   */
  llvm::Function *createFunctionProto(const std::string &fnName,
                                      llvm::FunctionType *fnType) {
    auto fn = llvm::Function::Create(
        fnType, /* ExternalLinkage: this function is visible from outside */
        llvm::Function::ExternalLinkage, fnName, *module);
    verifyFunction(*fn);

    return fn;
  }

  /**
   * Creates function block.
   */
  void createFunctionBlock(llvm::Function *fn) {
    auto entry = createBB("entry", /* fn: the parent object to attach to */ fn);

    // The IR builder (the code emitter) works like a state machine.
    // Once we allocate a new block, we must explicitly tell the builder to emit
    // the code to this block.
    builder->SetInsertPoint(entry);
  }

  /**
   * Creates a basic block. If the `fn` is passed, the block is automatically
   * appended to the parent function. Otherwise, the block should later be
   * appended manually via fn->getBasicBlockList().push_back(block);
   */
  llvm::BasicBlock *createBB(std::string name, llvm::Function *fn = nullptr) {
    return llvm::BasicBlock::Create(*ctx, name, fn);
  }

  /**
   * Saves IR to file.
   */
  void saveModuleToFile(const std::string &fileName) {
    std::error_code errorCode;
    llvm::raw_fd_ostream outLL(fileName, errorCode);
    module->print(outLL, nullptr);
  }

  /**
   * Initialize the module.
   */
  void moduleInit() {
    // Open a new context and module.
    ctx = std::make_unique<llvm::LLVMContext>();
    module = std::make_unique<llvm::Module>("EvaLLVM", *ctx);

    // Create a new builder for the module.
    builder = std::make_unique<llvm::IRBuilder<>>(*ctx);
  }

  /**
   * Currently compiling function.
   */
  llvm::Function *fn;

  /**
   * Context
   *
   * It owns and manages the core "global" data of LLVM's core
   * infrastructure, including the type and constant unique tables.
   */
  std::unique_ptr<llvm::LLVMContext> ctx;

  /**
   * Module
   *
   * A Module instance is used to store all the information related to an
   * LLVM module. Modules are the top level container of all other LLVM
   * Intermediate Representation (IR) objects. Each module directly contains a
   * list of global variables, a list of functions, a list of libraries (or
   * other modules) this module depends on, a symbol table, and various data
   * about the target's characteristics.
   *
   * A module maintains a GlobalList object that is used to hold all
   * constant references to global variables in the module. When a global
   * variable is destroyed, it should have no entries in the GlobalList.
   */
  std::unique_ptr<llvm::Module> module;

  /**
   * Builder
   *
   * Provides a uniform API for creating instructions and inserting
   * them into a basic block: either at the end of a BasicBlock, or at a
   * specific iterator location in a block.
   */
  std::unique_ptr<llvm::IRBuilder<>> builder;
};

#endif
