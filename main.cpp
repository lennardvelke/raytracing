#include "camera.h"
#include "vec3.h"
#include <cmath>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include <vector>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


// Vertex Shader Quellcode
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aTexCoord;
    out vec2 TexCoord;
    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0);
        TexCoord = aTexCoord;
    }
)";

static std::string loadFile(const char* path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open file: " << path << std::endl;
        return std::string();
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

static GLuint compileShaderChecked(GLenum type, const std::string& src, const char* tag) {
    if (src.empty()) {
        std::cerr << "ERROR: Shader source for " << tag << " is empty\n";
        return 0;
    }
    GLuint s = glCreateShader(type);
    const char* cstr = src.c_str();
    glShaderSource(s, 1, &cstr, nullptr);
    glCompileShader(s);

    GLint ok = 0;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    GLint len = 0;
    glGetShaderiv(s, GL_INFO_LOG_LENGTH, &len);
    if (len > 1) {
        std::string log(len, '\0');
        glGetShaderInfoLog(s, len, nullptr, &log[0]);
        std::cerr << "Shader log (" << tag << "):\n" << log << std::endl;
    }
    if (!ok) {
        std::cerr << "ERROR: Compilation failed for shader: " << tag << std::endl;
        glDeleteShader(s);
        return 0;
    }
    return s;
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, Camera* camera) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
      camera->position.z = camera->position.z + 1.0f;
  }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      camera->position = camera->position - camera->forward;
  }
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      camera->position = camera->position + camera->forward;
  }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      camera->position = camera->position - camera->right;
  }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      camera->position = camera->position + camera->right;
  }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
      camera->position = camera->position - camera->up;
  }
    if(glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS) {
      camera->up = normalize(rotateAroundAxis(camera->right, camera->up, 2.5f*M_PI/180.0f));
      camera->forward = normalize(crossProduct(camera->right, camera->up));
  }
    if(glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS) {
      camera->up = normalize(rotateAroundAxis(camera->right, camera->up, -2.5f*M_PI/180.0f));
      camera->forward = normalize(crossProduct(camera->right, camera->up));
  }
    if(glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS) {
      camera->right = normalize(rotateAroundAxis(camera->up, camera->right, 2.5f*M_PI/180.0f));
      camera->forward = normalize(crossProduct(camera->right, camera->up));
  }
    if(glfwGetKey(window, GLFW_KEY_RIGHT ) == GLFW_PRESS) {
      camera->right = normalize(rotateAroundAxis(camera->up, camera->right, -2.5f*M_PI/180.0f));
      camera->forward = normalize(crossProduct(camera->right, camera->up));
  }
    if(glfwGetKey(window, GLFW_KEY_I ) == GLFW_PRESS) {
    camera->number_of_samples_per_ray ++;

    std::cout << "Number of Samples:" << camera->number_of_samples_per_ray<<std::endl;
  }
    if(glfwGetKey(window, GLFW_KEY_O ) == GLFW_PRESS) {
    camera->number_of_samples_per_ray  = camera->number_of_samples_per_ray << 2;

    std::cout << "Number of Samples:" << camera->number_of_samples_per_ray<<std::endl;
  }
  
    if(glfwGetKey(window, GLFW_KEY_K ) == GLFW_PRESS) {
    camera->number_of_samples_per_ray = std::max(camera->number_of_samples_per_ray-1, 0);

    std::cout << "Number of Samples:" << camera->number_of_samples_per_ray<<std::endl;
  }
    if(glfwGetKey(window, GLFW_KEY_L ) == GLFW_PRESS) {
    camera->number_of_samples_per_ray  = std::max(camera->number_of_samples_per_ray >> 2,0);
    
    std::cout << "Number of Samples:" << camera->number_of_samples_per_ray<<std::endl;
  }

}

int main() {
    // GLFW initialisieren
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // GLFW Fensterkonfiguration
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Fenster erstellen
    GLFWwindow* window = glfwCreateWindow(800, 800, "Raytracer with Scene Input", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);

    // GLAD initialisieren
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Vertex Daten für ein Vollbild-Quad
    float vertices[] = {
        // Positionen   // Texturkoordinaten
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f,  0.0f, 1.0f
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Vertex Buffer Object (VBO), Vertex Array Object (VAO) und Element Buffer Object (EBO)
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Shader Programm erstellen
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


  std::string fragmentSrc = loadFile("./shader/fragment_shader.glsl");

  GLuint fragmentShader = compileShaderChecked(GL_FRAGMENT_SHADER, fragmentSrc, "fragmentShader"); 

    // Shader Programm linken
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

  Camera camera;
  camera.position = {0.0f,0.0f,0.0f};
  camera.up = {0.0f, 0.0f, 1.0f};
  camera.right = {0,1,0};  
  camera.number_of_samples_per_ray = 10; 
  camera.fov = 60;
  camera.forward = {1.0f, 0.0f,  0.0f};
  camera.ratio = 800.0f / 800.0f;
   

// glEnable(GL_FRAMEBUFFER_SRGB);

    // Uniform-Locations abrufen
    GLint camPosLoc = glGetUniformLocation(shaderProgram, "cam_pos");
    GLint camDirLoc = glGetUniformLocation(shaderProgram, "cam_forward");
    GLint camUpLoc = glGetUniformLocation(shaderProgram, "cam_up");
    GLint aspectRatioLoc = glGetUniformLocation(shaderProgram, "cam_ratio");
    GLint camFOV = glGetUniformLocation(shaderProgram, "cam_fov");
    GLint timeLoc = glGetUniformLocation(shaderProgram, "u_time");
    GLint frameLoc = glGetUniformLocation(shaderProgram, "u_numAccumulatedFrames");
    GLint numberLoc = glGetUniformLocation(shaderProgram, "u_num_rays_per_pixel");
    uint count = 0;
    // Render Loop
  //
  //
  //
    float timeStart = glfwGetTime();
  float timeEnd = glfwGetTime();  
  while (!glfwWindowShouldClose(window)) {
    timeStart = glfwGetTime();    
    processInput(window, &camera);
        count++;
        

        
        // Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Uniforms setzen
        glUseProgram(shaderProgram);
        glUniform3fv(camPosLoc, 1, (GLfloat *)&camera.position);
        glUniform3fv(camDirLoc, 1, (GLfloat *)&camera.forward);
        glUniform1f(camFOV, camera.fov);
        glUniform3fv(camUpLoc, 1, (GLfloat *)&camera.up);
        glUniform1f(timeLoc, glfwGetTime());
        glUniform1f(aspectRatioLoc, camera.ratio);
        glUniform1ui(frameLoc, count);
        glUniform1ui(numberLoc, camera.number_of_samples_per_ray); 
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
     if (round(fmod(glfwGetTime() ,5)) == 0) {
     
      std::cout<< "FPS: "<<1.0f/(glfwGetTime() - timeStart) << std::endl;
     }  

    }

    // Aufräumen
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
