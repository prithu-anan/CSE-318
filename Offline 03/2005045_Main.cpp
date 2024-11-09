#include <vector>
#include "2005045_TSP.cpp"
using namespace std;

// Main function
int main() {
    vector<City> cities = {
        {0, 0.0, 0.0}, {1, 1.0, 1.0}, {2, 2.0, 2.0} // Example setup
    };
    TSPSolver solver(cities);
    solver.solve();
    return 0;
}