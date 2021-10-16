#include <glm/gtx/transform.hpp>
#include "Frustum.h"
void Frustum::ComputeFrustum(float fov, float near, float far, const glm::vec3& pos, const glm::vec3& view, float ratio)
{
	glm::vec3 centre_near = pos + (glm::normalize(view) * near);
	glm::vec3 centre_far = pos + (glm::normalize(view) * far);

	float fov_radians = glm::radians(fov / 2.0F);
	float near_height = 2.0F * glm::tan(fov) * near;
	float near_width =  near_height * ratio;
	float far_height = 2.0F * glm::tan(fov) * far;
	float far_width = far_height * ratio;

	//near plane points (botLeft, botRight, topRight, topLeft)
	mPoints[0] = {-near_width, -near_height, 1};
	mPoints[1] = {near_width,  -near_height, 1};
	mPoints[2] = {near_width,   near_height, 1};
	mPoints[3] = {-near_width,  near_height, 1};

	//far plane points (botLeft, botRight, topRight, topLeft)
	mPoints[4] = {-far_width, -far_height, 1};
	mPoints[5] = {far_width,  -far_height, 1};
	mPoints[6] = {far_width,   far_height, 1};
	mPoints[7] = {-far_width,  far_height, 1};

	//near plane
	mIndices[0] = 0;
	mIndices[1] = 1;
	mIndices[2] = 1;
	mIndices[3] = 2;
	mIndices[4] = 2;
	mIndices[5] = 3;
	mIndices[6] = 3;
	mIndices[7] = 4;
	//side lines
	mIndices[8] = 4;
	mIndices[9] = 0;
	mIndices[10] = 0;
	mIndices[11] = 5;
	mIndices[12] = 1;
	mIndices[13] = 6;
	mIndices[14] = 2;
	mIndices[15] = 6;
	mIndices[16] = 3;
	mIndices[17] = 7;
	//far plane
	mIndices[18] = 4;
	mIndices[19] = 5;
	mIndices[20] = 5;
	mIndices[21] = 6;
	mIndices[22] = 6;
	mIndices[23] = 7;
	mIndices[24] = 7;
	mIndices[25] = 4;
}

void Frustum::CreateModel()
{
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(2, mVBO);
	//binding the VAO
	glBindVertexArray(mVAO);

	//addign pos
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, mPoints.size() * sizeof(glm::vec3), &mPoints[0].x, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	//adding triangle indexes
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned short), &mIndices[0], GL_STATIC_DRAW);

	// Unbind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

GLsizei Frustum::GetDrawElements() { return static_cast<GLsizei>(mIndices.size()); }
