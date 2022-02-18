#include <common.h>

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>
#include <string>
#include <queue>
#include <set>
#include <map>
#include <cstdlib>
#include <chrono>

using namespace std;

const int kInf = 1000000000;

template<class T>
queue<T> vec_to_queue(const vector<T>& vec) {
    queue<T> q;
    for (auto elem : vec) {
        q.push(elem);
    }
    return q;
}

struct MySolver : public Context {
    set<pair<int, int>> developer_ready_day;
    
    vector<vector<int>> feature_bins;
    vector<queue<pair<int, int>>> feature_bins_remaining;
    
    vector<int> dev_currently_in;
    
    vector<int> services_in_binary;
    vector<int> cur_devs_in_binary;
    
    int incomplete_feature = -1;
    
    set<int> remaining_features;
    
    void Init() {
        Solution.resize(engineers);
        
        for (int i = 0; i < engineers; ++i) {
            developer_ready_day.emplace(0, i);
        }
        
        feature_bins.resize(features_num);
        feature_bins_remaining.resize(features_num);
        
        for (const auto& feature : features) {
            set<int> bins;
            for (int service : feature.services) {
                bins.insert(services[service].binary);
            }
            feature_bins[feature.id] = vector<int>(bins.begin(), bins.end());
        }
        
        dev_currently_in.resize(engineers, -1);
        
        services_in_binary.resize(initial_binaries_num);
        cur_devs_in_binary.resize(initial_binaries_num);
        
        for (const auto& service : services) {
            ++services_in_binary[service.binary];
        }
        
        for (int i = 0; i < features_num; ++i) {
            remaining_features.insert(i);
        }
    }
    
    static void FreeDevs(int day,
                         const set<pair<int, int>>& dev_ready_day,
                         vector<int>& dev_cur_in,
                         vector<int>& cur_devs_in_bin) {
        auto cur_day_beg = dev_ready_day.lower_bound({day, -kInf});
        auto cur_day_end = dev_ready_day.upper_bound({day, kInf});
        for (auto it = cur_day_beg; it != cur_day_end; ++it) {
            auto [_, eng_id] = *it;
            if (dev_cur_in[eng_id] != -1) {
                --cur_devs_in_bin[dev_cur_in[eng_id]];
                dev_cur_in[eng_id] = -1;
                // TODO: update some shit in `feature_bins_remaining` here
            }
        }
    }
    
    vector<pair<int, int>> MakeFeatureBinsRemainingSortedVec(int feature) {
        vector<pair<int, int>> feature_bins_remaining_vec;
        for (int fb : feature_bins[feature]) {
            int cost = features[feature].difficulty + services_in_binary[fb] + cur_devs_in_binary[fb];
            feature_bins_remaining_vec.emplace_back(cost, fb);
        }
        sort(feature_bins_remaining_vec.begin(), feature_bins_remaining_vec.end(), greater<>());
        return feature_bins_remaining_vec;
    }
    
    int Model(int day, int feature_id, queue<pair<int, int>> remaining) const {
        auto dev_ready_day = developer_ready_day;
        auto dev_cur_in = dev_currently_in;
        auto cur_devs_in_bin = cur_devs_in_binary;
        
        for (; !remaining.empty(); ++day) {
            FreeDevs(day, dev_ready_day, dev_cur_in, cur_devs_in_bin);
            
            while (!remaining.empty() && dev_ready_day.begin()->first <= day) {
                auto [_1, eng_id] = pair(*dev_ready_day.begin());
                dev_ready_day.erase(dev_ready_day.begin());
                auto [_2, bin_id] = pair(remaining.front());
                remaining.pop();
                dev_ready_day.emplace(day + features[feature_id].difficulty +
                                      services_in_binary[bin_id] +
                                      cur_devs_in_bin[bin_id],
                                      eng_id);
                ++cur_devs_in_bin[bin_id];
            }
        }
        
        return days_limit - day;
    }
    
    void Solve() {
        Init();
        
        for (int day = 0; day < days_limit; ++day) {
            FreeDevs(day, developer_ready_day, dev_currently_in, cur_devs_in_binary);
            
            do {
                if (incomplete_feature != -1) {
                    auto &remaining = feature_bins_remaining[incomplete_feature];
                    while (!remaining.empty() && !developer_ready_day.empty() && developer_ready_day.begin()->first <= day) {
                        auto [_1, eng_id] = pair(*developer_ready_day.begin());
                        developer_ready_day.erase(developer_ready_day.begin());
                        auto [_2, bin_id] = pair(remaining.front());
                        remaining.pop();
                        Solution[eng_id].push_back(new Implement(feature_id_to_name[incomplete_feature], bin_id));
                        developer_ready_day.emplace(day + features[incomplete_feature].difficulty +
                                                    services_in_binary[bin_id] +
                                                    cur_devs_in_binary[bin_id],
                                                    eng_id);
                        ++cur_devs_in_binary[bin_id];
                    }
                    
                    if (remaining.empty()) {
                        incomplete_feature = -1;
                    }
                }
                
                if (incomplete_feature == -1 && !remaining_features.empty() && developer_ready_day.begin()->first <= day) {
                    vector<pair<uint64_t, int>> feature_bonus;
                    feature_bonus.reserve(remaining_features.size());
                    for (int feature : remaining_features) {
                        auto feature_bins_remaining_vec = MakeFeatureBinsRemainingSortedVec(feature);
                        int time_to_complete = Model(day, feature, vec_to_queue(feature_bins_remaining_vec));
                        uint64_t bonus = static_cast<uint64_t>(max(0, days_limit - day -  time_to_complete)) * features[feature].users;
                        feature_bonus.emplace_back(bonus, feature);
                    }
                    auto [_, feature] = *max_element(feature_bonus.begin(), feature_bonus.end());
                    incomplete_feature = feature;
                    remaining_features.erase(feature);
                    feature_bins_remaining[feature] = vec_to_queue(MakeFeatureBinsRemainingSortedVec(feature));
                }
            } while (incomplete_feature != -1 && developer_ready_day.begin()->first <= day);
        }
    }
};

int main() {
    MySolver solver;

    solver.Input();

    auto start = std::chrono::system_clock::now();
    cerr << "Started solving..." << endl;
    solver.Solve();
    cerr << "Done!" << endl;
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    cerr << "Test time: " << elapsed_seconds.count() << endl;

    cerr << "Outputting" << endl;
    solver.Output();

    cerr << solver.GetScore() << endl;
    return 0;
}
