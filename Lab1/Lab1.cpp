#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>
#include <glm/ext/matrix_relational.hpp>

constexpr float THRESHOLD{ 10E-5F };

void firstExercise(glm::vec3 view, glm::vec3 up, glm::vec3 vSolution, glm::vec3 wSolution, glm::vec3 uSolution)
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

glm::mat3 dualMatrix(glm::vec3 vector) {
    return glm::mat3({ 0.0f, vector.z, -vector.y }, { -vector.z, 0, vector.x }, { vector.y, -vector.x, 0 });
}

void secondExercise(glm::vec3 rotationVector) {

    float angle = glm::half_pi<float>();

    glm::mat3 RodriguesRotationMatrix;
    glm::mat3 A = dualMatrix(rotationVector);

    //Calculate the Rodrigues Rotation Matrix
    RodriguesRotationMatrix = glm::mat3(1.0f) + sin(angle) * A + (1 - cos(angle)) * A * A;

    std::cout << std::endl;
    std::cout << "Rotation Vector: " << rotationVector << std::endl;
    std::cout << "Angle: " << angle << std::endl;
    std::cout << "Rodrigues Rotation Matrix: " << RodriguesRotationMatrix << std::endl;
    std::cout << std::endl;

    //Calculate the Rotation Matrix by vector (1, 0, 0), (0, 1, 0), (0, 0, 1)
    glm::vec3 vec1(1.0f, 0.0f, 0.0f);
    glm::vec3 vec2(0.0f, 1.0f, 0.0f);
    glm::vec3 vec3(0.0f, 0.0f, 1.0f);

    glm::vec3 vec1Rotated = RodriguesRotationMatrix * vec1;
    glm::vec3 vec2Rotated = RodriguesRotationMatrix * vec2;
    glm::vec3 vec3Rotated = RodriguesRotationMatrix * vec3;

    std::cout << "Vector 1: " << vec1 << std::endl;
    std::cout << "Vector 2: " << vec2 << std::endl;
    std::cout << "Vector 3: " << vec3 << std::endl;
    std::cout << std::endl;

    std::cout << "Vector 1 Rotated: " << vec1Rotated << std::endl;
    std::cout << "Vector 2 Rotated: " << vec2Rotated << std::endl;
    std::cout << "Vector 3 Rotated: " << vec3Rotated << std::endl;

    //Rotation vector is x
    if (glm::all(glm::equal(rotationVector, glm::vec3(1.0f, 0.0f, 0.0f)))) {
        assert(glm::all(glm::equal(vec1Rotated, glm::vec3(1.0f, 0.0f, 0.0f)))); //x rotating 90 degress is still x
        assert(glm::all(glm::equal(vec2Rotated, glm::vec3(0.0f, 0.0f, 1.0f)))); //y rotating 90 degress is z
        assert(glm::all(glm::equal(vec3Rotated, glm::vec3(0.0f, -1.0f, 0.0f)))); //z rotating 90 degress is -y
        std::cout << "Correct" << std::endl;
    }
    //Rotation vector is y
    else if (glm::all(glm::equal(rotationVector, glm::vec3(0.0f, 1.0f, 0.0f)))) {
        assert(glm::all(glm::equal(vec1Rotated, glm::vec3(0.0f, 0.0f, -1.0f)))); //x rotating 90 is -z
        assert(glm::all(glm::equal(vec2Rotated, glm::vec3(0.0f, 1.0f, 0.0f)))); //y rotating 90 degress is still y
        assert(glm::all(glm::equal(vec3Rotated, glm::vec3(1.0f, 0.0f, 0.0f)))); //z rotating 90 degress is x
        std::cout << "Correct" << std::endl;
    }
    //Rotation vector is z
    else if (glm::all(glm::equal(rotationVector, glm::vec3(0.0f, 0.0f, 1.0f)))) {
        assert(glm::all(glm::equal(vec1Rotated, glm::vec3(0.0f, 1.0f, 0.0f)))); //x rotating 90 degress is y
        assert(glm::all(glm::equal(vec2Rotated, glm::vec3(-1.0f, 0.0f, 0.0f)))); //y rotating 90 degress is -x
        assert(glm::all(glm::equal(vec3Rotated, glm::vec3(0.0f, 0.0f, 1.0f)))); //z rotating 90 degress is still z
        std::cout << "Correct" << std::endl;
    }
}

