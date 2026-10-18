#include "image_processor.h"

#include <exception>
#include <iostream>

#include "bmp.h"
#include "command_line.h"

namespace imgproc {

int RunImageProcessor(int argc, char* argv[]) {
    try {
        if (argc == 1) {
            std::cout << CommandLineParser::HelpMessage(argv[0]);
            return 0;
        }

        CommandLine command_line = CommandLineParser::Parse(argc, argv);
        Image image = BmpCodec::Load(command_line.input_path);

        for (const FilterPtr& filter : command_line.filters) {
            filter->Apply(image);
        }

        BmpCodec::Save(image, command_line.output_path);
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}

}  // namespace imgproc
