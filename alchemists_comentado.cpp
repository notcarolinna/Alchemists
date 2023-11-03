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

// Para cada elemento, tem-se um vetor de pares contendo elementos e quantidades que o compõe, por exemplo:
// '8 cadmio 6 netunio -> 1 paladio', teria algo como paladio{(cadmio,8),(netunio,6)}

std::map<std::string, std::vector<std::pair<std::string, int>>> dados;
std::map<std::string, math::Integer> resultados; // Armazena os resultados usando uma instância da classe Integer do header math_Integer

math::Integer calculoHidrogenio(const std::string& elemento) {

    // Verifica se o resultado do elemento já foi calculado.
    // Esse trecho foi o que melhorou absurdamente a eficiência do código, já que evita recálculos.
    // A função find procura a chave do elemento no mapa resultados, se ela for encontrada, significa
    // que o resultado já foi calculado e pode ser retornado imediatamente. 

    if (resultados.find(elemento) != resultados.end()) {
        return resultados[elemento];
    }

    math::Integer total = 0; // Acumula o resultado do cálculo

    for (auto item : dados[elemento]) { // Percorre os elementos do vetor associado ao elemento no mapa dados
        // o auto item representa os pares de elementos e suas quantidades, que está em dados[elemento].
        // item.first representa o elemento e o item.second representa a quantidade

        if (item.first == "hidrogenio") { // Se o elemento atual for o hidrogênio
            total += item.second; 
        }
        else { // Se não for hidrogênio, é um outro elemento
            total += item.second * calculoHidrogenio(item.first); // A função é chamada recursivamente para calcular a quantidade
            // de hidrogenio necessária para fazer esse outro elemento químico que não é o hidrogenio. 

            /*    
            Exemplo: '6 zinco 1 selenio -> 1 ouro'
            O ouro precisa de 6 zincos e 1 selenio para ser feito, então chama-se a função recursiva pro zinco e pro selenio, multiplicando
            a função do zinco por 6 e a do selenio por 1. No entanto, o zinco tem o seguinte vetor 'zinco{(cromo,4),(praseoodimio,3),(promecio,4),(paladio,9)}',
            então chama a função recursiva para cada um dos elementos até chegar no hidrogênio. O mesmo processo se repete para todos os elementos e no final 
            adiciona-se o resultado à variável total, possuindo o tipo integer que é uma instância da classe Integer do arquivo math_Integer, sendo responsável
            por armazenar inteiros em formato de sinal-magnitude.         
            Contudo, o objetivo da função não é calcular quantos outro elementos tem no grafo e sim a quantidade de hidrogenios :)
            */
           
        }
    }

    resultados[elemento] = total; // adiciona o total ao vetor de resultados do elemento pra não precisar ficar recalculando
    return total; // retorna total, que representa o número de hidrogênios necessários para fazer um ouro
}

int main() {

    std::ifstream file("./casoc360.txt");
    std::string line;

    if (!file) {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
        return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();

    while (std::getline(file, line)) { // While externo que percorre cada linha do arquivo lido

        std::istringstream ss(line); // Processamento de palavra por palavra

        std::vector<std::pair<std::string, int>> custos; // Armazena os pares de elementos e suas quantidades
        std::string quantidade; // Representa o peso das arestas
        std::string elemento; // Representa o elemento químico :O

        while (1) { // While interno que processa as palavras da linha

            // Exemplo de linha do arquivo: 10 hidrogenio -> 1 iterbio

            if (!(ss >> quantidade)) { // Armazena o peso da aresta (quantidade)
                break; // Se não conseguir, dá break
            }

            if (quantidade != "->") {  // Se a string lida não for a seta
                if (!(ss >> elemento)) { // Armazena no elemento
                    break; // Se não conseguir, dá break
                }
                custos.push_back(std::make_pair(elemento, std::stoi(quantidade))); // Coloca o elemento e o peso de sua aresta no pair
                // o stoi converte a string quantidade para um inteiro :)
            }

            else if (!(ss >> quantidade >> elemento)) { // Armazena o par quantidade-elemento (ex: 10 hidrogenio)
                break; // Se não conseguir, morra
            }
        }

        // Atualização do mapa 

        auto it = dados.find(elemento); // Procura no mapa se o elemento atual já tem uma chave correspondente

        if (it == dados.end()) { // Se o iterador estiver apontando para o final do mapa, o elemento não foi encontrado
            dados[elemento] = custos; // Cria-se uma chave com o nome do elemento e atribui-se o custos a esse elemento
            // Ou seja, se a linha lida é '10 hidrogenio -> 1 iterbio', cria-se uma chave iterbio que receberá o hidrogenio e 10
            // ficando: iterbio{(hidrogenio,10)}
        }
        else { // Se o elemento já existe no mapa
            it->second.insert(it->second.end(), custos.begin(), custos.end());
            // Acrescenta mais informações no final do vetor contendo as informações do elemento
            // Se a linha lida for '4 cromo 3 praseodimio -> 1 zinco', ficaria com 'zinco{(cromo,4),(praseoodimio,3)}', mas se logo depois
            // houver outra linha como '8 promecio 9 paladio -> 1 zinco', ficaria 'zinco{(cromo,4),(praseoodimio,3),(promecio,4),(paladio,9)}'
        }
    }

    math::Integer quantHidro = calculoHidrogenio("ouro"); 
    // Chama a função calculoHidrogenio para calcular a quantidade de hidrogênio necessária para produzir o ouro, armazenando em quantHidro

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << quantHidro << "\n\n" << std::fixed << std::setprecision(1) << duration.count() << " nanossegundos." << std::endl;

    return 0;
}
