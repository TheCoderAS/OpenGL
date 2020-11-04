#include"libraries.h"
using namespace std;

Vertex vertices[]={
  //Position                     //Color                  //TextCoord
  vec3(0.0f,0.5f,0.f),           vec3(1.f,0.f,0.f),       vec2(0.f,1.f),
  vec3(-0.5f,-0.5f,0.f),         vec3(0.f,1.f,0.f),       vec2(0.f,0.f),
  vec3(0.5f,-0.5f,0.f),          vec3(0.f,0.f,1.f),       vec2(1.f,0.f)  
};

unsigned nrOfVertices=sizeof(vertices)/sizeof(Vertex);

GLuint indices[]={
  0,1,2
};
unsigned nrOfIndices=sizeof(indices)/sizeof(GLuint);


void updateInput(GLFWwindow* window){
  if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS){
    glfwSetWindowShouldClose(window,GLFW_TRUE);
  }
}

bool LoadShaders(GLuint &program){
  bool loadsuccess=true;
  char infoLog[512];
  GLint success;

  string temp="";
  string src="";
  
  ifstream in_file;

  //Vertex
  in_file.open("triangles_vert.glsl");
  if(in_file.is_open()){
    while(getline(in_file,temp)){
      src+=temp+"\n";
    }
  }else{
    loadsuccess=false;
    cout<<"ERROR: Cannot load vertex shader\n"<<endl;
  }
  in_file.close();

  GLuint vertexShader =glCreateShader(GL_VERTEX_SHADER);
  const GLchar* vertSrc=src.c_str();
  glShaderSource(vertexShader,1,&vertSrc,NULL);
  glCompileShader(vertexShader);
  //Error check
  glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
  if(!success){
    glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
    cout<<"ERROR: Could not compile shader\n";
    cout<<infoLog<<endl;
    loadsuccess=false;
  }

  temp="";
  src="";
  //Fragment
  in_file.open("triangles_frag.glsl");
  if(in_file.is_open()){
    while(getline(in_file,temp)){
      src+=temp+"\n";
    }
  }else{
    loadsuccess=false;
    cout<<"ERROR: Cannot load fragment shader\n"<<endl;
  }
  in_file.close();

  GLuint fragmentShader =glCreateShader(GL_FRAGMENT_SHADER);
  const GLchar* fragSrc=src.c_str();
  glShaderSource(fragmentShader,1,&fragSrc,NULL);
  glCompileShader(fragmentShader);
  //Error check
  glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
  if(!success){
    glGetShaderInfoLog(fragmentShader,512,NULL,infoLog);
    cout<<"ERROR: Could not compile shader\n";
    cout<<infoLog<<endl;
    loadsuccess=false;
  }

  //Program
  program=glCreateProgram();
  glAttachShader(program,vertexShader);
  glAttachShader(program,fragmentShader);

  glLinkProgram(program);

  glGetProgramiv(program,GL_LINK_STATUS,&success);
  if(!success){
    glGetProgramInfoLog(program,512,NULL,infoLog);
    cout<<"ERROR: Could not link program\n";
    cout<<infoLog<<endl;
    loadsuccess=false;
  }

  //End
  glUseProgram(0);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return loadsuccess;
}

void init(GLFWwindow* window){

}
void display(GLFWwindow* window, double currentTime){
  glClearColor(0.0,0.0,0.0,1.0);
  glClear(GL_COLOR_BUFFER_BIT);
}

int main(){
  cout<<"Initializing game"<<endl;

  if(!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4); //Determine the major version of compatible OpenGL version 4.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3); //Determine the major version of compatible OpenGL version 4.3

  GLFWwindow* window =glfwCreateWindow(800,600,"Create Window",NULL,NULL);// Create window
  glfwMakeContextCurrent(window);//Make opengl current context

  if(glewInit()!=GLEW_OK)
    exit(EXIT_FAILURE);

  //OpenGL options
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  //Shaders

  GLuint core_program;
  if(!LoadShaders(core_program)){
    exit(EXIT_FAILURE);
  }

  //Model

  //generate vao and bind it
  GLuint VAO;
  glCreateVertexArrays(1,&VAO);
  glBindVertexArray(VAO);

  //generate vbo and bind and send data
  GLuint VBO;
  glGenBuffers(1,&VBO);
  glBindBuffer(GL_ARRAY_BUFFER,VBO);
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

  //generate ebo and bind and send data
  GLuint EBO;
  glGenBuffers(1,&EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

  //set vertexattibpointer and enable
  //position
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(GLvoid*)offsetof(Vertex,position));
  glEnableVertexAttribArray(0);
  //color
  glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(GLvoid*)offsetof(Vertex,color));
  glEnableVertexAttribArray(1);

  //textcoord
  glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(GLvoid*)offsetof(Vertex,textcoord));
  glEnableVertexAttribArray(2);

  //Bind vao 0

  //glBindVertexArray(0);
      glUseProgram(core_program);


  //Main LOOP

  glfwSwapInterval(1);
  init(window);

  while(!glfwWindowShouldClose(window)){
    glfwPollEvents();
    updateInput(window);
    display(window,glfwGetTime());
    //use a program

    //bind vertex array object
    //glBindVertexArray(VAO);

    //draw
            glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

    //glDrawArrays(GL_TRIANGLES,0,nrOfIndices);
    glDrawElements(GL_TRIANGLES,nrOfIndices, GL_UNSIGNED_INT,0);

    glfwSwapBuffers(window);
    glFlush();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  exit(EXIT_SUCCESS);

}