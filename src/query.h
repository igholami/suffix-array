//
// Created by Iman Gholami on 2/26/24.
//

#ifndef CMSC701_A0_QUERY_H
#define CMSC701_A0_QUERY_H

#include <string>
#include <vector>
#include "sa_index.h"

namespace suffix_array {

    class query {
    public:
        std::string name;
        std::string pattern;
        int char_cmp_lb;
        int char_cmp_ub;
        std::vector<int> hits;

        query(std::string name, std::string pattern);

        void perform(suffix_array::sa_index index, std::string mode);
    };

} // suffix_array

#endif //CMSC701_A0_QUERY_H
