//
// Created by Iman Gholami on 2/24/24.
//

#include "sa_index.h"

#include <utility>

namespace suffix_array {
    sa_index::sa_index(std::string input, int k) {
        this->input = std::move(input);
        this->k = k;
    }

    void sa_index::build_prefix_lookup_table() {
        lookup_table.clear();
        for (int i=0; i<suffixArray.size(); i++) {
            if (suffixArray[i] + k > suffixArray.size())
                continue;
            auto key = input.substr(suffixArray[i], k);
            if (lookup_table.find(key) == lookup_table.end())
                lookup_table[key] = {i, i+1};
            lookup_table[key] = {std::get<0>(lookup_table[key]), i + 1};
        }
    };

    void sa_index::build() {
        int n = (int)this->input.size();
        std::vector<std::pair<std::string, int>> sa(n);
        for (int i = 0; i < n; i++)
            sa[i] = {this->input.substr(i), i};
        sort(sa.begin(), sa.end());
        this->suffixArray.resize(n);
        for (int i = 0; i < n; i++)
            this->suffixArray[i] = sa[i].second;
    }

    void sa_index::build_fast() {
        int n = (int)this->input.size();
        std::vector<int> sa(n);
        for (int i=0; i<n; i++) sa[i] = i;
        sort(
                sa.begin(),
                sa.end(),
                [this](int s1, int s2) {return this->suffix_cmp(s1, s2);}
                );
        this->suffixArray = std::move(sa);
    }

    void sa_index::build_faster() {
        int n = (int)this->input.size();
        std::vector<int> rank(n);
        std::vector<std::pair<std::pair<int, int>, int>> doubled(n);
        for (int i=0; i<n; i++)
            rank[i] = (int)input[i];
        for (int l = 0; (1 << l) <= n; l++) {
            for (int i = 0; i < n; i++) {
                if (i + (1 << l) >= n)
                    doubled[i] = {{rank[i], -1}, i};
                else
                    doubled[i] = {{rank[i], rank[i + (1 << l)]}, i};
            }
            sort(doubled.begin(), doubled.end());
            for (int i=0; i<n; i++)
                if (i == 0 || doubled[i].first != doubled[i-1].first)
                    rank[doubled[i].second] = i+1;
                else
                    rank[doubled[i].second] = rank[doubled[i-1].second];
        }
        this->suffixArray.resize(n);
        for (int i=0; i<n; i++)
            this->suffixArray[i] = doubled[i].second;
    }

    sa_index::sa_index() = default;

    bool sa_index::suffix_cmp(int suffix1, int suffix2) {
        while (
                suffix1 < input.size() and
                suffix2 < input.size() and
                input[suffix1] == input[suffix2]
                )
            suffix1++, suffix2++;
        if (std::max(suffix1, suffix2) == input.size())
            return suffix1 > suffix2;
        return input[suffix1] < input[suffix2];

    }

    int sa_index::naive_lcp(int suffix1, int suffix2) {
        int ans = 0;
        while (
                suffix1 < input.size() and
                suffix2 < input.size() and
                input[suffix1] == input[suffix2]
                )
            suffix1++, suffix2++, ans++;
        return ans;
    }

    std::pair<int, int> sa_index::naiveCompare(int suffix, const std::string& pattern) {
        return naiveCompare(suffix, pattern, 0);
    }

    std::pair<int, int> sa_index::naiveCompare(int suffix, const std::string& pattern, int ensure_cp) {
        int ans = 0;
        suffix += ensure_cp;
        for (int i=ensure_cp; i<pattern.size(); i++) {
            auto c = pattern[i];
            ans ++;
            if (suffix >= this->input.size() or c > this->input[suffix])
                return {-1, ans};
            if (c < this->input[suffix])
                return {1, ans};
            suffix ++;
        }
        ans ++;
        return {0, ans};
    }
} // suffix_array