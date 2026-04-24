#include <iostream>
#include <random>
#include <unordered_map>
#include <unordered_set>

#include "lang.h"
#include "transform.h"

class Cheat : public Transform {
 private:
  std::unordered_map<std::string, std::string> nameMap;
  std::unordered_set<std::string> builtinFuncs;
  std::mt19937 rng;
  int counter = 0;

  std::string generateName() {
    std::string name = "x";
    int n = counter++;
    if (n == 0) return name;
    while (n > 0) {
      name += char('a' + (n % 26));
      n /= 26;
    }
    return name;
  }

  std::string getNewName(const std::string& oldName) {
    if (oldName == "main") return "main";
    if (builtinFuncs.count(oldName)) return oldName;
    if (nameMap.find(oldName) == nameMap.end()) {
      nameMap[oldName] = generateName();
    }
    return nameMap[oldName];
  }

 public:
  Cheat() : rng(42) {
    builtinFuncs = {"+", "-", "*", "/", "%", "<", ">", "<=", ">=", "==", "!=", 
                    "||", "&&", "!", "scan", "print", "array.create", "array.scan",
                    "array.print", "array.get", "array.set"};
  }

  Variable *transformVariable(Variable *node) override {
    return new Variable(getNewName(node->name));
  }

  FunctionDeclaration *transformFunctionDeclaration(FunctionDeclaration *node) override {
    std::vector<Variable *> params;
    for (auto param : node->params) {
      params.push_back(transformVariable(param));
    }
    return new FunctionDeclaration(getNewName(node->name), params, transformStatement(node->body));
  }

  Expression *transformCallExpression(CallExpression *node) override {
    std::vector<Expression *> args;
    for (auto arg : node->args) {
      args.push_back(transformExpression(arg));
    }
    return new CallExpression(getNewName(node->func), args);
  }

  Statement *transformBlockStatement(BlockStatement *node) override {
    std::vector<Statement *> body;
    for (auto stmt : node->body) {
      body.push_back(transformStatement(stmt));
    }
    return new BlockStatement(body);
  }

  Expression *transformIntegerLiteral(IntegerLiteral *node) override {
    return new IntegerLiteral(node->value);
  }
};

int main() {
  auto code = scanProgram(std::cin);
  auto cheat = Cheat().transformProgram(code);
  std::cout << cheat->toString();
  return 0;
}
