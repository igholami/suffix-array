//
// Created by Iman Gholami on 2/26/24.
//

#include "query.h"

#include <utility>

namespace suffix_array {
    query::query(std::string name, std::string pattern, sa_index index, std::string mode) {
        this->name = std::move(name);
        this->pattern = std::move(pattern);
        this->index = std::move(index);
        this->optimization = mode == "simpaccel";
    }

    std::pair<int, int> query::binary_search(bool isUpperBound) {
        int count = 0;
        int compare_threshold = isUpperBound ? 0 : -1;
        int l = 0, r = (int)index.suffixArray.size();
        int leftLCP = 0, rightLCP = 0;
        while (r - l > 1) {
            int c = (l + r) / 2;
            int currentLCP = std::min(leftLCP, rightLCP);
            auto [cmp, cnt] = index.naiveCompare(index.suffixArray[c], pattern, optimization ? currentLCP : 0);
//            std::cout << pattern << " " << index.input.substr(index.suffixArray[c], pattern.size()) << " " << leftLCP << " " << rightLCP << " " << cmp << " " << cnt << "\n";
            if (cmp <= compare_threshold)
                l = c, leftLCP = currentLCP + cnt - 1;
            else
                r = c, rightLCP = currentLCP + cnt - 1;
            count += cnt;
        }
//        exit(0);
        return {r, count};
    }
    void query::perform() {
        auto [lb, lcmp] = binary_search(false);
        auto [ub, ucmp] = binary_search(true);
        this->char_cmp_lb = lcmp;
        this->char_cmp_ub = ucmp;
        for (int i=lb; i < ub; i++)
            this->hits.push_back(index.suffixArray[i]);
    }
} // suffix_array