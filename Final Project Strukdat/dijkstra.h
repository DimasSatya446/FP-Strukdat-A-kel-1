#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Graph.h"
#include "Preference.h"
#include <queue>
#include <map>
#include <set>
#include <stack>

struct PathResult {
    std::vector<std::string> path;
    double totalCost;
};

PathResult findShortestPath(const Graph& graph, const std::string& start,
                            const std::string& end, const std::string& mode) {
    std::map<std::string, double> cost;
    std::map<std::string, std::string> previous;
    std::set<std::string> visited;

    auto cmp = [&cost](const std::string& a, const std::string& b) {
        return cost[a] > cost[b];
    };
    std::priority_queue<std::string, std::vector<std::string>, decltype(cmp)> pq(cmp);

    for (const auto& pair : graph.getLocations()) {
        cost[pair.first] = std::numeric_limits<double>::infinity();
    }

    cost[start] = 0;
    pq.push(start);

    while (!pq.empty()) {
        std::string current = pq.top();
        pq.pop();

        if (visited.count(current)) continue;
        visited.insert(current);

        for (const auto& route : graph.getRoutesFrom(current)) {
            std::string neighbor = route.getDestination();
            double weight = 0;

            if (mode == "jarak") weight = route.getDistance();
            else if (mode == "waktu") weight = route.getTime();
            else if (mode == "biaya") weight = route.getCost();
            else {
                std::cerr << "Mode tidak dikenal!\n";
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

    std::vector<std::string> path;
    std::string temp = end;
    while (temp != start && previous.find(temp) != previous.end()) {
        path.push_back(temp);
        temp = previous[temp];
    }

    if (temp == start) {
        path.push_back(start);
        std::reverse(path.begin(), path.end());
        return { path, cost[end] };
    }

    std::cout << "Rute tidak ditemukan dari " << start << " ke " << end << ".\n";
    return { {}, 0 };
}

PathResult findBestRouteWithPreference(const Graph& graph, const std::string& start,
                                       const std::string& end, const Preference& pref) {
    std::map<std::string, double> cost;
    std::map<std::string, std::string> previous;
    std::set<std::string> visited;

    auto cmp = [&cost](const std::string& a, const std::string& b) {
        return cost[a] > cost[b];
    };
    std::priority_queue<std::string, std::vector<std::string>, decltype(cmp)> pq(cmp);

    for (const auto& pair : graph.getLocations()) {
        cost[pair.first] = std::numeric_limits<double>::infinity();
    }

    cost[start] = 0;
    pq.push(start);

    while (!pq.empty()) {
        std::string current = pq.top();
        pq.pop();

        if (visited.count(current)) continue;
        visited.insert(current);

        for (const auto& route : graph.getRoutesFrom(current)) {
            std::string neighbor = route.getDestination();

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

    std::vector<std::string> path;
    std::string temp = end;
    while (temp != start && previous.find(temp) != previous.end()) {
        path.push_back(temp);
        temp = previous[temp];
    }

    if (temp == start) {
        path.push_back(start);
        std::reverse(path.begin(), path.end());
        return { path, cost[end] };
    }

    std::cout << "Rute tidak ditemukan dari " << start << " ke " << end << ".\n";
    return { {}, 0 };
}

#endif
