#include "shader.h"

int ShaderProgram::load_shader_file(const string& file_name, string& file_contents) {
	ifstream src_file;
	src_file.open(file_name);

	if (src_file.is_open()) {
		//The file successfully opened, load the file's contents
		src_file.seekg(0, std::ios::end);
		file_contents.resize(src_file.tellg());
		src_file.seekg(0, std::ios::beg);
		src_file.read(&file_contents[0],file_contents.size());
		src_file.close();
	} else {
		//The file could not be opened report an error.
		return 1;
	}
	return 0;
}

void ShaderProgram::create_program() {
	program = glCreateProgram();
}

int ShaderProgram::add_shader(const GLenum shader_type, const string& file_name) {
	GLuint shader = glCreateShader(shader_type);

	//Load in the source code from the file
	string shader_source;
	if (load_shader_file(file_name, shader_source) != 0) {
		std::cout << "Source for " << file_name << " could not be loaded" << std::endl;
		return 1;
	}
	// std::cout << "Shader source: " << shader_source.size() << std::endl << shader_source << std::endl;


	//Set the source and compile the shader
	const char * src = shader_source.data();
	glShaderSource(shader, 1, (const GLchar**)&src, NULL);
	glCompileShader(shader);
	GLint compile_success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_success);
	if (compile_success == GL_FALSE) {
		//Check for error specifics
		GLint error_len = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &error_len);

		string error_msg;
		error_msg.reserve(error_len);
		glGetShaderInfoLog(shader, error_len, &error_len, (GLchar*)error_msg.data());

		//Log the error (currently just print)
		std::cout << "Compilation error in " << file_name << std::endl;
		std::cout << error_msg.data() << std::endl;

		glDeleteShader(shader);
		return 2;
	}

	//Attach the shader to the program
	glAttachShader(program, shader);

	GLint currprog;
	glGetProgramiv(program, GL_ATTACHED_SHADERS, &currprog);
	std::cout << program << "Attached progs: " << currprog << std::endl;

	//Add the shader to the list of included shaders
	shaders.push_back(pair<GLuint, GLenum>(shader, shader_type));
	std::cout << "SHADER SUCCESSFULLY ADDED: " << file_name << std::endl;
	return 0;
}

void ShaderProgram::link_program() {
	glLinkProgram(program);
	// GLint link_result;
	// glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	// if (link_result == GL_FALSE) {
	// 	GLint error_len = 0;
	// 	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &error_len);

	// 	string error_msg;
	// 	std::cout << "Program link error: " << error_len << std::endl;
	// 	error_msg.reserve(error_len);
	// 	glGetProgramInfoLog(program, error_len, &error_len, (GLchar*)error_msg.data());

	// 	//Log the error (currently just print)
	// 	for (int i = 0; i < error_len; i++) {
	// 		std::cout << error_msg[i];
	// 	}
	// 	std::cout << std::endl;
	// }

	
	GLint error_len = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &error_len);

	string error_msg;
	std::cout << "Program link info: " << error_len << std::endl;
	error_msg.reserve(error_len);
	glGetProgramInfoLog(program, error_len, &error_len, (GLchar*)error_msg.data());

	//Log the error (currently just print)
	for (int i = 0; i < error_len; i++) {
		std::cout << error_msg[i];
	}
	std::cout << std::endl;
	std::cout << "PROGRAM SUCCESSFULLY LINKED" << std::endl;
}

void ShaderProgram::add_attribute(string attr_name) {
	unsigned int new_attr = 0;
	if (shader_attributes.size() != 0) {new_attr = shader_attributes.back().first+1;}
	shader_attributes.emplace_back(pair<unsigned int, string>(new_attr, attr_name));
	// glBindAttribLocation(program, new_attr, attr_name.data());
	std::cout << "Adding attribute: " << attr_name.data() << std::endl;
	glBindAttribLocation(program, new_attr, attr_name.data());
}

GLint ShaderProgram::get_program() {
	return program;
}

void ShaderProgram::use() {
	glUseProgram(get_program());
}

ShaderHandler::ShaderHandler() {
}

ShaderHandler::~ShaderHandler() {
}

ShaderProgram& ShaderHandler::access_program(SH_prog_id shad_id) {
	if (shad_id == SH_NUM_SHAD) {
		std::cerr << "Attempted to use invalid shader" << std::endl;
	}
	return shaders[shad_id];
}