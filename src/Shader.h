#pragma once

#include <glad\glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

class Shader
{
	public:
		// the program ID
		unsigned int ID;

		Shader(const char* vertexPath, const char* fragmentPath)
		{
			// 1. retrieve the vertex/fragment source code from filePath
			std::string vertexCode;
			std::string fragmentCode;
			std::ifstream vShaderFile;
			std::ifstream fShaderFile;
			// ensure ifstream objects can throw exceptions:
			vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			try
			{
				// open files
				vShaderFile.open(vertexPath);
				fShaderFile.open(fragmentPath);
				std::stringstream vShaderStream, fShaderStream;
				// read file's buffer contents into streams
				vShaderStream << vShaderFile.rdbuf();
				fShaderStream << fShaderFile.rdbuf();
				// close file handlers
				vShaderFile.close();
				fShaderFile.close();
				// convert stream into string
				vertexCode = vShaderStream.str();
				fragmentCode = fShaderStream.str();
			}
			catch (std::ifstream::failure e)
			{
				std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << std::filesystem::current_path() << std::endl;
			}

			const char* vShaderCode = vertexCode.c_str();
			const char* fShaderCode = fragmentCode.c_str();

			// 2. compile shaders
			unsigned int vertexShader, fragmentShader;
			vertexShader = glCreateShader(GL_VERTEX_SHADER);
			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(vertexShader, 1, &vShaderCode, NULL);
			glCompileShader(vertexShader);

			int compileStatus;
			char infoLog[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
			if (!compileStatus)
			{
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				std::cout << "Error in Vertex code: " << infoLog << std::endl;
			}

			glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
			glCompileShader(fragmentShader);
			
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
			if (!compileStatus)
			{
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				std::cout << "Error in Fragment code: " << infoLog << std::endl;
			}

			// shader Program
			ID = glCreateProgram();
			glAttachShader(ID, vertexShader);
			glAttachShader(ID, fragmentShader);
			glLinkProgram(ID);
			// print linking errors if any
			glGetProgramiv(ID, GL_LINK_STATUS, &compileStatus);
			if (!compileStatus)
			{
				glGetShaderInfoLog(ID, 512, NULL, infoLog);
				std::cout << "Error in Program code: " << infoLog << std::endl;
			}

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
		}
		void use()
		{
			glUseProgram(ID);
		}
		void setBool(const std::string &name, bool value) const
		{
			glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
		}
		void setInt(const std::string& name, int value) const
		{
			glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
		}
		void setFloat(const std::string& name, float value) const
		{
			glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
		}
};