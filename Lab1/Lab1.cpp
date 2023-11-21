// Lab1.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>
#include <glm/ext/matrix_relational.hpp>

constexpr float THRESHOLD{ 10E-5F };


void first(glm::vec3 view, glm::vec3 up, glm::vec3 vSolution, glm::vec3 wSolution, glm::vec3 uSolution)
{
    glm::vec3 v = glm::normalize(view);
    glm::vec3 w = glm::normalize(glm::cross(up, v));
    glm::vec3 u = glm::cross(v, w);

    std::cout << std::endl;
    std::cout << "Coordinate frame for view: " << view << " and up: " << up << std::endl;
    std::cout << v;
    assert(glm::all(glm::equal(v, vSolution, THRESHOLD)));
    std::cout << " Correct" << std::endl;
    std::cout << w;
    assert(glm::all(glm::equal(w, wSolution, THRESHOLD)));
    std::cout << " Correct" << std::endl;
    std::cout << u;
    assert(glm::all(glm::equal(u, uSolution, THRESHOLD)));
    std::cout << " Correct" << std::endl;

}

void second()
{

}

void third(glm::mat3 A, glm::mat3 B)
{
    glm::mat3 AB = A * B;
    glm::mat3 inverseAB = glm::inverse(AB);

    glm::mat3 inverseBinverseA = glm::inverse(B) * glm::inverse(A);

    assert(glm::all(glm::equal(inverseAB, inverseBinverseA, THRESHOLD)));
    //assert(glm::all(glm::equal(glm::determinant(inverseAB), glm::determinant(inverseBinverseA), THRESHOLD)));

    //verificar que determinante e 0
}

int main()
{
    std::cout << "Hello World!\n";
    first({ 0.0F, 0.0F, 2.0F }, { 0.0F, 1.0F, 1.0F }, { 0.0F, 0.0F, 1.0F }, { 1.0F, 0.0F, 0.0F }, { 0.0F, 1.0F, 0.0F });
    first({ 0.0F, 0.0F, -3.0F }, { 0.0F, 1.0F, 1.0F }, { 0.0F, 0.0F, -1.0F }, { -1.0F, 0.0F, 0.0F }, { 0.0F, 1.0F, 0.0F });
    //first({ 0.0F, 1.0F, 1.0F }, { 0.0F, -1.0F, 1.0F }, { 0.0F, 0.0F, 1.0F }, { 1.0F, 0.0F, 0.0F }, { 0.0F, 1.0F, 0.0F });

    first({ 0.0F, 2.0F, 0.0F }, { 0.0F, 1.0F, 1.0F }, { 0.0F, 1.0F, 0.0F }, { -1.0F, 0.0F, 0.0F }, { 0.0F, 0.0F, 1.0F });
    first({ 0.0F, -3.0F, 0.0F }, { 0.0F, 1.0F, 1.0F }, { 0.0F, -1.0F, 0.0F }, { 1.0F, 0.0F, 0.0F }, { 0.0F, 0.0F, 1.0F });
    //first({ 0.0F, 0.0F, 2.0F }, { 0.0F, 1.0F, -1.0F }, { 0.0F, 0.0F, 1.0F }, { 1.0F, 0.0F, 0.0F }, { 0.0F, 1.0F, 0.0F });

    first({ 2.0F, 0.0F, 0.0F }, { 1.0F, 1.0F, 0.0F }, { 0.0F, 0.0F, 1.0F }, { 1.0F, 0.0F, 0.0F }, { 0.0F, 1.0F, 0.0F });
    first({ -3.0F, 0.0F, 0.0F }, { 1.0F, 1.0F, 0.0F }, { 0.0F, 0.0F, 1.0F }, { 1.0F, 0.0F, 0.0F }, { 0.0F, 1.0F, 0.0F });
    first({ 0.0F, 0.0F, 2.0F }, { -1.0F, 0.0F, 1.0F }, { 0.0F, 0.0F, 1.0F }, { 1.0F, 0.0F, 0.0F }, { 0.0F, 1.0F, 0.0F });
}

// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
