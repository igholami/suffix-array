#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>

#include "fasta_reader.h"
#include "sa_index.h"
#include "cereal/archives/binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/vector.hpp"
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
                ("index", po::value<std::string>(&this->index)->required(), "index")
                ("sample_rate", po::value<int>(&this->sample_rate)->required(), "The rate to sample data")
                ("output", po::value<std::string>(&this->output)->required(), "Path for output the result")
                ;
        po::positional_options_description pd;
        pd.add("index", 1);
        pd.add("sample_rate", 1);
        pd.add("output", 1);
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).positional(pd).run(), vm);
        po::notify(vm);
    }

    std::string index;
    std::string output;
    int sample_rate{};
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


int main(int argc, char* argv[]) {
    input_parser inputParser;
    inputParser.parse(argc, argv);

    sa::sa_index index;
    std::ifstream file(inputParser.index);
    cereal::BinaryInputArchive archive(file);
    archive(index);

    std::vector<int> slcps = subsequent_lcps(index);

    std::ofstream out(inputParser.output);
    out << std::setprecision(10) << std::fixed << vector_mean(slcps) << "\n";
    out << vector_median(slcps) << "\n";
    out << vector_max(slcps) << "\n";

    std::vector<int> sampled = sample(index.suffixArray, inputParser.sample_rate);
    for (int x : sampled)
        out << x << "\t";
    out << "\n";
    return 0;
}
