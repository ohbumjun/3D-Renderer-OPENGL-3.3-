#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stb_image.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "FileSystem.h"
#include <iostream>
#include <stb_image.h>
#include "Shader.h"
#include "Model.h"
#include "Camera.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);
unsigned int loadCubemap(std::vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    std::string vrxShaderPath = FileSystem::getPath("LearnOpenGL/advanced_glsl.glsl");
    std::string fragShaderPath = FileSystem::getPath("LearnOpenGL/8.red.glsl");
    Shader shaderRed(vrxShaderPath.c_str(), fragShaderPath.c_str());

    vrxShaderPath = FileSystem::getPath("LearnOpenGL/ModelVertex.glsl");
    fragShaderPath = FileSystem::getPath("LearnOpenGL/FragCoordFrag.glsl");
    Shader modelVertexShader(vrxShaderPath.c_str(), fragShaderPath.c_str());

    vrxShaderPath = FileSystem::getPath("LearnOpenGL/advanced_glsl.glsl");
    fragShaderPath = FileSystem::getPath("LearnOpenGL/8.green.glsl");
    Shader shaderGreen(vrxShaderPath.c_str(), fragShaderPath.c_str());

    fragShaderPath = FileSystem::getPath("LearnOpenGL/8.blue.glsl");
    Shader shaderBlue(vrxShaderPath.c_str(), fragShaderPath.c_str());

    fragShaderPath = FileSystem::getPath("LearnOpenGL/8.yellow.glsl");
    Shader shaderYellow(vrxShaderPath.c_str(), fragShaderPath.c_str());

    vrxShaderPath = FileSystem::getPath("LearnOpenGL/HouseVertex.glsl");
    fragShaderPath = FileSystem::getPath("LearnOpenGL/HouseFrag.glsl");
    std::string geoShaderPath = FileSystem::getPath("LearnOpenGL/HouseGeomtery.glsl");
    Shader houseGeoShader(vrxShaderPath.c_str(), fragShaderPath.c_str(), 
        geoShaderPath.c_str());

    vrxShaderPath = FileSystem::getPath("LearnOpenGL/ExplodingVertex.glsl");
    fragShaderPath = FileSystem::getPath("LearnOpenGL/ExplodingFrag.glsl");
    geoShaderPath =
        FileSystem::getPath("LearnOpenGL/ExplodingGeomtery.glsl");
    Shader explodingGeoShader(vrxShaderPath.c_str(),
                          fragShaderPath.c_str(),
                          geoShaderPath.c_str());

    vrxShaderPath = FileSystem::getPath("LearnOpenGL/VisualizeNormalVertex.glsl");
    fragShaderPath = FileSystem::getPath("LearnOpenGL/VisualizeNormalFrag.glsl");
    geoShaderPath = FileSystem::getPath("LearnOpenGL/VisualizeNormalGeomtery.glsl");
    Shader visualizingNormalShader(vrxShaderPath.c_str(),
                              fragShaderPath.c_str(),
                              geoShaderPath.c_str());

    // load models
    // -----------
    std::string modelPath =
        FileSystem::getPath("BJResource/Resources/backpack/backpack.obj");
    Model sampleModel(modelPath.c_str());

    // cube VAO
    unsigned int textureCubeVAO, textureCubeVBO;
    float textureCubeVertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,
        0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,
    
        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
    
        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,
    
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    
        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f,
        0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f,
        0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f,
    
        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    
    unsigned int indices[] = {
        // note that we start from 0!
        0,
        1,
        3, // first triangle
        1,
        2,
        3 // second triangle
    };
    
    /*
    * VAO
    * - 속성 ~ VBO configure 정보를 한번에 담는 object
    * - 이것만 bind 시키기만 하면 된다.
    * - 뿐만 아니라, EBO 정보도 같이 가지고 있는다.
    */
    glGenVertexArrays(1, &textureCubeVAO);
    
    // VAO 를 먼저 bind 시킨다.
    // 그 다음에 비로소 vbo, 속성을 configure 한다.
    glBindVertexArray(textureCubeVAO);
    
    /*
    * indexed buffer 를 위한 object :GL_ELEMENT_ARRAY_BUFFER
    */
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                    sizeof(indices),
                    indices,
                    GL_STATIC_DRAW);
    
    glGenBuffers(1, &textureCubeVBO);
    
    // Opengl 은 여러 type 의 buffer object 를 가지고 있다.
    // 그리고 'vetex buffer object' 의 type 이 GL_ARRAY_BUFFER 이다.
    // opengl 은, 서로 다른 type의 여러 개 buffer 를 bind 할 수 있게 해준다.
    glBindBuffer(GL_ARRAY_BUFFER, textureCubeVBO);
    
    // 해당 VBO 메모리 공간에, 현재 vertices 의 데이터를 복사한다.
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(textureCubeVertices),
                 textureCubeVertices,
                    /*
                * 그래픽 카드가 우리가 제공한 데이터를 어떤식으로 다룰지를 결정한다.
                *
                * > GL_STATIC_DRAW : data 는 한번 set, 여러 번 사용됨
                * > GL_DYNAMIC_DRAW : data 는 여러 번 set, 여러 번 사용됨
                * > GL_STREAM_DRAW : data 는 한번 set, 정말 적게 사용됨
                *
                * 우리가 현재 넘겨주는 정점 정보는 , 변하지 않고, 여러번 사용된다.
                * 만약 자주 변하는 정보라면 GL_DYNAMIC_DRAW 을 사용하여
                * Opengl 이 빠르게 쓸 수 있는 메모리 공간으로 데이터를 위치시킬 수 있게
                * 해야 한다.
                */
                    GL_STATIC_DRAW);
    
    // shader 을 준비하는 것만으로는 부족하다
    // vertex shader 는, 아직 메모리 상의 vertex data 를
    // 어떻게 해석해야 할지 모른다. 따라서 vertex attribute 을
    // 통해서 이를 알려주어야 한다.
    glVertexAttribPointer(
        0, // 어떤 vertex attribute 을 configure 하는 것인가 // ex) layout(location = 0) in vec3 aPos;
        3,                 // vertex attribute 의 크기
        GL_FLOAT,          // 데이터의 type
        GL_FALSE,          // 정규화 여부
        8 * sizeof(float), // 각 정점 데이터 사이의 간격
        (void *)0 // 메모리 상에서 data 가 시작하는 offset
    );
    
    // 해당 속성을 enable 시켜야 한다.
    // vertex 속성들은, 기본적으로 disable 되어 있다.
    glEnableVertexAttribArray(0);
    
    // color attribute
    // glVertexAttribPointer(1,
    //                       3,
    //                       GL_FLOAT,
    //                       GL_FALSE,
    //                       8 * sizeof(float),
    //     // color 정보는, vertex 정보 다음에 위치하므로 아래와 같은 offset 세팅
    //                       (void *)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                            3,
                            GL_FLOAT,
                            GL_FALSE,
                            8 * sizeof(float),
                            (void *)(3 * sizeof(float)));
    
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2,
                            2,
                            GL_FLOAT,
                            GL_FALSE,
                            8 * sizeof(float),
                            (void *)(6 * sizeof(float)));
    
    glEnableVertexAttribArray(2);
    
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // VAO unbind 시키기

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cubeVertices[] = {
        // positions
        -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f,
        0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f,
        0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,
        0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f,
    };
    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(cubeVertices),
                 &cubeVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          3 * sizeof(float),
                          (void *)0);

    float housePoints[] = {
        -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, // top-left
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, // top-right
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
    };
    unsigned int houseVBO, houseVAO;
    glGenBuffers(1, &houseVBO);
    glGenVertexArrays(1, &houseVAO);
    glBindVertexArray(houseVAO);
    glBindBuffer(GL_ARRAY_BUFFER, houseVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(housePoints),
                 &housePoints,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          5 * sizeof(float),
                          (void *)(2 * sizeof(float)));
    glBindVertexArray(0);

    unsigned int cubeTexture = loadTexture(
        FileSystem::getPath("BJResource/marble.jpg").c_str());
    unsigned int grassTexture = loadTexture(
        FileSystem::getPath("BJResource/grass.png").c_str());

    // uniform block index : shader 에서 정의된 uniform block 안에서의 index
    unsigned int uniformBlockIndexRed =
        glGetUniformBlockIndex(shaderRed.ID, "Matrices");
    unsigned int uniformBlockIndexGreen =
        glGetUniformBlockIndex(shaderGreen.ID, "Matrices");
    unsigned int uniformBlockIndexBlue =
        glGetUniformBlockIndex(shaderBlue.ID, "Matrices");
    unsigned int uniformBlockIndexYellow =
        glGetUniformBlockIndex(shaderYellow.ID, "Matrices");

    // then we link each shader's uniform block to this uniform binding point
    // shader 안에 있는 특정 uniform block 을 여러 binding point 중 하나에
    // link 시키는 함수
    glUniformBlockBinding(shaderRed.ID, 
        uniformBlockIndexRed, 
        0);
    glUniformBlockBinding(shaderGreen.ID, uniformBlockIndexGreen, 0);
    glUniformBlockBinding(shaderBlue.ID, uniformBlockIndexBlue, 0);
    glUniformBlockBinding(shaderYellow.ID, uniformBlockIndexYellow, 0);

    // Now actually create the uniform buffer object 
    unsigned int uboMatrices;
    glGenBuffers(1, &uboMatrices);

    // ubo 생성 이후, binding point 에 link 한다.
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);

    // ubo 를 위한 메모리를 할당한다 (아직 데이터 채워지지 않음)
    glBufferData(GL_UNIFORM_BUFFER,
                 2 * sizeof(glm::mat4), // uniform buffer object 의 크기
                 NULL,
                 GL_STATIC_DRAW);

    // uniform binding 해제
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // define the range of the buffer that links to a uniform binding point
    glBindBufferRange(GL_UNIFORM_BUFFER,
                      0,  // binding point for the uniform buffer
                      uboMatrices,// 해당 binding point 에 link 시키고 싶은 ubo
                      0, // This is the offset in bytes from the beginning of the  buffer object.In this case, the range starts from the beginning of the buffer.
                      2 * sizeof(glm::mat4)); // This is the size of the range in bytes

    // store the projection matrix (we only do this once now) (note: we're not using zoom anymore by changing the FoV)
    glm::mat4 projection =
        glm::perspective(45.0f,
                         (float)SCR_WIDTH / (float)SCR_HEIGHT,
                         0.1f,
                         100.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);

    // 전체 ubo 중에서 일부분만 update 한다.
    glBufferSubData(GL_UNIFORM_BUFFER,
                    0, // offset in byte
                    sizeof(glm::mat4), // size of data to be written
                    glm::value_ptr(projection)); // ptr to data
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    modelVertexShader.use();
    modelVertexShader.setInt("frontTexture", 0); 
    modelVertexShader.setInt("backTexture", 1); 

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        {
            // configure transformation matrices
            glm::mat4 projection =
                glm::perspective(glm::radians(45.0f),
                                 (float)SCR_WIDTH / (float)SCR_HEIGHT,
                                 1.0f,
                                 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            glm::mat4 model = glm::mat4(1.0f);
            explodingGeoShader.use();
            explodingGeoShader.setMat4("projection", projection);
            explodingGeoShader.setMat4("view", view);
            explodingGeoShader.setMat4("model", model);

            // add time component to geometry shader in the form of a uniform
            explodingGeoShader.setFloat("time",
                                        static_cast<float>(glfwGetTime()));

            // draw model
            sampleModel.Draw(explodingGeoShader);

            // then draw normal
            visualizingNormalShader.use();
            visualizingNormalShader.setMat4("projection", projection);
            visualizingNormalShader.setMat4("view", view);
            visualizingNormalShader.setMat4("model", model);

            sampleModel.Draw(visualizingNormalShader);
        }

        {
            // draw points
            // houseGeoShader.use();
            // glBindVertexArray(houseVAO);
            // glDrawArrays(GL_POINTS, 0, 4);
        }

        {
            // modelVertexShader.use();
            // 
            // glActiveTexture(GL_TEXTURE0); 
            // glBindTexture(GL_TEXTURE_2D, cubeTexture);
            // glActiveTexture(GL_TEXTURE1);
            // glBindTexture(GL_TEXTURE_2D, grassTexture);
            // 
            // glBindVertexArray(textureCubeVAO);
            // 
            // glm::mat4 model = glm::mat4(1.0f);
            // glm::mat4 view = camera.GetViewMatrix();
            // glm::mat4 projection =
            //     glm::perspective(glm::radians(camera.Zoom),
            //                         (float)SCR_WIDTH / (float)SCR_HEIGHT,
            //                         0.1f,
            //                         100.0f);
            // model = glm::translate(model,
            //                    glm::vec3(-0.f, 0.f, 0.0f)); // move top-left
            // 
            // modelVertexShader.setMat4("model", model);
            // modelVertexShader.setMat4("projection", projection);
            // modelVertexShader.setMat4("view", view);
            // 
            // glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        {
            //// set the view and projection matrix in the uniform block - we only have to do this once per loop iteration.
            //glm::mat4 view = camera.GetViewMatrix();
            //glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);

            //glBufferSubData(GL_UNIFORM_BUFFER,
            //                sizeof(glm::mat4), // ubo 중에 앞에  project 외에 이후 view 만 update
            //                sizeof(glm::mat4),
            //                glm::value_ptr(view));

            //glBindBuffer(GL_UNIFORM_BUFFER, 0);

            //// draw 4 cubes
            //// RED
            //glBindVertexArray(cubeVAO);
            //shaderRed.use();
            //glm::mat4 model = glm::mat4(1.0f);
            //model =
            //    glm::translate(model,
            //                   glm::vec3(-0.75f, 0.75f, 0.0f)); // move top-left
            //shaderRed.setMat4("model", model);
            //glDrawArrays(GL_TRIANGLES, 0, 36);

            ////// GREEN
            //shaderGreen.use();
            //model = glm::mat4(1.0f);
            //model = glm::translate(model,
            //                       glm::vec3(0.75f, 0.75f, 0.0f)); // move top-right
            //shaderGreen.setMat4("model", model);
            //glDrawArrays(GL_TRIANGLES, 0, 36);

            ////// YELLOW
            //shaderYellow.use();
            //model = glm::mat4(1.0f);
            //model =
            //    glm::translate(model,
            //                   glm::vec3(-0.75f, -0.75f, 0.0f)); // move bottom-left
            //shaderYellow.setMat4("model", model);
            //glDrawArrays(GL_TRIANGLES, 0, 36);

            ////// BLUE
            //shaderBlue.use();
            //model = glm::mat4(1.0f);
            //model =
            //    glm::translate(model,
            //                   glm::vec3(0.75f, -0.75f, 0.0f)); // move bottom-right
            //shaderBlue.setMat4("model", model);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &houseVAO);
    glDeleteBuffers(1, &houseVBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset =
        lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const *path)
{
     unsigned int textureID;
     glGenTextures(1, &textureID);

     int width, height, nrComponents;
     unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
     if (data)
     {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     format,
                     width,
                     height,
                     0,
                     format,
                     GL_UNSIGNED_BYTE,
                     data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,
                        GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
     }
     else
     {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
     }

     return textureID;
}