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

    void query::perform_naive(sa_index index) {
        this->char_cmp_lb = 0;
        this->char_cmp_ub = 0;
        int lb = 0, ub = 0;
        {
            int l = 0, r = (int)index.suffixArray.size();
            while (r - l > 1) {
                int c = (l + r) / 2;
                auto [cmp, cnt] = index.naiveCompare(index.suffixArray[c], pattern);
                if (cmp < 0)
                    l = c;
                else
                    r = c;
                this->char_cmp_lb += cnt;
            }
            lb = r;
        }
        {
            int l = 0, r = (int)index.suffixArray.size();
            while (r - l > 1) {
                int c = (l + r) / 2;
                auto [cmp, cnt] = index.naiveCompare(index.suffixArray[c], pattern);
                if (cmp <= 0)
                    l = c;
                else
                    r = c;
                this->char_cmp_ub += cnt;
            }
            ub = r;
        }
        for (int i=lb; i < ub; i++)
            this->hits.push_back(index.suffixArray[i]);
    }
    void query::perform(const suffix_array::sa_index& index, const std::string& mode) {
        if (mode == "naive")
            query::perform_naive(index);
    }
} // suffix_array