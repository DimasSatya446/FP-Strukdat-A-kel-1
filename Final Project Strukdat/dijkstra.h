#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"
#include "preference.h"
#include <queue>
#include <map>
#include <set>
#include <stack>

using namespace std;
struct PathResult {
    vector<string> path;
    double totalCost;
};

PathResult findShortestPath(const Graph& graph, const string& start,
                            const string& end, const string& mode) {
    map<string, double> cost;
    map<string, string> previous;
    set<string> visited;

    auto cmp = [&cost](const string& a, const string& b) {
        return cost[a] > cost[b];
    };
    priority_queue<string, vector<string>, decltype(cmp)> pq(cmp);

    for (const auto& pair : graph.getLocations()) {
        cost[pair.first] = numeric_limits<double>::infinity();
    }

    cost[start] = 0;
    pq.push(start);

    while (!pq.empty()) {
        string current = pq.top();
        pq.pop();

        if (visited.count(current)) continue;
        visited.insert(current);

        for (const auto& route : graph.getRoutesFrom(current)) {
            string neighbor = route.getDestination();
            double weight = 0;

            if (mode == "jarak") weight = route.getDistance();
            else if (mode == "waktu") weight = route.getTime();
            else if (mode == "biaya") weight = route.getCost();
            else {
                cerr << "Mode tidak dikenal!\n";
                return {};
            }

            double newCost = cost[current] + weight;
            if (newCost < cost[neighbor]) {
                cost[neighbor] = newCost;
                previous[neighbor] = current;
                pq.push(neighbor);
            }
        }
    }

    vector<string> path;
    string temp = end;
    while (temp != start && previous.find(temp) != previous.end()) {
        path.push_back(temp);
        temp = previous[temp];
    }

    if (temp == start) {
        path.push_back(start);
        reverse(path.begin(), path.end());
        return { path, cost[end] };
    }

    cout << "Rute tidak ditemukan dari " << start << " ke " << end << ".\n";
    return { {}, 0 };
}

PathResult findBestRouteWithPreference(const Graph& graph, const string& start,
                                       const string& end, const Preference& pref) {
    map<string, double> cost;
    map<string, string> previous;
    set<string> visited;

    auto cmp = [&cost](const string& a, const string& b) {
        return cost[a] > cost[b];
    };
    priority_queue<string, vector<string>, decltype(cmp)> pq(cmp);

    for (const auto& pair : graph.getLocations()) {
        cost[pair.first] = numeric_limits<double>::infinity();
    }

    cost[start] = 0;
    pq.push(start);

    while (!pq.empty()) {
        string current = pq.top();
        pq.pop();

        if (visited.count(current)) continue;
        visited.insert(current);

        for (const auto& route : graph.getRoutesFrom(current)) {
            string neighbor = route.getDestination();

            double skor = pref.weightWaktu * route.getTime()
                        + pref.weightBiaya * route.getCost()
                        + pref.weightJarak * route.getDistance();

            double newCost = cost[current] + skor;
            if (newCost < cost[neighbor]) {
                cost[neighbor] = newCost;
                previous[neighbor] = current;
                pq.push(neighbor);
            }
        }
    }

    vector<string> path;
    string temp = end;
    while (temp != start && previous.find(temp) != previous.end()) {
        path.push_back(temp);
        temp = previous[temp];
    }

    if (temp == start) {
        path.push_back(start);
        reverse(path.begin(), path.end());
        return { path, cost[end] };
    }

    cout << "Rute tidak ditemukan dari " << start << " ke " << end << ".\n";
    return { {}, 0 };
}

#endif
