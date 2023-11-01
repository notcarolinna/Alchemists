#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <cmath>
#include "math_Integer.h"

std::map<std::string, std::vector<std::pair<std::string, int>>> dataMap;

math::Integer hidrogenio(std::vector<std::pair<std::string, int>> values) {
    math::Integer hidro = 0;
    for (auto item : values) {
        if (item.first == "hidrogenio") {
            hidro += item.second;
        }
        else {
            hidro += item.second * hidrogenio(dataMap[item.first]);
        }

    }
    return hidro;
}


int main() {
    std::string filename = "casoc240.txt";


    std::ifstream inputFile(filename);

    if (!inputFile) {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
        return 1;
    }

    std::string line;

    auto start = std::chrono::high_resolution_clock::now();

    while (std::getline(inputFile, line)) {
        std::istringstream ss(line);

        std::vector<std::pair<std::string, int>> values;
        std::string quantity;
        std::string element;

        while (1) {
            if (!(ss >> quantity)) {
                break;
            }

            if (quantity != "->") {
                if (!(ss >> element)) {
                    break;
                }
                values.push_back(std::make_pair(element, std::stoi(quantity)));

            }
            else if (!(ss >> quantity >> element)) {
                break;
            }


        }

        auto it = dataMap.find(element);

        if (it == dataMap.end()) {
            dataMap[element] = values;
        }
        else {
            it->second.insert(it->second.end(), values.begin(), values.end());
        }

    }

    std::cout << hidrogenio(dataMap["ouro"]);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    std::cout << "\n\n" << std::fixed << std::setprecision(1) << duration.count() << " segundos." << std::endl;

    return 0;
}
