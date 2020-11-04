#ifndef SHADER_H
#define SHADER_H

#include"libraries.h"

class Shader{
  public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath){
      std::string vertexCode;
      std::string fragmentCode;
      std::ifstream vShaderFile;
      std::ifstream fShaderFile;

      //Open Shader files

      vShaderFile.exceptions(std::ifstream::failbit|std::ifstream::badbit);
      fShaderFile.exceptions(std::ifstream::failbit|std::ifstream::badbit);

      try{

        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream,fShaderStream;

        vShaderStream<<vShaderFile.rdbuf();
        fShaderStream<<fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode=vShaderStream.str();
        fragmentCode=fShaderStream.str();

      }catch(std::ifstream::failure& e){
        std::cout<<"ERROR::SHADER_H::FILES_NOT_OPENED_SUCCESSFULLY"<<std::endl;
      }
      const char* vShaderCode=vertexCode.c_str();
      const char* fShaderCode=fragmentCode.c_str();

      //compile shaders

      unsigned int vertex,fragment;
      //vertex shaders
      vertex=glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertex,1,&vShaderCode,NULL);
      glCompileShader(vertex);
      checkCompileErrors(vertex,"VERTEX");
      //fragment shaders
      fragment=glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragment,1,&fShaderCode,NULL);
      glCompileShader(fragment);
      checkCompileErrors(fragment,"FRAGMENT");
      //shader program
      ID=glCreateProgram();
      glAttachShader(ID,vertex);
      glAttachShader(ID,fragment);
      glLinkProgram(ID);
      checkCompileErrors(ID,"PROGRAM");
      //delete shaders
      glDeleteShader(vertex);
      glDeleteShader(fragment);
    }

    //activate shader
    void use(){
      glUseProgram(ID);
    }

    //utility uniform functions
    void setBool(const std::string &name,bool value) const {
      glUniform1i(glGetUniformLocation(ID,name.c_str()),(int)value);
    }
    void setInt(const std::string &name,int value) const {
      glUniform1i(glGetUniformLocation(ID,name.c_str()),value);
    }
    void setFloat(const std::string &name,float value) const {
      glUniform1f(glGetUniformLocation(ID,name.c_str()),value);
    }

  private:
    void checkCompileErrors(unsigned int shader, std::string type){
      int success;
      char infoLog[512];
      if(type!="PROGRAM"){
        glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
        if(!success){
          glGetShaderInfoLog(shader,1024,NULL,infoLog);
          std::cout<<"ERROR::SHADER_H::COMPILATION_ERROR of type: "<<type<<std::endl<<infoLog<<std::endl<<"-------------------------------------------------------------------------------------------------------"<<std::endl;
        }
      }else{
        glGetProgramiv(shader,GL_LINK_STATUS,&success);
        if(!success){
          glGetProgramInfoLog(shader,1024,NULL,infoLog);
          std::cout<<"ERROR::SHADER_H::PROGRAM_LINKING_ERROR of type: "<<type<<std::endl<<infoLog<<std::endl<<"-------------------------------------------------------------------------------------------------------"<<std::endl;
        }
      }
    }
};
#endif