#include "ForwardStep.h"

ForwardStep::ForwardStep(Camera* camera, Program& program) 
	: camera(camera), program(program){

}

void ForwardStep::RenderStep() {

	glBindFramebuffer(GL_FRAMEBUFFER, *FBO);

	program.Use();
	program.SetMat4("projM", camera->GetProjectionMatrix());
	program.SetMat4("viewM", camera->GetLookAtMatrix());

	for (unsigned int i = 0; i < dataTextures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, *dataTextures.at(i));
	}

	for (int i = 0; i < models.size(); i++) {

		LightCube* aux = dynamic_cast<LightCube*>(models.at(i));
		Model* auxModel = dynamic_cast<Model*>(models.at(i));

		program.SetMat4("modelM", models.at(i)->transform.globalModel);

		if (aux != nullptr) {
			program.SetVec3("lightColor", aux->color);
		}else {
			program.SetMat4("normalM", glm::transpose(glm::inverse(models.at(i)->transform.globalModel)));
		}

		auxModel->Draw(program);
		
	}

	DrawCamerasFrustum();

}

void ForwardStep::DrawCamerasFrustum() {

	for (int i = 0; i < inactiveCameras.size(); i++) {

		if (inactiveCameras.at(i)->GetDrawFrustum()) {
			glm::mat4 inv = glm::inverse(inactiveCameras.at(i)->GetLookAtMatrix());

			float ar = inactiveCameras.at(i)->GetAR();
			float fov = inactiveCameras.at(i)->GetFOV();
			float near = inactiveCameras.at(i)->GetNear();
			float far = inactiveCameras.at(i)->GetFar();;
			float halfHeight = tanf(glm::radians(fov / 2.f));
			float halfWidth = halfHeight * ar;

			float xn = halfWidth * near;
			float xf = halfWidth * far;
			float yn = halfHeight * near;
			float yf = halfHeight * far;

			glm::vec4 f[8u] = {
				// near face
				{xn, yn, -near, 1.f},
				{-xn, yn, -near, 1.f},
				{xn, -yn, -near, 1.f},
				{-xn, -yn,-near , 1.f},

				// far face
				{xf, yf, -far, 1.f},
				{-xf, yf,-far , 1.f},
				{xf, -yf,-far , 1.f},
				{-xf, -yf,-far, 1.f},
			};
			glm::vec3 v[8];
			for (int j = 0; j < 8; j++)
			{
				glm::vec4 ff = inv * f[j];
				v[j].x = ff.x / ff.w;
				v[j].y = ff.y / ff.w;
				v[j].z = ff.z / ff.w;
			}

			float frustumVertices[24] = {
				v[0].x, v[0].y, v[0].z, //0
				v[1].x, v[1].y, v[1].z, //1
				v[2].x, v[2].y, v[2].z, //2
				v[3].x, v[3].y, v[3].z, //3

				v[4].x, v[4].y, v[4].z, //4
				v[5].x, v[5].y, v[5].z, //5
				v[6].x, v[6].y, v[6].z, //6
				v[7].x, v[7].y, v[7].z //7
			};
			unsigned int indices[24] = {
				//First cube (near)
				0, 1,
				1, 3,
				2, 3,
				2, 0,
				//Second cube (far)
				4, 5,
				5, 7,
				6, 7,
				6, 4,
				//Side lines
				0, 4,
				1, 5,
				2, 6,
				3, 7
			};

			unsigned int VAO;
			unsigned int VBO;
			unsigned int EBO;

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(frustumVertices), frustumVertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			program.SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

	}
}