#pragma once

#include "../Material.hxx"
#include "GlShader.hxx"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

namespace kyanite::engine::rendering::opengl {
	class GlMaterial : public Material {
	public:
		uint32_t programId;

		GlMaterial(std::map<ShaderType, std::shared_ptr<Shader>> shaders, bool isInstanced) : Material(shaders) {
			// Convert the shaders to OpenGL shaders
			this->isInstanced = isInstanced;
			auto vertexShader = std::static_pointer_cast<GlShader>(shaders[ShaderType::VERTEX]);
			auto fragmentShader = std::static_pointer_cast<GlShader>(shaders[ShaderType::FRAGMENT]);

			// Create shader program
			GLuint shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram, vertexShader->shaderId);
			glAttachShader(shaderProgram, fragmentShader->shaderId);
			glLinkProgram(shaderProgram);

			// Check linking status
			GLint success;
			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
			if (!success) {
				GLchar infoLog[512];
				glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
				std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
				// TODO: Throw exception
			}
			programId = shaderProgram;
		}

		void Bind() override {
			if (!isInstanced) {
				// Disable instancing
				// Disable the atribpointers
				glDisableVertexAttribArray(2);
				glDisableVertexAttribArray(3);
				glDisableVertexAttribArray(4);
				glDisableVertexAttribArray(5);
				
				// Reset the divisor for the model matrix
				glVertexAttribDivisor(2, 0);
				glVertexAttribDivisor(3, 0);
				glVertexAttribDivisor(4, 0);
				glVertexAttribDivisor(5, 0);
			}

			// Check for errors
			GLenum error = glGetError();
			if (error != GL_NO_ERROR) {
				std::cerr << "Error setting up vertex attribute pointers: " << error << std::endl;
			}

			glUseProgram(programId);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textures.begin()->second->ID());
				
			// Set the sampler uniform to use texture unit 0
			GLint textureLocation = glGetUniformLocation(programId, "DIFFUSE_MAP");
			glUniform1i(textureLocation, 0);
		}

		void SetBuiltins(glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
			if (!isInstanced) {
				GLint modelLoc = glGetUniformLocation(programId, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			}
			// Pass matrices to the shader
			GLint viewLoc = glGetUniformLocation(programId, "view");
			GLint projLoc = glGetUniformLocation(programId, "projection");

			// Pass matrices to the shader
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		}
	};
}