#ifndef SHADER_H
#define SHADER_H
#include"libs.h"

class Shader {

public:

	GLuint ID;
	Shader(const int versionMajor, const int versionMinor, const char* vertexPath, const char* fragmentPath)
		:versionMajor(versionMajor), versionMinor(versionMinor) {
		GLuint vertexShader = 0;
		GLuint fragmentShader = 0;

		vertexShader = loadShader(GL_VERTEX_SHADER, vertexPath);
		fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentPath);

		this->linkProgram(vertexShader, fragmentShader);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	~Shader() {
		glDeleteProgram(this->ID);
	}
	void use() {
		glUseProgram(this->ID);
	};
	void unuse()
	{
		glUseProgram(0);
	}

	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setMat4(const std::string &name, glm::mat4 value) {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}


private:

	const GLint versionMajor;
	const GLint versionMinor;

	GLint loadShader(GLenum type, const char* path) {
		GLchar info_log[512];
		int success;

		GLint shader = glCreateShader(type);
		std::string src = loadShaderSource(path);
		const GLchar* src_c = src.c_str();
		glShaderSource(shader, 1, &src_c, NULL);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, info_log);
			std::cout << "ERROR::loadshader::can't_compile_vertex_shader" << "\n";
			std::cout << "info_log : " << info_log << "\n";
		}
		return shader;
	}

	std::string loadShaderSource(const char* path) {
		std::string temp = "";
		std::string src = "";

		std::ifstream in_file;
		in_file.open(path);
		if (in_file.is_open())
		{
			while (std::getline(in_file, temp)) {
				src += temp + "\n";
			}
		}
		else {
			std::cout << "Shader::loadShaderSource : faild to open " << path << "\n";
		}

		in_file.close();

		std::string ver = std::to_string(this->versionMajor) +
			std::to_string(this->versionMinor) +
			"0";

		src.replace(src.find("#version"), 12, ("#version " + ver));

		return src;
	}

	void linkProgram(GLint vertex, GLint fragment) {
		GLchar info_log[512];
		int success;

		this->ID = glCreateProgram();
		glAttachShader(this->ID, vertex);
		glAttachShader(this->ID, fragment);
		glLinkProgram(this->ID);

		glGetProgramiv(this->ID, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(this->ID, 512, NULL, info_log);
			std::cout << "ERROR::loadshader::can't_compile_program" << "\n";
			std::cout << "link info_log : " << info_log << "\n";
		}

		glUseProgram(0);
	}
};


#endif