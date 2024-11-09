#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <random>
#include <ctime>

using namespace std;

// City struct to hold the coordinates of each city
struct City {
    int id;
    double x, y;
};

// Utility function to calculate Euclidean distance
double euclideanDistance(const City &a, const City &b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// Constructive Heuristics
class ConstructiveHeuristic {
public:
    // Nearest Neighbor Heuristic (NNH)
    vector<int> nearestNeighbor(const vector<City> &cities) {
        vector<int> tour;
        vector<bool> visited(cities.size(), false);

        int currentCity = 0; // Start from the first city (arbitrary choice)
        tour.push_back(currentCity);
        visited[currentCity] = true;

        for (size_t i = 1; i < cities.size(); i++) {
            double minDistance = numeric_limits<double>::max();
            int nextCity = -1;

            for (size_t j = 0; j < cities.size(); j++) {
                if (!visited[j]) {
                    double distance = euclideanDistance(cities[currentCity], cities[j]);
                    if (distance < minDistance) {
                        minDistance = distance;
                        nextCity = j;
                    }
                }
            }

            tour.push_back(nextCity);
            visited[nextCity] = true;
            currentCity = nextCity;
        }

        return tour;
    }

    // Cheapest Insertion Heuristic
    vector<int> cheapestInsertion(const vector<City> &cities) {
        vector<int> tour = {0, 1};  // Start with an initial pair of cities
        vector<bool> visited(cities.size(), false);
        visited[0] = visited[1] = true;

        while (tour.size() < cities.size()) {
            double minIncrease = numeric_limits<double>::max();
            int insertPosition = -1;
            int cityToInsert = -1;

            for (int i = 0; i < cities.size(); i++) {
                if (visited[i]) continue;
                for (int j = 0; j < tour.size(); j++) {
                    int prevCity = tour[j];
                    int nextCity = tour[(j + 1) % tour.size()];
                    double increase = euclideanDistance(cities[prevCity], cities[i]) +
                                      euclideanDistance(cities[i], cities[nextCity]) -
                                      euclideanDistance(cities[prevCity], cities[nextCity]);

                    if (increase < minIncrease) {
                        minIncrease = increase;
                        cityToInsert = i;
                        insertPosition = j + 1;
                    }
                }
            }

            tour.insert(tour.begin() + insertPosition, cityToInsert);
            visited[cityToInsert] = true;
        }

        return tour;
    }

    // Random Insertion Heuristic
    vector<int> randomInsertion(const vector<City> &cities) {
        vector<int> tour = {0, 1}; // Start with an initial pair of cities
        vector<bool> visited(cities.size(), false);
        visited[0] = visited[1] = true;

        random_device rd;
        mt19937 gen(rd());

        while (tour.size() < cities.size()) {
            int cityToInsert;
            do {
                cityToInsert = gen() % cities.size();
            } while (visited[cityToInsert]);

            int position = gen() % tour.size();
            tour.insert(tour.begin() + position, cityToInsert);
            visited[cityToInsert] = true;
        }

        return tour;
    }
};

// Perturbative Heuristics
class PerturbativeHeuristic {
public:
    // 2-opt Heuristic
    void twoOpt(vector<int> &tour, const vector<City> &cities) {
        bool improvement = true;
        while (improvement) {
            improvement = false;
            for (size_t i = 1; i < tour.size() - 1; i++) {
                for (size_t j = i + 1; j < tour.size(); j++) {
                    if (j - i == 1) continue;
                    double delta = -euclideanDistance(cities[tour[i - 1]], cities[tour[i]]) 
                                   - euclideanDistance(cities[tour[j]], cities[tour[(j + 1) % tour.size()]])
                                   + euclideanDistance(cities[tour[i - 1]], cities[tour[j]]) 
                                   + euclideanDistance(cities[tour[i]], cities[tour[(j + 1) % tour.size()]]);
                    if (delta < 0) {
                        reverse(tour.begin() + i, tour.begin() + j + 1);
                        improvement = true;
                    }
                }
            }
        }
    }

    // 3-opt Heuristic
    void threeOpt(vector<int> &tour, const vector<City> &cities) {
        // Implementing full 3-opt is complex; this uses 3-opt moves on small segments
        bool improvement = true;
        while (improvement) {
            improvement = false;
            for (size_t i = 0; i < tour.size() - 2; i++) {
                for (size_t j = i + 2; j < tour.size() - 1; j++) {
                    for (size_t k = j + 2; k < tour.size(); k++) {
                        double delta = -euclideanDistance(cities[tour[i]], cities[tour[i + 1]])
                                       - euclideanDistance(cities[tour[j]], cities[tour[j + 1]])
                                       - euclideanDistance(cities[tour[k]], cities[tour[(k + 1) % tour.size()]])
                                       + euclideanDistance(cities[tour[i]], cities[tour[j + 1]])
                                       + euclideanDistance(cities[tour[j]], cities[tour[(k + 1) % tour.size()]])
                                       + euclideanDistance(cities[tour[k]], cities[tour[i + 1]]);
                        if (delta < 0) {
                            reverse(tour.begin() + i + 1, tour.begin() + j + 1);
                            reverse(tour.begin() + j + 1, tour.begin() + k + 1);
                            improvement = true;
                        }
                    }
                }
            }
        }
    }

    // Node Swap Heuristic
    void nodeSwap(vector<int> &tour, const vector<City> &cities) {
        for (size_t i = 0; i < tour.size() - 1; i++) {
            for (size_t j = i + 1; j < tour.size(); j++) {
                swap(tour[i], tour[j]);
                if (calculateTourLength(tour, cities) < calculateTourLength(tour, cities)) {
                    continue;
                }
                swap(tour[i], tour[j]); // Swap back if no improvement
            }
        }
    }

    // Utility function to calculate tour length
    double calculateTourLength(const vector<int> &tour, const vector<City> &cities) {
        double length = 0;
        for (size_t i = 0; i < tour.size() - 1; i++) {
            length += euclideanDistance(cities[tour[i]], cities[tour[i + 1]]);
        }
        length += euclideanDistance(cities[tour.back()], cities[tour[0]]);
        return length;
    }
};

// Main TSP Solver
class TSPSolver {
    vector<City> cities;
    ConstructiveHeuristic constructive;
    PerturbativeHeuristic perturbative;

public:
    TSPSolver(const vector<City> &inputCities) : cities(inputCities) {}

    void solve() {
        vector<int> initialTour = constructive.nearestNeighbor(cities);
        perturbative.twoOpt(initialTour, cities);
        // Add additional combinations and output results for each dataset
    }
};
