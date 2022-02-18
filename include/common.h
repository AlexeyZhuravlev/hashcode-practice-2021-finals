#pragma once

#include <algorithm>
#include <vector>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <cassert>
#include <cmath>
#include <string>
#include <queue>
#include <set>
#include <map>
#include <cstdlib>
#include <unordered_map>

using namespace std;

#define mp make_pair
#define pb push_back
#define fi first
#define se second
#define li long long
#define pii pair<int, int>
#define vi vector<int>

#define forn(i, n) for (int i = 0; i < (int)n; i++)
#define fore(i, b, e) for (int i = (int)b; i <= (int)e; i++)
#define all(x) (x).begin(), (x).end()

struct Service {
    int id, binary;
};

struct Feature {
    int id, services_num, difficulty, users;
    vi services;
};

struct Task {
    virtual void print() = 0;
};

struct Implement: public Task {
    string feature;
    int binary;
    Implement(string feature, int binary): feature(feature), binary(binary) {}
    void print() override {
        cout << "impl " << feature << " " << binary << endl;
    }
};

struct Move: public Task {
    string service;
    int new_binary;
    Move(string service, int new_binary): service(service), new_binary(new_binary) {}
    void print() override {
        cout << "move " << service << " " << new_binary << endl;
    }
};

struct New: public Task {
    void print() override {
        cout << "new" << endl;
    }
};

struct Wait : public Task {
    int w;
    Wait(int w): w(w) {}
    void print() override {
        cout << "wait " << w << endl;
    }
};

struct Context {
    using TSolution = vector<vector<Task*>>;
    TSolution Solution;

    int days_limit, engineers, services_num, initial_binaries_num, features_num, new_binary_time;
    vector<Service> services;
    vector<Feature> features;
    map<string, int> service_name_to_id;
    map<string, int> feature_name_to_id;

    map<int, string> service_id_to_name;
    map<int, string> feature_id_to_name;

    void Input() {
        cin >> days_limit >> engineers >> services_num >> initial_binaries_num >> features_num >> new_binary_time;
        services.resize(services_num);
        forn(i, services_num) {
            string name;
            cin >> name >> services[i].binary;
            service_name_to_id[name] = services[i].id = i;
            service_id_to_name[i] = name;
        } 
        features.resize(features_num);
        forn(i, features_num) {
            Feature& feature = features[i];
            string feature_name;
            cin >> feature_name >> feature.services_num >> feature.difficulty >> feature.users;
            feature_name_to_id[feature_name] = feature.id = i;
            feature_id_to_name[i] = feature_name;

            feature.services.resize(feature.services_num);
            forn(j, feature.services_num) {
                string service_name;
                cin >> service_name;
                feature.services[j] = service_name_to_id[service_name];
            }
        }
    }

    void Output() {
        cout << Solution.size() << endl;
        for (auto tasks: Solution) {
            cout << tasks.size() << endl;
            for (auto task: tasks) {
                task->print();
            }
        }
    }

    uint64_t GetScore() {
        // vector<vi> events(days_limit);
        // vi pointer(engineers);
        // events[0].pb(0);
        // events[0].pb(1);
        // set<int> binaries(initial_binaries_num);
        // vi working_on_binary(engineers, -1);
        // set<int> services_in_binary(initial_binaries_num);
        // vi service_to_binary(services_num);
        // forn(j, services_num) {
        //     service_to_binary[j] = services[j].binary;
        // }

        // forn(day, days_limit) {
        //     if (!events[day].empty()) {
        //         check features, calc score
        //     }

        //     for (int eng: events[day]) {
        //         cerr << "day " << day " eng = " << eng << endl;
        //         if (pointer[eng] == (int)Solution[eng].size()) {
        //             // done all work for engineer
        //             continue;
        //         }
        //         Task* task = Solution[eng][pointer[eng]];

        //         Implement* impl = dynamic_cast<Implement*>(task);
        //         if (impl != nullptr) {
        //             int t = features[impl->feature] + binaries[impl->binary].size();
        //             forn(other, engineers) {
        //                 if (other != eng && working_on_binary[other] == impl->binary) {
        //                     ++t;
        //                 }
        //             }
        //             working_on_binary[eng] = impl->binary;
        //             if (day + t < days_limit) {
        //                 events[day + t].pb(eng);
        //             }
        //             continue;
        //         }
        //         // not working on any binaries 
        //         working_on_binary[eng] = -1;

        //         Move* move = dynamic_cast<Move*>(task);
        //         if (move != nullptr) {
        //             int cur_binary = service_to_binary[move->service];

        //             int t = max(services_in_binary[cur_binary].size(), service_to_binary[move->new_binary].size());
        //             services_in_binary[cur_binary].erase(move->service);
        //             services_in_binary[move->new_binary].insert(move->service);
        //             service_to_binary[move->service] = move->new_binary;

        //             cerr << "move" << endl;
        //         }
        //         New* nnew = dynamic_cast<New*>(task);
        //         if (nnew != nullptr) {
        //             cerr << "new" << endl;
        //         } 
        //         Wait* wait = dynamic_cast<Wait*>(task);
        //         if (wait != nullptr) {
        //             cerr << "wait" << endl;
        //         }
        //     }
        // }
        return 0;
    }
    
};
