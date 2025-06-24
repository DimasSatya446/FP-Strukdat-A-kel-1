#ifndef DIJKSTRA_ALGORITHM_H
#define DIJKSTRA_ALGORITHM_H


using namespace std;
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
    vector<string> path;
    double totalCost;
    
    PathResult() : totalCost(0) {}
    PathResult(const vector<string>& path, double cost) 
        : path(path), totalCost(cost) {}
};

class DijkstraAlgorithm {
private:
    const GraphManager& graph;
    
public:
    explicit DijkstraAlgorithm(const GraphManager& graph) : graph(graph) {}
    virtual ~DijkstraAlgorithm() = default;
    
    PathResult findShortestPath(const string& start, const string& end, 
                               const string& mode) const {
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
                double weight = route.getWeightByMode(mode);
                
                if (weight == 0 && mode != "jarak" && mode != "waktu" && mode != "biaya") {
                    cerr << "❌ Mode tidak dikenal!" << endl;
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
    
    PathResult findBestRouteWithPreference(const string& start, const string& end, 
                                          const PreferenceManager& pref) const {
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
    PathResult reconstructPath(const string& start, const string& end,
                              const map<string, string>& previous,
                              const map<string, double>& cost) const {
        vector<string> path;
        string temp = end;
        
        while (temp != start && previous.find(temp) != previous.end()) {
            path.push_back(temp);
            temp = previous.at(temp);
        }

        if (temp == start) {
            path.push_back(start);
            reverse(path.begin(), path.end());
            return PathResult(path, cost.at(end));
        }

        cout << "❌ Rute tidak ditemukan dari " << start << " ke " << end << endl;
        return PathResult();
    }
};

#endif
