#include <iostream>
#include <random>
#include <unordered_map>

#include "lang.h"
#include "transform.h"

class Cheat : public Transform {
 private:
  std::unordered_map<std::string, std::string> nameMap;
  std::mt19937 rng;
  int counter = 0;

  std::string generateName() {
    std::string name = "v";
    int n = counter++;
    while (n > 0) {
      name += char('a' + (n % 26));
      n /= 26;
    }
    return name;
  }

  std::string getNewName(const std::string& oldName) {
    if (oldName == "main") return "main";
    if (nameMap.find(oldName) == nameMap.end()) {
      nameMap[oldName] = generateName();
    }
    return nameMap[oldName];
  }

 public:
  Cheat() : rng(42) {}

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
    std::string funcName = node->func;
    if (funcName != "+" && funcName != "-" && funcName != "*" && funcName != "/" && funcName != "%" &&
        funcName != "<" && funcName != ">" && funcName != "<=" && funcName != ">=" &&
        funcName != "==" && funcName != "!=" && funcName != "||" && funcName != "&&" && funcName != "!" &&
        funcName != "scan" && funcName != "print" && funcName != "array.create" && funcName != "array.scan" &&
        funcName != "array.print" && funcName != "array.get" && funcName != "array.set") {
      funcName = getNewName(funcName);
    }
    return new CallExpression(funcName, args);
  }

  Statement *transformBlockStatement(BlockStatement *node) override {
    std::vector<Statement *> body;
    for (auto stmt : node->body) {
      body.push_back(transformStatement(stmt));
    }
    return new BlockStatement(body);
  }

  Expression *transformIntegerLiteral(IntegerLiteral *node) override {
    if (node->value == 0 || node->value == 1) {
      return new IntegerLiteral(node->value);
    }
    int choice = rng() % 3;
    if (choice == 0) {
      return new IntegerLiteral(node->value);
    } else if (choice == 1 && node->value > 0) {
      std::vector<Expression*> args;
      args.push_back(new IntegerLiteral(node->value - 1));
      args.push_back(new IntegerLiteral(1));
      return new CallExpression("+", args);
    } else {
      return new IntegerLiteral(node->value);
    }
  }
};

int main() {
  auto code = scanProgram(std::cin);
  auto cheat = Cheat().transformProgram(code);
  std::cout << cheat->toString();
  return 0;
}
