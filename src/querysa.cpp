#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>

#include "fasta_reader.h"
#include "sa_index.h"
#include "query.h"
#include "cereal/archives/binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/vector.hpp"

namespace po = boost::program_options;
namespace gr = genome_reader;
namespace sa = suffix_array;

class input_parser {

public:
    void parse(int argc, char* argv[]) {
        po::options_description desc;
        desc.add_options()
                ("index", po::value<std::string>(&this->index)->required(), "index")
                ("queries", po::value<std::string>(&this->queries)->required(), "Path to the file contains queries")
                ("mode", po::value<std::string>(&this->mode)->required(), "Query mode")
                ("output", po::value<std::string>(&this->output)->required(), "Path for output the result")
                ;
        po::positional_options_description pd;
        pd.add("index", 1);
        pd.add("queries", 1);
        pd.add("mode", 1);
        pd.add("output", 1);
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).positional(pd).run(), vm);
        po::notify(vm);
    }

    std::string index;
    std::string output;
    std::string queries;
    std::string mode;
};

void write_queries_answer(const gr::fasta_reader& fastaReader, const sa::sa_index& index, const std::string& mode, const std::string& output) {
    std::ofstream out(output);
    for (auto g : fastaReader.genomes) {
        sa::query q(g.header, g.content);
        q.perform(index, mode);
        if (mode == "prefaccel")
            out << q.name << "\t" << q.pref_interval_lb << "\t" << q.pref_interval_ub << "\t" << q.hits.size();
        else
            out << q.name << "\t" << q.char_cmp_lb << "\t" << q.char_cmp_ub << "\t" << q.hits.size();
        for (auto hit : q.hits)
            out << "\t" << hit;
        out << "\n";
    }
}


int main(int argc, char* argv[]) {
    input_parser inputParser;
    inputParser.parse(argc, argv);

    sa::sa_index index;
    std::ifstream file(inputParser.index);
    cereal::BinaryInputArchive archive(file);
    archive(index);

    gr::fasta_reader fastaReader(inputParser.queries);

    write_queries_answer(fastaReader, index, inputParser.mode, inputParser.output);
    return 0;
}
