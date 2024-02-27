//
// Created by Iman Gholami on 2/24/24.
//

#ifndef CMSC701_A0_SA_INDEX_H
#define CMSC701_A0_SA_INDEX_H

#include <string>
#include <vector>
#include "cereal/access.hpp"

namespace suffix_array {

    class sa_index {


    public:
        explicit sa_index(std::string input);

        std::vector<int> suffixArray;

        void build();

        template<class Archive>
        void serialize(Archive& archive) {
            archive(this->input, this->suffixArray);
        }

        sa_index();

        int naive_lcp(int suffix1, int suffix2);
        std::pair<int, int> naiveCompare(int suffix, const std::string& pattern);

        void build_fast();
        void build_faster();

        bool suffix_cmp(int suffix1, int suffix2);

        std::pair<int, int> naiveCompare(int suffix, const std::string &pattern, int ensure_cp);

        std::string input;
    };

} // suffix_array

#endif //CMSC701_A0_SA_INDEX_H
