#ifndef DIJKSTRA_ALGORITHM_H
#define DIJKSTRA_ALGORITHM_H

#include "GraphManager.h"
#include "PreferenceManager.h"
#include <queue>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>

struct PathResult {
    std::vector<std::string> path;
    double totalCost;
    
    PathResult() : totalCost(0) {}
    PathResult(const std::vector<std::string>& path, double cost) 
        : path(path), totalCost(cost) {}
};

class DijkstraAlgorithm {
private:
    const GraphManager& graph;
    
public:
    explicit DijkstraAlgorithm(const GraphManager& graph) : graph(graph) {}
    virtual ~DijkstraAlgorithm() = default;
    
    PathResult findShortestPath(const std::string& start, const std::string& end, 
                               const std::string& mode) const {
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
                double weight = route.getWeightByMode(mode);
                
                if (weight == 0 && mode != "jarak" && mode != "waktu" && mode != "biaya") {
                    std::cerr << "❌ Mode tidak dikenal!" << std::endl;
                    return PathResult();
                }

                double newCost = cost[current] + weight;
                if (newCost < cost[neighbor]) {
                    cost[neighbor] = newCost;
                    previous[neighbor] = current;
                    pq.push(neighbor);
                }
            }
        }

        return reconstructPath(start, end, previous, cost);
    }
    
    PathResult findBestRouteWithPreference(const std::string& start, const std::string& end, 
                                          const PreferenceManager& pref) const {
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

                double skor = pref.calculateScore(route.getTime(), route.getCost(), route.getDistance());

                double newCost = cost[current] + skor;
                if (newCost < cost[neighbor]) {
                    cost[neighbor] = newCost;
                    previous[neighbor] = current;
                    pq.push(neighbor);
                }
            }
        }

        return reconstructPath(start, end, previous, cost);
    }
    
private:
    PathResult reconstructPath(const std::string& start, const std::string& end,
                              const std::map<std::string, std::string>& previous,
                              const std::map<std::string, double>& cost) const {
        std::vector<std::string> path;
        std::string temp = end;
        
        while (temp != start && previous.find(temp) != previous.end()) {
            path.push_back(temp);
            temp = previous.at(temp);
        }

        if (temp == start) {
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return PathResult(path, cost.at(end));
        }

        std::cout << "❌ Rute tidak ditemukan dari " << start << " ke " << end << std::endl;
        return PathResult();
    }
};

#endif
