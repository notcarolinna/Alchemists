#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>

/*
 Ideia inicial:
 1) Percorrer o arquivo de trás para frente;
 2) Fazer um mapa dos elementos e seus "custos". Ex: Cr{(H,2))}, Au{(Sn,1),{(Zn,6)}
 3) Ir nos itens do vetor, pegando as keys do mapa até chegar no hidrogênio
*/

int main() {
	std::map<std::string, std::vector<std::pair<std::string, int>>> elementos;
	std::ifstream file("./casoteste.txt");
	std::vector<std::string> lines;
	std::string line;

	while (std::getline(file, line)) {

		size_t found = line.find("->");
		while (found != std::string::npos) {
			line.replace(found, 2, "");
			found = line.find("->");
		}

		found = line.find("  ");
		while (found != std::string::npos) {
			line.replace(found, 2, " ");
			found = line.find("  ");
		}

		lines.push_back(line);
	}

	for (int i = lines.size() - 1; i >= 0; i--) {
		std::cout << lines[i] << std::endl;
	}

}
