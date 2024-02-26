//
// Created by Iman Gholami on 2/26/24.
//

#include "query.h"

#include <utility>

namespace suffix_array {
    query::query(std::string name, std::string pattern) {
        this->name = std::move(name);
        this->pattern = std::move(pattern);
    }

    void query::perform(suffix_array::sa_index index, std::string mode) {

    }
} // suffix_array