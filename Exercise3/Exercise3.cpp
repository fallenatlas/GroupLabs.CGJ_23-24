#include <iostream>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>
#include <glm/ext/matrix_relational.hpp>


constexpr float THRESHOLD{ 10E-5F };

void assertProposition() {
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

	std::cout << "Matrix 1: " << matrix1 << std::endl;
	std::cout << "Matrix 2: " << matrix2 << std::endl;

	glm::mat3 result1 = glm::inverse(matrix1 * matrix2);
	glm::mat3 result2 = glm::inverse(matrix2) * glm::inverse(matrix1);

	std::cout << "Result 1: " << result1 << std::endl;
	std::cout << "Result 2: " << result2 << std::endl;

	assert(glm::all(glm::equal(result1, result2, THRESHOLD)));
}


int main()
{
	for (int i = 0; i <= 10; i++)
	{
		assertProposition();
	}
}
