#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>


glm::mat3 dualMatrix(glm::vec3 vector);

void assertProposition(glm::vec3 rotationVector) {

    float angle = glm::half_pi<float>();

    glm::mat3 RodriguesRotationMatrix;
    glm::mat3 A = dualMatrix(rotationVector);

    //Calculate the Rodrigues Rotation Matrix
    RodriguesRotationMatrix = glm::mat3(1.0f) + sin(angle) * A + (1 - cos(angle)) * A * A;

    std::cout << "Rotation Vector: " << rotationVector << std::endl;
    std::cout << "Angle: " << angle << std::endl;
    std::cout << "Rodrigues Rotation Matrix: " << RodriguesRotationMatrix << std::endl;
    std::cout << std::endl;

    //Calculate the ROtation Matrix by vector (1, 0, 0), (0, 1, 0), (0, 0, 1)
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
    std::cout << std::endl;

    //Rotation vector is x
    if (glm::all(glm::equal(rotationVector, glm::vec3(1.0f, 0.0f, 0.0f)))) {
        assert(glm::all(glm::equal(vec1Rotated, glm::vec3(1.0f, 0.0f, 0.0f)))); //x rotating 90 degress is still x
        assert(glm::all(glm::equal(vec2Rotated, glm::vec3(0.0f, 0.0f, 1.0f)))); //y rotating 90 degress is z
        assert(glm::all(glm::equal(vec3Rotated, glm::vec3(0.0f, -1.0f, 0.0f)))); //z rotating 90 degress is -y
    }
    //Rotation vector is y
    else if (glm::all(glm::equal(rotationVector, glm::vec3(0.0f, 1.0f, 0.0f)))) {
        assert(glm::all(glm::equal(vec1Rotated, glm::vec3(0.0f, 0.0f, -1.0f)))); //x rotating 90 is -z
        assert(glm::all(glm::equal(vec2Rotated, glm::vec3(0.0f, 1.0f, 0.0f)))); //y rotating 90 degress is still y
        assert(glm::all(glm::equal(vec3Rotated, glm::vec3(1.0f, 0.0f, 0.0f)))); //z rotating 90 degress is x
    }
    //Rotation vector is z
    else if (glm::all(glm::equal(rotationVector, glm::vec3(0.0f, 0.0f, 1.0f)))) {
        assert(glm::all(glm::equal(vec1Rotated, glm::vec3(0.0f, 1.0f, 0.0f)))); //x rotating 90 degress is y
        assert(glm::all(glm::equal(vec2Rotated, glm::vec3(-1.0f, 0.0f, 0.0f)))); //y rotating 90 degress is -x
        assert(glm::all(glm::equal(vec3Rotated, glm::vec3(0.0f, 0.0f, 1.0f)))); //z rotating 90 degress is still z
    }



}

int main()
{
    //Choose rotation vector and angle

    glm::vec3 rotationVector1(1.0f, 0.0f, 0.0f);
    glm::vec3 rotarionVector2(0.0f, 1.0f, 0.0f);
    glm::vec3 rotarionVector3(0.0f, 0.0f, 1.0f);

    assertProposition(rotationVector1);
    assertProposition(rotarionVector2);
    assertProposition(rotarionVector3);


}

glm::mat3 dualMatrix(glm::vec3 vector) {
    return glm::mat3({ 0.0f, vector.z, -vector.y }, { -vector.z, 0, vector.x }, { vector.y, -vector.x, 0 });
}


