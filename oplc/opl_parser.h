
#pragma once
#include <string>
#include <vector>
#include <memory>

struct ASTNode {
    std::string type;
    std::string value;
    std::vector<std::shared_ptr<ASTNode>> children;
};

class OPLParser {
public:
    static std::shared_ptr<ASTNode> Parse(const std::string& filepath);
};
