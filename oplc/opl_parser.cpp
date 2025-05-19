
#include "opl_parser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::shared_ptr<ASTNode> OPLParser::Parse(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) throw std::runtime_error("Cannot open file");

    auto root = std::make_shared<ASTNode>();
    root->type = "Program";

    std::string line;
    while (std::getline(file, line)) {
        auto node = std::make_shared<ASTNode>();
        node->type = "Line";
        node->value = line;
        root->children.push_back(node);
    }
    return root;
}
