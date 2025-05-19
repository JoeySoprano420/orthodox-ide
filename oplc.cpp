#include "opl_parser.h"
#include "opl_emitter.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: oplc <input.opl>" << std::endl;
        return 1;
    }

    std::string inputPath = argv[1];
    try {
        auto ast = OPLParser::Parse(inputPath);
        OPLEmitter emitter;
        emitter.EmitASM(ast, "build/output.asm");
        std::cout << "Assembly written to build/output.asm" << std::endl;
    } catch (std::exception& ex) {
        std::cerr << "Compilation error: " << ex.what() << std::endl;
        return 2;
    }

    return 0;
}
