//
// Created by Michał Zmyślony on 22/09/2021.
//

#include "PerimeterChecking.h"
#include "ValarrayOperations.h"


std::valarray<double> getRepulsion(std::vector<std::vector<int>> &filledTable,
                                std::vector<std::valarray<int>> &checkedArea,
                                std::valarray<int> &startPositions,
                                std::valarray<int> &sizes, double repulsionCoefficient) {

    std::valarray<double> attraction = {0, 0};
    int numberOfEmptySpots = 0;
    for (auto &direction: checkedArea) {
        std::valarray<int> positionsNew = direction + startPositions;
        if (0 <= positionsNew[0] && positionsNew[0] < sizes[0] && 0 <= positionsNew[1] && positionsNew[1] < sizes[1]) {
            if (filledTable[positionsNew[0]][positionsNew[1]] == 0) {
                attraction += itodArray(direction);
                numberOfEmptySpots++;
            }
        }
    }

    return -repulsionCoefficient * attraction / numberOfEmptySpots;
}

bool isPerimeterFree(std::vector<std::vector<int>> &filledTable, std::vector<std::vector<int>> &shapeTable,
                     std::vector<std::valarray<int>> &perimeterList,
                     std::valarray<int> &startPositions, std::valarray<int> &sizes) {
    if (shapeTable[startPositions[0]][startPositions[1]] == 0) {
        return false;
    }
    for (auto &perimeter: perimeterList) {
        std::valarray<int> positionsNew = perimeter + startPositions;
        if (0 <= positionsNew[0] && positionsNew[0] < sizes[0] && 0 <= positionsNew[1] && positionsNew[1] < sizes[1]) {
            if (filledTable[positionsNew[0]][positionsNew[1]] > 0) {
                return false;
            }
        }
    }
    return true;
}


bool
isOnEdge(const std::vector<std::vector<int>> &shapeTable, const std::valarray<int> &startPositions,
         const std::valarray<int> &sizes) {
    std::vector<std::valarray<int>> listOfNearestNeighbours = {{-1, 0},
                                                               {-1, 1},
                                                               {0,  1},
                                                               {1,  1},
                                                               {1,  0},
                                                               {1,  -1},
                                                               {0,  -1},
                                                               {-1, -1}};

    if (shapeTable[startPositions[0]][startPositions[1]] == 0) {
        return false;
    }

    for (auto &neighbour: listOfNearestNeighbours) {
        std::valarray<int> positionsNew = neighbour + startPositions;
        if (0 <= positionsNew[0] && positionsNew[0] < sizes[0] && 0 <= positionsNew[1] && positionsNew[1] < sizes[1]) {
            if (shapeTable[positionsNew[0]][positionsNew[1]] == 0) {
                return true;
            }
        }
    }
    return false;
}


std::vector<std::valarray<int>>
findUnsortedPerimeters(const std::vector<std::vector<int>> &shapeMatrix, const std::valarray<int> &sizes) {
    std::vector<std::valarray<int>> unsortedPerimeters;
    for (int i = 0; i < sizes[0]; i++) {
        for (int j = 0; j < sizes[1]; j++) {
            std::valarray<int> currentPosition = {i, j};
            if (isOnEdge(shapeMatrix, currentPosition, sizes)) {
                unsortedPerimeters.push_back(currentPosition);
            }
        }
    }
    return unsortedPerimeters;
}


void removeElement(std::vector<std::valarray<int>> &array, int index) {
    array.erase(array.begin() + index);
}


std::valarray<int> findClosestNeighbour(std::vector<std::valarray<int>> &array, std::valarray<int> &element) {
    std::valarray<int> closestElement;
    double closestDistance = DBL_MAX;

    int iMin = 0;
    for (int i = 0; i < array.size(); i++) {
        double distance = norm(array[i] - element);
        if (distance < closestDistance) {
            closestElement = array[i];
            iMin = i;
            closestDistance = distance;
        }
    }
    removeElement(array, iMin);
    return closestElement;
}


std::vector<std::valarray<int>> sortPerimeters(std::vector<std::valarray<int>> &unsortedPerimeters, int startingIndex) {
    std::valarray<int> currentElement = unsortedPerimeters[startingIndex];
    removeElement(unsortedPerimeters, startingIndex);

    std::vector<std::valarray<int>> sortedPerimeters;
    sortedPerimeters.push_back(currentElement);
    while (!unsortedPerimeters.empty()) {
        currentElement = findClosestNeighbour(unsortedPerimeters, currentElement);
        sortedPerimeters.push_back(currentElement);
    }
    return sortedPerimeters;
}


std::vector<std::valarray<int>>
findSortedPerimeters(const std::vector<std::vector<int>> &shapeMatrix, const std::valarray<int> &sizes) {
    std::vector<std::valarray<int>> unsortedPerimeters = findUnsortedPerimeters(shapeMatrix, sizes);
    std::vector<std::valarray<int>> sortedPerimeters = sortPerimeters(unsortedPerimeters, 0);
    return sortedPerimeters;
}