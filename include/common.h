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
    int days;
    Wait(int days): days(days) {}
    void print() override {
        cout << "wait " << days << endl;
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
        li score = 0;

        vector<vi> events(days_limit);
        vi pointer(engineers);
        forn(j, Solution.size()) {
            events[0].pb(j);
        }
        vector<vi> working_on_binary(engineers, vi(days_limit, -1));
        vector<set<int>> services_in_binary(initial_binaries_num);
        vi service_to_binary(services_num);
        forn(j, services_num) {
            service_to_binary[j] = services[j].binary;

            services_in_binary[services[j].binary].insert(j);
        }

        vector<set<int>> remaining_services_for_feature(features_num);
        forn(i, features_num) {
            for (int s: features[i].services) {
                remaining_services_for_feature[i].insert(s);
            }
        }

        forn(day, days_limit) {
            // cerr << remaining_services_for_feature[12].size() << endl;
            for (int eng: events[day]) {
                // cerr << "day " << day << " eng = " << eng <<  " pointer for him = " << pointer[eng] << endl;
                if (pointer[eng]) {
                    // check if we were implementing last time
                    Implement* impl = dynamic_cast<Implement*>(Solution[eng][pointer[eng] - 1]);
                    if (impl != nullptr) {
                        int feature = feature_name_to_id[impl->feature];
                        assert(feature >= 0 && feature < features_num && "incorrect feature id");
                        // assert(remaining_services_for_feature.size() == features_num);
                        if (!remaining_services_for_feature[feature].empty()) {
                            assert(impl->binary >= 0 && impl->binary < (int)services_in_binary.size() && "invalid impl binary");
                            for (int service: services_in_binary[impl->binary]) {
                                assert(service >= 0 && service < services_num && "incorrect service id");
                                // cerr << "before erase" << endl;
                                // assert(feature < remaining_services_for_feature.size());
                                // cerr << "feature = " << feature << " " << remaining_services_for_feature[feature].size() << endl;
                                if (remaining_services_for_feature[feature].count(service)) {
                                    remaining_services_for_feature[feature].erase(service);
                                }
                                // cerr << "done with erase" << endl;
                            }
                            if (remaining_services_for_feature[feature].empty()) {
                                // cerr << "feature " << feature << " done on day " << day << endl;
                                score += (li)(days_limit - day) * features[feature].users;
                            }
                        }
                        // cerr << "done with this block" << endl;
                    }
                }
                if (pointer[eng] == (int)Solution[eng].size()) {
                    // cerr << "done work for eng " << eng << endl;
                    // done all work for engineer
                    continue;
                }
                Task* task = Solution[eng][pointer[eng]];
                ++pointer[eng];

                Implement* impl = dynamic_cast<Implement*>(task);
                if (impl != nullptr) {
                    int feature = feature_name_to_id[impl->feature];
                    int t = features[feature].difficulty + services_in_binary[impl->binary].size();
                    forn(other, engineers) {
                        if (other != eng && working_on_binary[other][day] == impl->binary) {
                            ++t;
                        }
                    }
                    // cerr << "impl t = " << t << endl;
                    fore(d, day, min(days_limit, day + t) - 1) {
                        working_on_binary[eng][d] = impl->binary;
                    }
                    if (day + t < days_limit) {
                        events[day + t].pb(eng);
                    }
                    continue;
                }

                Move* move = dynamic_cast<Move*>(task);
                if (move != nullptr) {
                    int service = service_name_to_id[move->service];
                    int cur_binary = service_to_binary[service];
                    assert(cur_binary != move->new_binary && "move should be to a different binary");
                    forn(other, engineers) {
                        assert(working_on_binary[other][day] != cur_binary && "move: source binary is being worked on");
                        assert(working_on_binary[other][day] != move->new_binary && "move: dest binary is being worked on");
                    }

                    int t = max(services_in_binary[cur_binary].size(), services_in_binary[move->new_binary].size());

                    services_in_binary[cur_binary].erase(service);
                    services_in_binary[move->new_binary].insert(service);
                    service_to_binary[service] = move->new_binary;

                    // cerr << "move t = " << t << endl;
                    fore(d, day, min(days_limit, day + t) - 1) {
                        working_on_binary[eng][d] = move->new_binary;
                    }
                    if (day + t < days_limit) {
                        events[day + t].pb(eng);
                    }

                    // cerr << "move" << endl;
                }
                New* nnew = dynamic_cast<New*>(task);
                if (nnew != nullptr) {
                    services_in_binary.resize(services_in_binary.size() + 1);

                    fore(d, day, min(days_limit, day + new_binary_time) - 1) {
                        working_on_binary[eng][d] = services_in_binary.size() - 1;
                    }

                    if (day + new_binary_time < days_limit) {
                        events[day + new_binary_time].pb(eng);
                    }
                    // cerr << "new" << endl;
                } 
                Wait* wait = dynamic_cast<Wait*>(task);
                if (wait != nullptr) {
                    // cerr << "wait" << endl;
                    if (day + wait->days) {
                        events[day + wait->days].pb(eng);
                    }
                }
            }
        }
        return score;
    }
    
};
