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

std::vector<int> subsequent_lcps(sa::sa_index index) {
    int n = (int)index.suffixArray.size();
    std::vector<int> result(n-1);
    for (int i = 0; i + 1 < n; i++)
        result[i] = index.naive_lcp(index.suffixArray[i], index.suffixArray[i+1]);
    return result;
}

double vector_mean(const std::vector<int>& v) {
    double sum = 0;
    for (int x: v) sum += x;
    return sum / (double)v.size();
}

int vector_max(const std::vector<int>& v) {
    int max_value = 0;
    for (int x: v) max_value = std::max(max_value, x);
    return max_value;
}

int vector_median(std::vector<int> v) {
    sort(v.begin(), v.end());
    return v[(int)v.size()/2];
}

std::vector<int> sample(const std::vector<int>& v, int sample_rate) {
    std::vector<int> result;
    for (int i = 0; i < v.size(); i += sample_rate) {
        result.push_back(v[i]);
    }
    return result;
}

void write_queries_answer(gr::fasta_reader fastaReader, sa::sa_index index, std::string mode, std::string output) {
    std::ofstream out(output);
    for (auto g : fastaReader.genomes) {
        sa::query q(g.header, g.content);
        q.perform(index, mode);
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
