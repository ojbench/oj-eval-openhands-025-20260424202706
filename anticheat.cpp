#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

#include "lang.h"
#include "visitor.h"

class StructureAnalyzer : public Visitor<std::string> {
 public:
  std::string visitProgram(Program *node) override {
    std::string result = "P[";
    for (auto func : node->body) {
      result += visitFunctionDeclaration(func) + ";";
    }
    result += "]";
    return result;
  }
  
  std::string visitFunctionDeclaration(FunctionDeclaration *node) override {
    return "F(" + std::to_string(node->params.size()) + "," + visitStatement(node->body) + ")";
  }

  std::string visitExpressionStatement(ExpressionStatement *node) override {
    return "E(" + visitExpression(node->expr) + ")";
  }
  
  std::string visitSetStatement(SetStatement *node) override {
    return "S(" + visitExpression(node->value) + ")";
  }
  
  std::string visitIfStatement(IfStatement *node) override {
    return "I(" + visitExpression(node->condition) + "," + visitStatement(node->body) + ")";
  }
  
  std::string visitForStatement(ForStatement *node) override {
    return "L(" + visitStatement(node->init) + "," + visitExpression(node->test) + "," + 
           visitStatement(node->update) + "," + visitStatement(node->body) + ")";
  }
  
  std::string visitBlockStatement(BlockStatement *node) override {
    std::string result = "B[";
    for (auto stmt : node->body) {
      result += visitStatement(stmt) + ";";
    }
    result += "]";
    return result;
  }
  
  std::string visitReturnStatement(ReturnStatement *node) override {
    return "R(" + visitExpression(node->value) + ")";
  }

  std::string visitIntegerLiteral(IntegerLiteral *node) override {
    return "N";
  }
  
  std::string visitVariable(Variable *node) override {
    return "V";
  }
  
  std::string visitCallExpression(CallExpression *node) override {
    std::string result = "C(" + node->func + ",";
    for (auto expr : node->args) {
      result += visitExpression(expr) + ",";
    }
    result += ")";
    return result;
  }
};

class Length : public Visitor<int> {
 public:
  int visitProgram(Program *node) override {
    int l = 0;
    for (auto func : node->body) {
      l += visitFunctionDeclaration(func);
    }
    return l;
  }
  
  int visitFunctionDeclaration(FunctionDeclaration *node) override {
    return visitStatement(node->body) + 1;
  }

  int visitExpressionStatement(ExpressionStatement *node) override {
    return visitExpression(node->expr) + 1;
  }
  
  int visitSetStatement(SetStatement *node) override {
    return visitExpression(node->value) + 1;
  }
  
  int visitIfStatement(IfStatement *node) override {
    return visitExpression(node->condition) + visitStatement(node->body) + 1;
  }
  
  int visitForStatement(ForStatement *node) override {
    return visitStatement(node->init) + visitExpression(node->test) + 
           visitStatement(node->update) + visitStatement(node->body) + 1;
  }
  
  int visitBlockStatement(BlockStatement *node) override {
    int l = 1;
    for (auto stmt : node->body) {
      l += visitStatement(stmt);
    }
    return l;
  }
  
  int visitReturnStatement(ReturnStatement *node) override { 
    return visitExpression(node->value) + 1; 
  }

  int visitIntegerLiteral(IntegerLiteral *node) override { return 1; }
  int visitVariable(Variable *node) override { return 1; }
  
  int visitCallExpression(CallExpression *node) override {
    int l = 1;
    for (auto expr : node->args) {
      l += visitExpression(expr);
    }
    return l;
  }
};

int levenshteinDistance(const std::string& s1, const std::string& s2) {
  int m = s1.length();
  int n = s2.length();
  std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));
  
  for (int i = 0; i <= m; i++) dp[i][0] = i;
  for (int j = 0; j <= n; j++) dp[0][j] = j;
  
  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n; j++) {
      if (s1[i-1] == s2[j-1]) {
        dp[i][j] = dp[i-1][j-1];
      } else {
        dp[i][j] = 1 + std::min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]});
      }
    }
  }
  
  return dp[m][n];
}

double structuralSimilarity(const std::string& s1, const std::string& s2) {
  int dist = levenshteinDistance(s1, s2);
  int maxLen = std::max(s1.length(), s2.length());
  if (maxLen == 0) return 1.0;
  return 1.0 - (double)dist / maxLen;
}

int main() {
  Program *prog1 = scanProgram(std::cin);
  Program *prog2 = scanProgram(std::cin);
  
  std::string input;
  int c;
  while ((c = std::cin.get()) != EOF) {
    input += c;
  }

  // Calculate multiple similarity metrics
  int len1 = Length().visitProgram(prog1);
  int len2 = Length().visitProgram(prog2);
  
  std::string struct1 = StructureAnalyzer().visitProgram(prog1);
  std::string struct2 = StructureAnalyzer().visitProgram(prog2);
  
  // Length similarity
  double lenSim = 1.0 - std::min(1.0, std::abs(len1 - len2) / (double)std::max(len1, len2));
  
  // Structural similarity
  double structSim = structuralSimilarity(struct1, struct2);
  
  // Combined score
  double similarity = 0.3 * lenSim + 0.7 * structSim;
  
  // Map to 0-1 range with 0.5 as neutral
  double score = 0.5 + 0.5 * similarity;
  score = std::max(0.0, std::min(1.0, score));
  
  std::cout << score << std::endl;
  return 0;
}