void thirdExercise()
{
    //Array of 10 random numbers
    int randomNumbers1[10];
    int randomNumbers2[10];

    for (int i = 0; i < 10; i++)
    {
        //Random number from -10 to 10
        randomNumbers1[i] = rand() % 21 - 10;
        randomNumbers2[i] = rand() % 21 - 10;
    }

    glm::mat3 matrix1(randomNumbers1[0], randomNumbers1[1], randomNumbers1[2],
        randomNumbers1[3], randomNumbers1[4], randomNumbers1[5],
        randomNumbers1[6], randomNumbers1[7], randomNumbers1[8]);

    glm::mat3 matrix2(randomNumbers2[0], randomNumbers2[1], randomNumbers2[2],
        randomNumbers2[3], randomNumbers2[4], randomNumbers2[5],
        randomNumbers2[6], randomNumbers2[7], randomNumbers2[8]);

    std::cout << std::endl;
    std::cout << "Matrix 1: " << matrix1 << std::endl;
    std::cout << "Matrix 2: " << matrix2 << std::endl;

    glm::mat3 result1 = glm::inverse(matrix1 * matrix2);
    glm::mat3 result2 = glm::inverse(matrix2) * glm::inverse(matrix1);

    std::cout << "inverse(m1 * m2): " << result1 << std::endl;
    std::cout << "inverse(m2) * inverse(m1): " << result2 << std::endl;

    assert(glm::all(glm::equal(result1, result2, THRESHOLD)));
    std::cout << "Correct" << std::endl;
}

int main()
{
    // first exercice
    std::cout << "-- First Exercice --" << std::endl;
    firstExercise({ 0.0F, 0.0F, 2.0F }, { 0.0F, 1.0F, 1.0F }, { 0.0F, 0.0F, 1.0F }, { 1.0F, 0.0F, 0.0F }, { 0.0F, 1.0F, 0.0F });
    firstExercise({ 0.0F, 0.0F, -3.0F }, { 0.0F, 1.0F, 1.0F }, { 0.0F, 0.0F, -1.0F }, { -1.0F, 0.0F, 0.0F }, { 0.0F, 1.0F, 0.0F });
    firstExercise({ 0.0F, 0.0F, 1.0F }, { -1.0F, 0.0F, -1.0F }, { 0.0F, 0.0F, 1.0F }, { 0.0F, 1.0F, 0.0F }, { -1.0F, 0.0F, 0.0F });

    firstExercise({ 0.0F, 2.0F, 0.0F }, { 0.0F, 1.0F, 1.0F }, { 0.0F, 1.0F, 0.0F }, { -1.0F, 0.0F, 0.0F }, { 0.0F, 0.0F, 1.0F });
    firstExercise({ 0.0F, -3.0F, 0.0F }, { 0.0F, 1.0F, 1.0F }, { 0.0F, -1.0F, 0.0F }, { 1.0F, 0.0F, 0.0F }, { 0.0F, 0.0F, 1.0F });
    firstExercise({ 0.0F, 1.0F, 0.0F }, { 1.0F, 1.0F, 0.0F }, { 0.0F, 1.0F, 0.0F }, { 0.0F, 0.0F, 1.0F }, { 1.0F, 0.0F, 0.0F });

    firstExercise({ 2.0F, 0.0F, 0.0F }, { 1.0F, 1.0F, 0.0F }, { 1.0F, 0.0F, 0.0F }, { 0.0F, 0.0F, -1.0F }, { 0.0F, 1.0F, 0.0F });
    firstExercise({ -3.0F, 0.0F, 0.0F }, { 1.0F, 1.0F, 0.0F }, { -1.0F, 0.0F, 0.0F }, { 0.0F, 0.0F, 1.0F }, { 0.0F, 1.0F, 0.0F });
    firstExercise({ 1.0F, 0.0F, 0.0F }, { -1.0F, 0.0F, 1.0F }, { 1.0F, 0.0F, 0.0F }, { 0.0F, 1.0F, 0.0F }, { 0.0F, 0.0F, 1.0F });

    //second exercise
    std::cout << std::endl << "-- Second Exercice --" << std::endl;
    secondExercise({ 1.0f, 0.0f, 0.0f }); // rotate every axis around the x axis
    secondExercise({ 0.0f, 1.0f, 0.0f }); // rotate every axis around the y axis
    secondExercise({ 0.0f, 0.0f, 1.0f }); // rotate every axis around the z axis

    //third exercise
    std::cout << std::endl << "-- Third Exercice --" << std::endl;
    for (int i = 0; i <= 10; i++)
    {
        thirdExercise();
    }
}
