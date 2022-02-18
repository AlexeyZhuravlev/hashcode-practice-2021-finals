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
    string name;
    int services_num, difficulty, users;
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
    int binary;
    Move(string service, int binary): service(service), binary(binary) {}
    void print() override {
        cout << "move " << service << " " << binary << endl;
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

    void Input() {
        cin >> days_limit >> engineers >> services_num >> initial_binaries_num >> features_num >> new_binary_time;
        services.resize(services_num);
        forn(i, services_num) {
            string name;
            cin >> name >> services[i].binary;
            service_name_to_id[name] = services[i].id = i;
        } 
        features.resize(features_num);
        forn(i, features_num) {
            Feature& feature = features[i];
            cin >> feature.name >> feature.services_num >> feature.difficulty >> feature.users;
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
        return 0;
    }
    
};
