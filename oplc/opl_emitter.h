
#pragma once
#include "opl_parser.h"
#include <string>

class OPLEmitter {
public:
    void EmitASM(std::shared_ptr<ASTNode> root, const std::string& outputPath);
};
