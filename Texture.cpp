#include"libraries.h"
#include"SHADER.H"
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

void framebuffer_size_callback(GLFWwindow* window,int width,int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH=800;
const unsigned int SCR_HEIGHT=600;

int main(int argc,char **argv){
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

  #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE,GL_TRUE);
  #endif

  GLFWwindow* window=glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"Texture",NULL,NULL);
  if(window==NULL){
    std::cout<<"Failed to create window"<<std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    std::cout<<"Failed to initialize GLAD"<<std::endl;
    exit(EXIT_FAILURE);
  }

  Shader ourShader("textures_vert.glsl","textures_frag.glsl");

  float vertices[]={
    //positions         //colors          //texture coordinates
    0.5f,0.5f,0.0f,      1.0f,0.0f,0.0f,    1.0f,1.0f, //top right
    0.5f,-0.5f,0.0f,     0.0f,1.0f,0.0f,    1.0f,0.0f, //bottom right
    -0.5f,-0.5f,0.0f,    0.0f,0.0f,1.0f,    0.0f,0.0f, //bottom left
    -0.5f,0.5f,0.0f,     1.0f,1.0f,0.0f,    0.0f,1.0f  //top left
  };
  unsigned int indices[]={
    0,1,3,
    1,2,3
  };

  unsigned int VBO,VAO,EBO;
  glGenVertexArrays(1,&VAO);
  glGenBuffers(1,&VBO);
  glGenBuffers(1,&EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER,VBO);
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
  glEnableVertexAttribArray(2);

  unsigned int texture;
  glGenTextures(1,&texture);
  glBindTexture(GL_TEXTURE_2D,texture);

  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

  int width,height,nrChannels;
  std::cout<<argv[1]<<"  "<<argv[0]<<std::endl;
  unsigned char* data=stbi_load(argv[1],&width,&height,&nrChannels,0);
  if(data){
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }else{
    std::cout<<"Failed to load texture"<<std::endl;
  }
  stbi_image_free(data);

  while(!glfwWindowShouldClose(window)){
    processInput(window);

    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D,texture);

    ourShader.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1,&VAO);
  glDeleteBuffers(1,&VBO);
  glDeleteBuffers(1,&EBO);

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow* window){
  if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS){
    glfwSetWindowShouldClose(window,true);
  }
}

void framebuffer_size_callback(GLFWwindow* window,int width,int height){
  glViewport(0,0,width,height);
}