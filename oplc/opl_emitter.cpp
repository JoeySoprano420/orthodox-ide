
#include "opl_emitter.h"
#include <fstream>

void OPLEmitter::EmitASM(std::shared_ptr<ASTNode> root, const std::string& outputPath) {
    std::ofstream out(outputPath);
    for (const auto& child : root->children) {
        out << "; " << child->value << "\n";
        out << "nop\n";
    }
}
