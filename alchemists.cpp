#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <cmath>
#include "math_Integer.h"

/*
Lógica do algoritmo:
Para saber a quantidade de hidrogênios para formar o ouro, precisa-se da quantidade de hidrogênio de cada filho do ouro somado
multiplicado pela quantidade do elemento.
*/

std::map<std::string, std::vector<std::pair<std::string, int>>> dados;
std::map<std::string, math::Integer> resultados;
math::Integer calculoHidrogenio(const std::string& elemento) {

	if (resultados.find(elemento) != resultados.end()) {
		return resultados[elemento];
	}

	math::Integer total = 0;

	for (auto item : dados[elemento]) {

		if (item.first == "hidrogenio") {
			total += item.second;
		}
		else {
			total += item.second * calculoHidrogenio(item.first);
		}
	}

	resultados[elemento] = total;
	return total;
}

int main() {

	std::ifstream file("./casoc360.txt");
	std::string line;

	if (!file) {
		std::cerr << "Erro ao abrir o arquivo." << std::endl;
		return 1;
	}

	auto start = std::chrono::high_resolution_clock::now();

	while (std::getline(file, line)) {

		std::istringstream ss(line);

		std::vector<std::pair<std::string, int>> custos;
		std::string quantidade;
		std::string elemento;

		while (1) {

			if (!(ss >> quantidade)) {
				break;
			}

			if (quantidade != "->") {
				if (!(ss >> elemento)) {
					break;
				}
				custos.push_back(std::make_pair(elemento, std::stoi(quantidade)));
			}

			else if (!(ss >> quantidade >> elemento)) {
				break;
			}
		}

		auto it = dados.find(elemento);

		if (it == dados.end()) {
			dados[elemento] = custos;
		}
		else {
			it->second.insert(it->second.end(), custos.begin(), custos.end());
		}
	}

	math::Integer quantHidro = calculoHidrogenio("ouro");

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	std::cout << quantHidro << "\n\n" << std::fixed << std::setprecision(1) << duration.count() << " nanossegundos." << std::endl;

	return 0;
}
