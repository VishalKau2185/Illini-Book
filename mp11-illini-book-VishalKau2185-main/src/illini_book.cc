#include "illini_book.hpp"
#include "utilities.hpp"
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <queue>

IlliniBook::IlliniBook(const std::string &people_fpath, const std::string &relations_fpath) {
    std::ifstream people_stream(people_fpath);
    std::ifstream rel_stream(relations_fpath);
    std::string line;
    while (people_stream.good()) {
        people_stream >> line;
        graph_.insert({std::stoi(line), std::list<std::pair<int, std::string>>()});
    }
    std::vector<std::string> substrs;
    while (rel_stream.good()) {
        rel_stream >> line;
        substrs = utilities::Split(line, ',');
        graph_.at(std::stoi(substrs.at(0))).push_back(std::pair<int, std::string>(std::stoi(substrs.at(1)), substrs.at(2)));
        graph_.at(std::stoi(substrs.at(1))).push_back(std::pair<int, std::string>(std::stoi(substrs.at(0)), substrs.at(2)));
    }
}

bool IlliniBook::AreRelated(int uin_1, int uin_2) const {
    std::set<int> v;
    std::queue<int> queue;
    queue.push(uin_1);
    v.insert(uin_1);
    while (!queue.empty()) {
        const auto& current = queue.front();
        queue.pop();
        if (current == uin_2) {
            return true;
        }
        const auto& a_list = graph_.find(current)->second;
        for (const auto& n : a_list) {
            if (v.count(n.first) == 0u) {
                queue.push(n.first);
                v.insert(n.first);
            }
        }
    }
    return false;
}

bool IlliniBook::AreRelated(int uin_1, int uin_2, const std::string &relationship) const {
    std::set<int> v;
    std::queue<int> queue;
    queue.push(uin_1);
    v.insert(uin_1);
    while (!queue.empty()) {
        const auto& current = queue.front();
        queue.pop();
        if (current == uin_2) {
            return true;
        }
        const auto& a_list = graph_.find(current)->second;
        for (const auto& n : a_list) {
            const auto& relation2 = n.second;
            if (relation2 == relationship) {
                if (v.count(n.first) == 0u) {
                    queue.push(n.first);
                    v.insert(n.first);
                }
            }
        }
    }
    return false;
}

int IlliniBook::GetRelated(int uin_1, int uin_2) const {
    std::map<int, int> d;
    std::queue<int> queue;
    std::vector<int> results;
    results.push_back(-1);
    queue.push(uin_1);
    d.insert({uin_1, 0});
    while (!queue.empty()) {
        const auto& current = queue.front();
        queue.pop();
        const auto& a_list = graph_.find(current)->second;
        for (const auto& n : a_list) {
            if (n.first == uin_2) {
                results.push_back(d[current] + 1);
                if (current != uin_1) {
                    queue.push(uin_1);
                }
            } else if (d.count(n.first) == 0u) {
                queue.push(n.first);
                d.insert({n.first, d[current] + 1});
            }
        }
    }
    unsigned int min = results.at(0);
    for (unsigned int i = 1; i < results.size(); ++i) {
        if ((unsigned int) results.at(i) < min) {
            min = results.at(i);
        }
    }
    return (int) min;
}

int IlliniBook::GetRelated(int uin_1, int uin_2, const std::string &relationship) const {
    std::map<int, int> d;
    std::queue<int> queue;
    std::vector<int> results;
    results.push_back(-1);
    queue.push(uin_1);
    d.insert({uin_1, 0});
    while (!queue.empty()) {
        const auto& current = queue.front();
        queue.pop();
        const auto& a_list = graph_.find(current)->second;
        for (const auto& n : a_list) {
            const auto& relation = n.second;
            if (relation == relationship) {
                if (n.first == uin_2) {
                    results.push_back(d[current] + 1);
                    if (current != uin_1) {
                        queue.push(uin_1);
                    }
                }
                if (d.count(n.first) == 0u) {
                    queue.push(n.first);
                    d.insert({n.first, d[current] + 1});
                }
            }
        }
    }
    unsigned int min = results.at(0);
    for (unsigned int i = 1; i < results.size(); ++i) {
        if ((unsigned int) results.at(i) < min) {
            min = results.at(i);
        }
    }
    return (int) min;
}

std::vector<int> IlliniBook::GetSteps(int uin, int n) const {
    std::map<int, int> d;
    std::queue<int> queue;
    std::vector<int> results;
    queue.push(uin);
    d.insert({uin, 0});
    while (!queue.empty()) {
        const auto& current = queue.front();
        queue.pop();
        if (d[current] == n) {
            results.push_back(current);
        }
        const auto& a_list = graph_.find(current)->second;
        for (const auto& n : a_list) {
            if (d.count(n.first) == 0u) {
                queue.push(n.first);
                d.insert({n.first, d[current] + 1});
            }
        }
    }
    return results;
}

size_t IlliniBook::CountGroups() const {
    std::set<int> v;
    size_t c = 0;
    for (const auto& vertex : GetVertices()) {
        if (v.count(vertex) == 0u) {
            std::queue<int> queue;
            queue.push(vertex);
            v.insert(vertex);
            while (!queue.empty()) {
                const auto& current = queue.front();
                queue.pop();
                const auto& a_list = graph_.find(current)->second;
                for (const auto& n : a_list) {
                    if (v.count(n.first) == 0u) {
                        queue.push(n.first);
                        v.insert(n.first);
                    }
                }
            }
            c++;
        }
    }
    return c;
}

size_t IlliniBook::CountGroups(const std::string &relationship) const {
    std::set<int> v;
    size_t c = 0;
    for (const auto& vertex : GetVertices()) {
        if (v.count(vertex) == 0u) {
            std::queue<int> queue;
            queue.push(vertex);
            v.insert(vertex);
            while (!queue.empty()) {
                const auto& current = queue.front();
                queue.pop();
                const auto& a_list = graph_.find(current)->second;
                for (const auto& n : a_list) {
                    const auto& relation = n.second;
                    if (relation == relationship) {
                        if (v.count(n.first) == 0u) {
                            queue.push(n.first);
                            v.insert(n.first);
                        }
                    }
                }
            }
            c++;
        }
    }
    return c;
}
size_t IlliniBook::CountGroups(const std::vector<std::string> &relationships) const {
    std::set<int> v;
    size_t c = 0;
    for (const auto& vertex : GetVertices()) {
        if (v.count(vertex) == 0u) {
            std::queue<int> queue;
            queue.push(vertex);
            v.insert(vertex);
            while (!queue.empty()) {
                const auto& current = queue.front();
                queue.pop();
                const auto& a_list = graph_.find(current)->second;
                for (const auto& n : a_list) {
                    std::string relation = n.second;
                    if (Con(relation, relationships)) {
                        if (v.count(n.first) == 0u) {
                            queue.push(n.first);
                            v.insert(n.first);
                        }
                    }
                }
            }
            c++;
        }
    }
    return c;
}


// helpers

std::list<int> IlliniBook::GetVertices() const{
    std::list<int> verticies;
    for (auto it = graph_.begin(); it != graph_.end(); it++) {
        verticies.push_back(it->first);
    }
    return verticies;
}

bool IlliniBook::Con(std::string &rel, std::vector<std::string> relationships) const{
    for (unsigned int i = 0; i < relationships.size(); ++i) {
        if (relationships.at(i) == rel) {
            return true;
        }
    }
    return false;
}
