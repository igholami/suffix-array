#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>

#include "fasta_reader.h"
#include "sa_index.h"
#include "cereal/archives/binary.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/map.hpp"
#include "cereal/types/tuple.hpp"

namespace po = boost::program_options;
namespace gr = genome_reader;
namespace sa = suffix_array;

class input_parser {

public:
    void parse(int argc, char* argv[]) {
        po::options_description desc;
        desc.add_options()
                ("reference", po::value<std::string>(&this->reference)->required(), "Path to genome reference in FASTA format")
                ("k", po::value<int>(&this->k)->required(), "Size of the prefix for prefix lookup table")
                ("output", po::value<std::string>(&this->output)->required(), "Path for index of suffix array")
                ;
        po::positional_options_description pd;
        pd.add("reference", 1);
        pd.add("k", 1);
        pd.add("output", 1);
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).positional(pd).run(), vm);
        po::notify(vm);
    }

    std::string reference;
    std::string output;
    int k{};
};

int main(int argc, char* argv[]) {
    input_parser inputParser;
    inputParser.parse(argc, argv);

    gr::fasta_reader fastaReader(inputParser.reference);

    sa::sa_index index(fastaReader.whole_genome() + "$", inputParser.k);
    index.build_faster();
    index.build_prefix_lookup_table();

    std::ofstream file(inputParser.output);
    cereal::BinaryOutputArchive archive(file);
    archive(index);
    file.close();

    return 0;
}
