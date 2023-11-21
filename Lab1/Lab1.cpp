#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>
#include <glm/ext/matrix_relational.hpp>

constexpr float THRESHOLD{ 10E-5F };

void firstExercice(glm::vec3 view, glm::vec3 up, glm::vec3 vSolution, glm::vec3 wSolution, glm::vec3 uSolution)
{
    glm::vec3 v = glm::normalize(view);
    glm::vec3 w = glm::normalize(glm::cross(up, v));
    glm::vec3 u = glm::cross(v, w);

    std::cout << std::endl;
    std::cout << "Coordinate frame for view: " << view << " and up: " << up << std::endl;
    std::cout << "v: " << v;
    assert(glm::all(glm::equal(v, vSolution, THRESHOLD)));
    std::cout << " Correct" << std::endl;
    std::cout << "w: " << w;
    assert(glm::all(glm::equal(w, wSolution, THRESHOLD)));
    std::cout << " Correct" << std::endl;
    std::cout << "u: " << u;
    assert(glm::all(glm::equal(u, uSolution, THRESHOLD)));
    std::cout << " Correct" << std::endl;
}

void secondExercice()
{

}

void thirdExercice(glm::mat3 A, glm::mat3 B)
{
    glm::mat3 AB = A * B;
    glm::mat3 inverseAB = glm::inverse(AB);

    glm::mat3 inverseBinverseA = glm::inverse(B) * glm::inverse(A);

    assert(glm::all(glm::equal(inverseAB, inverseBinverseA, THRESHOLD)));
}

int main()
{
    // first exercice
    std::cout << "-- First Exercice --" << std::endl;
    firstExercice({ 0.0F, 0.0F, 2.0F }, { 0.0F, 1.0F, 1.0F }, { 0.0F, 0.0F, 1.0F }, { 1.0F, 0.0F, 0.0F }, { 0.0F, 1.0F, 0.0F });
    firstExercice({ 0.0F, 0.0F, -3.0F }, { 0.0F, 1.0F, 1.0F }, { 0.0F, 0.0F, -1.0F }, { -1.0F, 0.0F, 0.0F }, { 0.0F, 1.0F, 0.0F });
    firstExercice({ 0.0F, 0.0F, 1.0F }, { -1.0F, 0.0F, -1.0F }, { 0.0F, 0.0F, 1.0F }, { 0.0F, 1.0F, 0.0F }, { -1.0F, 0.0F, 0.0F });

    firstExercice({ 0.0F, 2.0F, 0.0F }, { 0.0F, 1.0F, 1.0F }, { 0.0F, 1.0F, 0.0F }, { -1.0F, 0.0F, 0.0F }, { 0.0F, 0.0F, 1.0F });
    firstExercice({ 0.0F, -3.0F, 0.0F }, { 0.0F, 1.0F, 1.0F }, { 0.0F, -1.0F, 0.0F }, { 1.0F, 0.0F, 0.0F }, { 0.0F, 0.0F, 1.0F });
    firstExercice({ 0.0F, 1.0F, 0.0F }, { 1.0F, 1.0F, 0.0F }, { 0.0F, 1.0F, 0.0F }, { 0.0F, 0.0F, 1.0F }, { 1.0F, 0.0F, 0.0F });

    firstExercice({ 2.0F, 0.0F, 0.0F }, { 1.0F, 1.0F, 0.0F }, { 1.0F, 0.0F, 0.0F }, { 0.0F, 0.0F, -1.0F }, { 0.0F, 1.0F, 0.0F });
    firstExercice({ -3.0F, 0.0F, 0.0F }, { 1.0F, 1.0F, 0.0F }, { -1.0F, 0.0F, 0.0F }, { 0.0F, 0.0F, 1.0F }, { 0.0F, 1.0F, 0.0F });
    firstExercice({ 1.0F, 0.0F, 0.0F }, { -1.0F, 0.0F, 1.0F }, { 1.0F, 0.0F, 0.0F }, { 0.0F, 1.0F, 0.0F }, { 0.0F, 0.0F, 1.0F });

    //second exercise

    //third exercise
}
