#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>

#include <unistd.h>  // for getopt

#include "MD.h"
#include "Engine.h"

MD::MD(int argc, char **argv)
{
  opterr = 0;
  int c;
  while((c = getopt(argc, argv, "a:hi:o:")) != -1) {
    std::string optString;
    if (isprint(optopt)) {
      optString = "character ";
      char s[2] = {static_cast<char>(optopt)};
      optString += s;
    } else {
      optString = "ASCII " + std::to_string(optopt);
    }
    switch(c) {
      case 'a':
        if (ofs.is_open())
          throw std::invalid_argument("Output file already opened.");
        ofs.open(optarg, std::ofstream::app);
        if (!ofs.is_open())
          throw std::runtime_error("Error opening output file " + 
              std::string(optarg) + ".");
        break;
      case 'h':
        print_help();
        break;
      case 'o':
        if (ofs.is_open())
          throw std::invalid_argument("Output file already opened.");
        ofs.open(optarg);
        if (!ofs.is_open())
          throw std::runtime_error("Error opening output file " + 
              std::string(optarg) + ".");
        break;
      case 'i':
        if (ifs.is_open())
          throw std::invalid_argument("Input file already opened.");
        ifs.open(optarg);
        if (!ifs.is_open())
          throw std::runtime_error("Error opening input file " + 
              std::string(optarg) + ".");
        break;
      case '?':
        std::cerr << "# Option " << optString << " requires an argument, "
                  << "or unknown option " << optString << "."
                  << std::endl;
        throw std::invalid_argument("Invalid command line option. "
            " (line "
            + std::to_string(__LINE__) + ", file " + __FILE__ + ")");
        break;
      default:
        throw std::logic_error("Run into impossible code blocks. "
            "Coding error. (line "
            + std::to_string(__LINE__) + ", file " + __FILE__ + ")");
        break;
    }
  }
  //std::cout << argc << " " << optind << std::endl;
  if (optind < argc) {
    // Unprocessed options
    std::cout << "Unprocessed options: ";
    for (int i = optind; i != argc; ++i)
      std::cout << argv[i] << " ";
    std::cout << std::endl;
    throw std::invalid_argument("Invalid command line option. "
        " (line "
        + std::to_string(__LINE__) + ", file " + __FILE__ + ")");
  }

  engine = std::make_shared<Engine>(ofs.is_open()?ofs:std::cout);
  engine->read(ifs);
}

MD::~MD()
{
  if (ifs.is_open())
    ifs.close();
  if (ofs.is_open())
    ofs.close();
}

void MD::print_help()
{
  std::cout 
    << "MD engine" << std::endl
    << std::endl
    << "-a       Open the output file with append mode." << std::endl
    << "-h       Print this info page." << std::endl
    << "-i       Open the input file." << std::endl
    << "-o       Open the output file." << std::endl
  ;
  exit(0);
}

int MD::run()
{
  //TODO
  engine->run();
  return 0;
}
