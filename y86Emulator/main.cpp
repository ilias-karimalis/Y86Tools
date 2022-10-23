#include <iostream>
#include <fstream>
#include "processor.h"

int
main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout << "expected usage: y86Tools <binary_program_file>\n";
        return EXIT_FAILURE;
    }
    std::fstream binary_file;
    binary_file.open(argv[1], std::ios::in | std::ios::binary);

    // Get length of file:
    binary_file.seekg(0, binary_file.end);
    ssize_t program_size = binary_file.tellg();
    binary_file.seekg(0, binary_file.beg);

    std::cout <<"Program Size: " << program_size << "\n";
    uint8_t* program = static_cast<uint8_t*>(malloc(program_size));
    binary_file.read(reinterpret_cast<char *>(program), program_size);

    auto processor = Processor(program, program_size);

    // Add execution or instruction printing or ImGui.
    processor.reset();
    while (processor.status == Processor::Status::AOK) {
        auto instr = processor.fetchInstruction();
        if (!instr.has_value()) {
            std::cout << static_cast<int>(processor.status) << '\n';
            break;
        }
        std::cout << *instr << "\n";
        processor.executeInstruction(*instr);
    }
}