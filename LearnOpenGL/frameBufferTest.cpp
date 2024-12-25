//#include <GLFW/glfw3.h>
//#include <glad/glad.h>
//#include "FileSystem.h"
//#include <iostream>
//#include <stb_image.h>
//#include "Shader.h"
//#include <map>
//#include "Camera.h"
//#include "Model.h"
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
///*
//* >> GLFW ? GLAD ?
//penGL의 specification을 OS에 상관없이 
//쉽게 implementation하게 도와주는 것이 GLFW와 GLAD
//
//>> GLFW ?
//LFW는 OpenGL 컨텍스트를 사용하여 창을 만들고 관리 할 수있는 작은 C 라이브러리
//
//>> GLAD ?
// GLAD는 흩어진 function의 위치를 찾아 함수포인터를 가져올 때에 
// 도움을 준다는 점
//*/
//
///*
//* window 창이 resize 될 때마다 호출되는 콜백 함수
//*/
//void framebuffer_size_callback(GLFWwindow *window, int width, int height);
//void mouse_callback(GLFWwindow *window, double xpos, double ypos);
//void processInput(GLFWwindow *window);
//void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
//unsigned int loadTexture(const char *path);
//unsigned int loadCubemap(std::vector<std::string> faces);
//    // // Camera 의 World Pos
//// glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//// // Camer Direction
//// glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//// // glm::cross 를 통해서 Camera Right 을 얻기 위한 up vector
//// glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//// float yaw = -90.0f;
//// float pitch = 0.0f;
//// float Zoom = 45.f;
//// float fov = 45.f;
//
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//
//float deltaTime = 0.0f; // Time between current frame and last frame
//
//float lastFrame = 0.0f; // Time of last frame
//
//
//float lastMouseX = 0.f, lastMouseY = 0.f; // store last pos
//
//bool firstMouse = true;
//
//// settings
//const unsigned int SCR_WIDTH = 1200;
//const unsigned int SCR_HEIGHT = 900;
//
//size_t vertexInfoSize = 8; //
//
//glm::vec3 dirLightDir(-0.2f, -1.f, -03.f);
//glm::vec3 dirLightPos(0.f, 5.f, 0.f);
//
//int main(int argc, char *argv[])
//{
//    #pragma region OpenGL 초기화
//    // GLFW 초기화
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    GLFWwindow *window =
//        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//
//    // GLAD 초기화
//    // GLAD manages function pointers for OpenGL
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    // tell OpenGL the size of the rendering window
//    // 0,0 : lower left corner
//    // 800, 600 : width, height
//    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
//
//    // 콜백함수 등록
//    // > window resize
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//    // > Mouse
//    glfwSetCursorPosCallback(window, mouse_callback);
//    // > Scroll
//    glfwSetScrollCallback(window, scroll_callback);
//
//    // vertex attribute 의 최대 개수
//    // - 보통 vertex shader 의 input 을 얘기할 때
//    // 'vertex attribute' 이라고 한다.
//    int nrAttributes;
//    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
//    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes
//              << std::endl;
//
//    #pragma endregion
//
//    struct point
//    {
//        int x;
//        int y;
//    };
//
//    #pragma region Shader Setting
//
//    // shader
//    std::string vrxShaderPath =
//        "D:\\OpenGL\\LearnOpenGL\\LearnOpenGLSrc\\LearnOpenGL\\DepthTestingVertex.glsl";
//    std::string fragShaderPath =
//        "D:\\OpenGL\\LearnOpenGL\\LearnOpenGLSrc\\LearnOpenGL\\DepthTestingFrag.glsl";
//   
//    Shader shader(vrxShaderPath.c_str(), fragShaderPath.c_str());
//
//    fragShaderPath = "D:\\OpenGL\\LearnOpenGL\\LearnOpenGLSrc\\LearnOpenGL\\SingleColorFrag.glsl";
//
//    Shader shaderSingleColor(vrxShaderPath.c_str(), fragShaderPath.c_str());
//   
//    fragShaderPath =
//        "D:\\OpenGL\\LearnOpenGL\\LearnOpenGLSrc\\LearnOpenGL\\SimpleBlendingDiscard.glsl";
//
//    Shader simpleBlendingDiscard(vrxShaderPath.c_str(), fragShaderPath.c_str());
//   
//    vrxShaderPath = 
//        "D:\\OpenGL\\LearnOpenGL\\LearnOpenGLSrc\\LearnOpenGL\\ScreenShaderVertex.glsl";
//
//    fragShaderPath = 
//        "D:\\OpenGL\\LearnOpenGL\\LearnOpenGLSrc\\LearnOpenGL\\ScreenShaderFrag.glsl";
//
//    Shader screenShader(vrxShaderPath.c_str(), fragShaderPath.c_str());
//
//    vrxShaderPath = "D:\\OpenGL\\LearnOpenGL\\LearnOpenGLSrc\\LearnOpenGL\\SkyBoxVertex.glsl";
//
//    fragShaderPath = "D:\\OpenGL\\LearnOpenGL\\LearnOpenGLSrc\\LearnOpenGL\\SkyBoxFrag.glsl";
//
//    Shader skyBoxShader(vrxShaderPath.c_str(), fragShaderPath.c_str());
//
//    #pragma endregion
//
//    #pragma region Vertex
//    float cubeVertices[] = {
//        // positions          // texture Coords
//        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
//        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
//        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
//
//        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
//        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//        -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,
//
//        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
//        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
//        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,
//
//        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
//        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
//        0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
//        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
//        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
//
//        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
//        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};
//
//   // float planeVertices[] = {
//   //     // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
//   //     5.0f, -0.5f, 5.0f,  2.0f,  0.0f,  -5.0f, -0.5f, 5.0f,
//   //     0.0f, 0.0f,  -5.0f, -0.5f, -5.0f, 0.0f,  2.0f,
//   // 
//   //     5.0f, -0.5f, 5.0f,  2.0f,  0.0f,  -5.0f, -0.5f, -5.0f,
//   //     0.0f, 2.0f,  5.0f,  -0.5f, -5.0f, 2.0f,  2.0f
//   // };
//
//    float planeVertices[] = {
//        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
//        -5.0f, -0.5f, 5.0f,  0.0f, 0.0f, 5.0f,  -0.5f, 5.0f,  2.0f, 0.0f,
//        5.0f,  -0.5f, -5.0f, 2.0f, 2.0f, 5.0f,  -0.5f, -5.0f, 2.0f, 2.0f,
//        -5.0f, -0.5f, -5.0f, 0.0f, 2.0f, -5.0f, -0.5f, 5.0f,  0.0f, 0.0f,
//    };
//
//    float quadVertices[] = {
//        // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
//        // positions   // texCoords
//        -1.0f, 1.0f, 0.0f, 1.0f,  -1.0f, -1.0f,
//        0.0f,  0.0f, 1.0f, -1.0f, 1.0f,  0.0f,
//
//        -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  -1.0f,
//        1.0f,  0.0f, 1.0f, 1.0f,  1.0f,  1.0f
//    };
//
//    float transparentVertices[] = {
//        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
//        0.0f, 0.5f, 0.0f, 0.0f,  0.0f, 0.0f, -0.5f, 0.0f,
//        0.0f, 1.0f, 1.0f, -0.5f, 0.0f, 1.0f, 1.0f,
//
//        0.0f, 0.5f, 0.0f, 0.0f,  0.0f, 1.0f, -0.5f, 0.0f,
//        1.0f, 1.0f, 1.0f, 0.5f,  0.0f, 1.0f, 0.0f};
//
//     float skyboxVertices[] = {
//        // positions
//        -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
//        1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,
//
//        -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
//        -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,
//
//        1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
//        1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,
//
//        -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
//        1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,
//
//        -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
//        1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,
//
//        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
//        1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};
//
//    // cube VAO
//    unsigned int cubeVAO, cubeVBO;
//    glGenVertexArrays(1, &cubeVAO);
//    glGenBuffers(1, &cubeVBO);
//    glBindVertexArray(cubeVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
//    glBufferData(GL_ARRAY_BUFFER,
//                 sizeof(cubeVertices),
//                 &cubeVertices,
//                 GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0,
//                          3,
//                          GL_FLOAT,
//                          GL_FALSE,
//                          5 * sizeof(float),
//                          (void *)0);
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1,
//                          2,
//                          GL_FLOAT,
//                          GL_FALSE,
//                          5 * sizeof(float),
//                          (void *)(3 * sizeof(float)));
//    glBindVertexArray(0);
//
//    // plane VAO
//    unsigned int planeVAO, planeVBO;
//    glGenVertexArrays(1, &planeVAO);
//    glGenBuffers(1, &planeVBO);
//    glBindVertexArray(planeVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
//    glBufferData(GL_ARRAY_BUFFER,
//                 sizeof(planeVertices),
//                 &planeVertices,
//                 GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0,
//                          3,
//                          GL_FLOAT,
//                          GL_FALSE,
//                          5 * sizeof(float),
//                          (void *)0);
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1,
//                          2,
//                          GL_FLOAT,
//                          GL_FALSE,
//                          5 * sizeof(float),
//                          (void *)(3 * sizeof(float)));
//    glBindVertexArray(0);
//
//    // transparent VAO
//    unsigned int transparentVAO, transparentVBO;
//    glGenVertexArrays(1, &transparentVAO);
//    glGenBuffers(1, &transparentVBO);
//    glBindVertexArray(transparentVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
//    glBufferData(GL_ARRAY_BUFFER,
//                 sizeof(transparentVertices),
//                 transparentVertices,
//                 GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0,
//                          3,
//                          GL_FLOAT,
//                          GL_FALSE,
//                          5 * sizeof(float),
//                          (void *)0);
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1,
//                          2,
//                          GL_FLOAT,
//                          GL_FALSE,
//                          5 * sizeof(float),
//                          (void *)(3 * sizeof(float)));
//    glBindVertexArray(0);
//
//     // screen quad VAO
//    unsigned int quadVAO, quadVBO;
//    glGenVertexArrays(1, &quadVAO);
//    glGenBuffers(1, &quadVBO);
//    glBindVertexArray(quadVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
//    glBufferData(GL_ARRAY_BUFFER,
//                 sizeof(quadVertices),
//                 &quadVertices,
//                 GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0,
//                          2,
//                          GL_FLOAT,
//                          GL_FALSE,
//                          4 * sizeof(float),
//                          (void *)0);
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1,
//                          2,
//                          GL_FLOAT,
//                          GL_FALSE,
//                          4 * sizeof(float),
//                          (void *)(2 * sizeof(float)));
//
//    // skybox VAO
//    unsigned int skyboxVAO, skyboxVBO;
//    glGenVertexArrays(1, &skyboxVAO);
//    glGenBuffers(1, &skyboxVBO);
//    glBindVertexArray(skyboxVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
//    glBufferData(GL_ARRAY_BUFFER,
//                 sizeof(skyboxVertices),
//                 &skyboxVertices,
//                 GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0,
//                          3,
//                          GL_FLOAT,
//                          GL_FALSE,
//                          3 * sizeof(float),
//                          (void *)0);
//
//    #pragma endregion
//
//    #pragma region Texture
//
//    // load textures
//    // -------------
//    unsigned int cubeTexture = loadTexture(
//        FileSystem::getPath("BJResource/marble.jpg").c_str());
//    unsigned int floorTexture = loadTexture(
//        FileSystem::getPath("BJResource/metal.png").c_str());
//    unsigned int grassTexture =
//        loadTexture(FileSystem::getPath("BJResource/grass.png").c_str());
//    unsigned int blendingTexture =
//        loadTexture(FileSystem::getPath("BJResource/window.png").c_str());
//
//    std::vector<std::string> faces
//    {  FileSystem::getPath("BJResource/Resources/skybox/right.jpg"), 
//        FileSystem::getPath("BJResource/Resources/skybox/left.jpg"), 
//        FileSystem::getPath("BJResource/Resources/skybox/top.jpg"), 
//        FileSystem::getPath("BJResource/Resources/skybox/bottom.jpg"), 
//        FileSystem::getPath("BJResource/Resources/skybox/front.jpg"), 
//        FileSystem::getPath("BJResource/Resources/skybox/back.jpg")
//    };
//
//    unsigned int cubemapTexture = loadCubemap(faces);
//
//    #pragma endregion
//
//    // glUseProgram(shaderProgram); // don’t forget to activate the shader first!
//    shader.use();
//    shader.setInt("skybox", 0); // 
//    
//    // skyBox Mapping
//    skyBoxShader.use();
//    skyBoxShader.setInt("skybox", 0);
//
//    #pragma region STENCIL TEST
//
///*
//* >> STENCIL TEST ?
//- depth test 처럼, 특정 frag 만 pass 하고, 나머지는 discard 시키는 기능 제공
//- 해당 단계에서 pass 된 대상만 depth test 로 넘어간다.
//
//>> STENCIL BUFFER
//- 각 픽셀당 8bit 의 stencil value 가 있다.
//0 ~ 255 까지의 값을 가질 수 있다.
//해당 값을 사용하여, 특정 stencil value 가 있는 frag 를 버리거나 pass 할 수 있다.
//*/
//
//    glEnable(GL_STENCIL_TEST);
//
//    glStencilFunc(
//        GL_NOTEQUAL, // 현재 stencil buffer 의 값이 ref 값과 같지 않은 frag 들만 pass 된다.
//        1, // 새로 pass 시킬 때 해당 frag 에 대한 stencil value 로 '1' 로 쓴다.
//        0xFF // write 허용 (모든 bit 의 숫자가 compare 된다)
//    );
//    glStencilOp(GL_KEEP, GL_KEEP, // stencil pass, fail 둘다 stencil 값 유지
//        GL_REPLACE); // depth test pass 시 stencil buffer 의 값을 ref 값으로 대체
//
//    // >> glStencilMask : stencil buffer 의 mask 를 설정한다.
//    // - 0xFF(default): 모든 bit 가 1 이므로, stencil buffer 의 모든 bit 가 write 를 허용한다.
//    // - 0x00 : 모든 bit 가 0 이므로, stencil buffer 의 모든 bit 가 write 를 허용하지 않는다.
//    // glStencilMask(0xFF); // each bit is written to as is
//    // glStencilMask(0x00); // each bit ends up as 0 (disabling writes)
//
//    /*
//    >> glStencilFunc(GLenum func, GLint ref, GLuint mask)
//    - func : stencil test 함수 세팅
//        frag 가 pass 될지 disacrd 될지 결정
//    - ref : stencil test 의 reference value -> stencil buffer 의 값과 비교된다.
//    - mask : 위에서의 glStencilMask 와 같은 원리
//
//    >> frag 옵션 예시 (depth 와 비슷한 의미)
//    GL_NEVER, GL_LESS, GL_LEQUAL, GL_GREATER,
//    GL_GEQUAL, GL_EQUAL, GL_NOTEQUAL and GL_ALWAYS.
//    */
//
//    /*
//    * >> glStencilFunc 함수가 오로지 opengl 이 frag 를 pass, discard 할지 결정한다.
//    * 한편, glStencilOp 함수는 stencil buffer 의 값을 어떻게 바꿀지(update) 결정한다.
//    * 
//    * glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass) 
//    * 
//    sfail: action to take if the stencil test fails.
//    dpfail: action to take if the stencil test passes, but the depth test fails.
//    dppass: action to take if both the stencil and the depth test pass.
//
//    GL_KEEP The currently stored stencil value is kept.
//    GL_ZERO The stencil value is set to 0.
//    GL_REPLACE The stencil value is replaced with the reference value set with glStencilFunc.
//    GL_INCR The stencil value is increased by 1 if it is lower than the maximum value.
//    GL_INCR_WRAP Same as GL_INCR, but wraps it back to 0 as soon as the maximum value is exceeded.
//    GL_DECR The stencil value is decreased by 1 if it is higher than the minimum value.
//    GL_DECR_WRAP Same as GL_DECR, but wraps it to the maximum value if it ends up lower than 0.
//    GL_INVERT Bitwise inverts the current stencil buffer value.
//
//    >> default :  (GL_KEEP, GL_KEEP, GL_KEEP) 
//
//    whatever the outcome of any of the tests, 
//    the stencil buffer keeps its values. 
//    
//    The default behavior does not update the stencil buffer, 
//    so if you want to write to the stencil buffer you need to specify at
//    least one different action for any of the options
//    */
//
//    #pragma endregion
//
//    #pragma region OBJECT OUTLINE
//
//    /*
//1. Enable stencil writing.
//
//2. Set the stencil op to GL_ALWAYS 
//before drawing the (to be outlined) objects, updating the
//stencil buffer with 1s wherever the objects’ fragments are rendered.
//
//- ALWAY : depth, stencil test 결과에 상관없이 항상 stencil buffer 를 update 한다.
//- ex) 우리가 outline 을 그리고자 하는 object 로 덮여진 모든 frag 에 1 을 write 한다.
//
//3. Render the objects.
//- outine 시키고 싶은 object 를 평소처럼 그린다
//- 이 과정에서 stencil buffer 에 1 이 write 된다. (object 가 드려지는 곳)
//- 이 과정은 2개의 목적이 있다. 
//1) 화면에 직접 그리기 
//2) outline 을 위한 stencil 값만 쓰기 위함으로 보인다.
//
//4. Disable stencil writing and depth testing.
//- 이후, stencil 에 저장된 값이 변경되지 않도록 stencil writing 을 disable 한다.
//- scaled-up 된 object 가 depth 와 상관없이 
//원본 object 위로 그려질 수 있도록 하기 위해서
//depth 를 disable 한다. (enable 하면, 이미 현재 해당 위치에 object 가 있고 같은 depth 여서
//안그려진다. 따라서 dsiable 시키고 무조건 그리게 하는 것으로 보인다)
//
//5. Scale each of the objects by a small amount.
//- 기존 object 보다 조금 더 크게 scale 하여 그린다.
//여기가 진짜 object 를 화면에 그려내기 위한 과정으로 보인다 ?
//
//6. Use a different fragment shader that outputs a single (border) color.
//- 새로운 frag shader 로 변경한다. 해당 shader 는
//모든 frag 에 대해서 single color 를 그린다.
//outline 을 그리기 위한 shader 이다.
//
//7. Draw the objects again, but only if their fragments’ 
//stencil values are not equal to 1.
//
//- 위에서 그린 scaled object 를 한번 더 변경된 shader 로 그린다
//- 이 경우에는 stencil buffer 의 값이 1 이 아닌 곳에만 그린다.
//자. 지금은 object 에 대해서 single color 를 그려내는 과정이다.
//단, stencil 이 '1' 이 아닌 곳에만 그린다
//기존에 scale 되기 전에 해당 object 가 맺힌 곳에는 stencil 이 1
//scale 되고 나면, scale 되기 전, scale 되기 후. 차이가 나는 부분에는
//stencil 이 1 이 아닌 곳이 있을 것이다.
//그 위치에만 현재 frag 로 single color 를 출력하는 원리이다.
//
//다시 말해, 이미 화면에는 object 가 그려져 있음
//이제 그 위에 외곽선만 그리고 싶은 것
//
//8. Enable depth testing again and restore stencil func to GL_KEEP.
//- 다시 depth testing 을 복구시킨다. 
//- stencil func 을 default state 로 복구 시킨다.
//scene 에서 다른 object 들에 대한 depth testing 이 필요하기 때문이다.
//
//즉, 그냥 쉽게 말해 다시 원래 상태로 복구 시켜서
//다음 렌더링에 영향을 주지 않도록 한다.
//
//    */
//
//    #pragma endregion
//
//    #pragma region DEPTH TEST
//    // enable depth test
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);
//    // glDepthFunc(GL_ALWAYS); // always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
//    /*
//    >> 옵션
//    GL_ALWAYS The depth test always passes.
//    GL_NEVER The depth test never passes.
//    GL_LESS Passes if the fragment’s depth value is less than the stored depth value.
//    GL_EQUAL Passes if the fragment’s depth value is equal to the stored depth value.
//    GL_LEQUAL Passes if the fragment’s depth value is less than or equal to the stored depth value.
//    GL_GREATER Passes if the fragment’s depth value is greater than the stored depth value.
//    GL_NOTEQUAL Passes if the fragment’s depth value is not equal to the stored depth value.
//    GL_GEQUAL Passes if the fragment’s depth value is greater than or equal to the stored depth value.
//    */
//
//
//#pragma endregion
//
//    #pragma region Camera
//
//    /*
//    // Camera 의 World Pos
//    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//
//    // Camer Direction
//    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
//
//    // Camera Dir : nagative z 축 방향 (오른손 좌표계)
//    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
//
//    // 아래의 camera right 을 얻기 위해서 up 을 정의 (일종의 trick)
//    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
//
//    // positive x- axis
//    // 참고 : 만약 negative x- axis 를 원한다면, cross 순서를 변경 ex) glm::cross(cameraDirection, up)
//    // ex) https://www.youtube.com/watch?v=eu6i7WJeinw 5:55
//    // ex) https://stackoverflow.com/questions/37519476/how-do-you-use-the-right-hand-rule-for-glmcross
//    // ex) glm::cross(x,y) = z
//    // ex) glm::cross(2번째 손가락, 3번째 손가락) = 엄지 손가락 (이것을 기준으로 생각)
//    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
//
//    // positive y- axis
//    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight); // y
//
//    // LookAt matrix
//    // 3개의 view / camera space vector 를 정의했다.
//    // 해당 matrix 들을 활용하여 LookAt matrix 를 만들 수 있다.
//    // 방법
//    // - 1) camera space origin 을 원점으로 이동시키는 translation matrix
//    //  ex) 현재 camera pos 가 (0,0,3) 이라면, -cameraPos 만큼 이동시키는 matrix
//    // - 2) camera space 의 axes 를 world space 의 axes 로 mapping 시키는 rotation matrix
//    //  ex) right, up, direction 을 row 로 하는 matrix !
//    // 이를 통해 만들어지는 LookAt matrix 는 모든 world 좌표계를 camera 좌표계로 이동시킨다.
//    glm::mat4 view;
//    
//    // sample
//    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), // Camera Pos
//                       glm::vec3(0.0f, 0.0f, 0.0f), // Camera Target Pos
//                       glm::vec3(0.0f, 1.0f, 0.0f)); // Camera Right 을 계산하기 위해 사용한 'up'
//    */
//
//    #pragma endregion
//
//    #pragma region Zoom
//
//    /*
//    * FOV (field of view) 가 Scene 에서 우리가 얼만큼 볼 수 있는지를 결정한다.
//    * fov 값이 작아지면, Scene 에 project 된 (투영된) 공간의 크기는 작아진다.
//    * 같은 NDC 공간에, 더 작은 공간이 투영되는 것이다.
//    * 이를 통해 zoom in 하는 것처럼 보이게 할 수 있다.
//    */
//
//    #pragma endregion
//
//    #pragma region Euler Angle
//    /*
//    * >> Euler Angle (p101)
//    * - 흔히 
//    1) pitch (x축 기준 회전 : 위,아래 ) 
//    2) yaw (y축 기준 회전 : 좌,우)
//    3) roll (z축 기준 회전 : 알아서 상상하삼) 을 사용하여 카메라를 회전시키는 방법
//    * - 각 축 ? 에서의 회전을 "combine" 시킨 하나의 single value 를 의미하게 된다.
//    * 
//    * Pitch, Yaw 만 일단 고려해본다.
//    * Pitch 와 Yaw 가 주어지면 새로운 direction 3d vector 를 얻을 수 있다.
//    * 
//    * >> Yaw 기준 회전 
//    * - xz 평명
//    * - 각도 : 반시계 방향 양수
//    * - x 축 : cos 각도 , z 축 : sin 각도
//
//    glm::vec3 direction;
//    direction.x = cos(glm::radians(yaw)); // convert to radians first
//    direction.z = sin(glm::radians(yaw));
//
//    * >> Pitch 기준 회전
//    * - yz 평면
//    * - 각도 : 위로 양수
//    * - y 축 : sin 각도 , z 축 : cos 각도
//    direction.y = sin(glm::radians(pitch));
//
//    >> 종합
//    - 그런데 x,z 평면은 cos (pitch) 에 의해 영향을 받는다.
//
//    (부가 설명)
//    >> 삼각함수의 성질:
//    직각삼각형에서 빗변의 길이가 1일 때, 
//    한 예각의 크기가 θ라면 빗변에 인접한 변의 길이는 cos(θ), 
//    빗변에 대변하는 변의 길이는 sin(θ)가 됩니다.
//
//    >> 피치 각과의 관계:
//    피치 각이 변함에 따라, 원래 x축과 z축 방향을 가리키던 벡터가 
//    x-z 평면 상에서 기울어지면서 새로운 직각삼각형이 형성됩니다.
//
//    이 새로운 직각삼각형에서 빗변의 길이는 변하지 않지만, 
//    빗변에 인접한 변의 길이(즉, x축과 z축 성분)는 cos(pitch)만큼 줄어들게 됩니다.
//
//    따라서, 원래 x축과 z축 성분에 cos(pitch)를 곱해주어야 
//    새로운 직각삼각형에서의 x축과 z축 성분을 정확하게 계산할 수 있습니다.
//
//    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//    direction.y = sin(glm::radians(pitch));
//    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//
//
//    >> 카메라 방향 조절 : yaw = -90.0f;
//    - 위 원리대로 한다면, 카메라는 맨 처음에 양의 x 축 방향을 향하게 된다.
//    - 그런데 default 로 z 음의 방향을 바라보게 하고 싶다.
//    - yaw 가 0 이면, 양의 x 축 방향을 가리키므로, 시계 방향 90도 회전을 시키면 된다.
//    - 양의 각도는 반 시계 방향을 가리키므로, yaw : -90 도
//
//    자. 여기서 의문일 수 있다. 아니 오른손 좌표계에서 y 축 기준 시계방향
//    회전시키면 x 축은 z 축 양의 방향을 가리키는 거 아닌가 ?
//    왜 그런데 저기서는 z 축 '음' 의 방향을 가라킨다고 얘기하는 거지 ?
//
//    여기서 말하는 yaw, pitch 는 오른손 좌표계랑 개념이 아예 다르다.
//    오른쪽 x, 위 y, 앞쪽이 z (즉, z 축이 opengl 좌표계랑 반대)
//
//    */
//    #pragma endregion
//
//    #pragma region MouseInput
//
//    /*
//    * >> Hide the cursor & Capture the cursor
//    - Hide the cursor : 커서가 화면상에 안보이게 해서 player 를 방해하지 않도록 한다.
//    - Capturing Cursor : cursor 의 움직임이 window 창 안에 있을 때만 영향 받도록 한다.
//            설령 window 창 밖에서 움직여도, cursor 은 window center 쪽으로 이동하게 된다.
//    */
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//    #pragma endregion
//
//    #pragma region BLEND
//
//    /*
//    >> Options
//    GL_ZERO Factor is equal to 0.
//    GL_ONE Factor is equal to 1.
//    GL_SRC_COLOR Factor is equal to the source color vector C¯
//    source.
//    GL_ONE_MINUS_SRC_COLOR Factor is equal to 1 minus the source color vector: 1−C¯
//    source.
//    GL_DST_COLOR Factor is equal to the destination color vector C¯
//    destination
//    GL_ONE_MINUS_DST_COLOR Factor is equal to 1 minus the destination color vector: 1−C¯
//    destination.
//    GL_SRC_ALPHA
//    Factor is equal to the al pha component of the source color vector
//    C¯
//    source.
//    GL_ONE_MINUS_SRC_ALPHA Factor is equal to 1−al pha of the source color vector C¯
//    source.
//    GL_DST_ALPHA
//    Factor is equal to the al pha component of the destination color vector
//    C¯
//    destination.
//    GL_ONE_MINUS_DST_ALPHA Factor is equal to 1−al pha of the destination color vector C¯
//    destination.
//    GL_CONSTANT_COLOR Factor is equal to the constant color vector C¯
//    constant.
//    GL_ONE_MINUS_CONSTANT_COLOR Factor is equal to 1 - the constant color vector C¯
//    constant.
//    GL_CONSTANT_ALPHA
//    Factor is equal to the al pha component of the constant color vector
//    C¯
//    constant.
//    GL_ONE_MINUS_CONSTANT_ALPHA Factor is equal to 1−al pha of the constant color vector C¯
//    constant.
//    */
//
//    // ex)  C¯result = C¯ source ∗Fsource + C¯ destination ∗Fdestination
//    glEnable(GL_BLEND);
//
//    // ex) src * alpha + dest * (1 - alpha)
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//    #pragma endregion
//
//    #pragma region FACE CULL
//
//    /*
//    >> OPENGL 기준으로,  front face 는 반시계 방향, back face 는 시계 방향
//    사용자가 삼각형을 보려면 정점 순서가 반시계 방향이어야 한다.
//    
//    >> 렌더링 과정
//    정점 셰이더 실행 이후, rasterization 단계에서 렌더링 순서가 결정된다.
//    뷰어의 시점에서 볼 때 삼각형의 면이 어떻게 보이는지를 고려하여 렌더링 순서가 결정됩니다.
//
//    >> 전면 삼각형과 후면 삼각형:
//    뷰어가 바라보는 쪽의 삼각형은 우리가 정의한 반시계 방향 순서대로 정상적으로 렌더링됩니다.
//    반면, 큐브의 뒷면에 위치한 삼각형은 뷰어의 시점에서 시계 방향으로 렌더링됩니다.
//    즉, 우리가 반시계 방향으로 정의했지만, 뷰어의 시점에서는 반대로 렌더링되는 것입니다.
//    
//    >> 백 페이스 컬링(Back-face Culling):
//    이러한 현상을 이용하여 숨겨진 면을 제거할 수 있습니다. 아예 그리지 않는 것이다.
//    뷰어의 시점에서 시계 방향으로 렌더링되는 삼각형(후면 삼각형)은 컬링(제거)함으로써 렌더링 성능을 향상
//    
//    >> 결론:
//    정점 순서를 반시계 방향으로 정의하면, 뷰어의 시점에서 전면 삼각형은 반시계 방향으로, 후면 삼각형은 시계 방향으로 렌더링됩니다.
//    이를 이용하여 후면 삼각형을 컬링하여 렌더링 성능을 향상시킬 수 있습니다
//    */
//
//    glEnable(GL_CULL_FACE);
//
//    #pragma endregion
//
//    #pragma region RENDERBUFFER OBJECT
//
//    /*
//    >> 기본 개념
//렌더버퍼 객체(Renderbuffer Object)에 대하여
//
//> 특징:
//
//텍스처 이미지와 유사하게 바이트, 정수, 픽셀 등의 데이터를 저장하는 버퍼입니다.
//직접 읽을 수 없습니다.
//OpenGL에서 메모리 최적화를 수행하여 텍스처보다 성능적으로 유리할 수 있습니다.
//장점:
//
//렌더링 데이터를 텍스처 포맷으로 변환하지 않고 직접 저장하여 쓰기 성능이 빠릅니다.
//버퍼 간의 데이터 복사가 빠릅니다.
//
//> 단점:
//
//직접 데이터를 읽을 수 없습니다.
//glReadPixels 함수를 사용하여 간접적으로 읽을 수 있지만 느립니다.
//활용 예:
//
//glfwSwapBuffers 함수와 유사하게 더블 버퍼링을 구현할 수 있습니다.
//렌더링 결과를 렌더버퍼에 쓰고, 두 개의 렌더버퍼를 교체하여 화면에 출력합니다.
//
//> 요약:
//
//렌더버퍼 객체는 렌더링 데이터를 빠르게 쓰고 저장할 수 있는 메모리 버퍼입니다. 
//직접 읽을 수 없지만, 렌더링 작업과 버퍼 교환 등의 작업에서 높은 성능을 보여줍니다.
//
//렌더버퍼 객체는 텍스처와 비슷하지만, 직접 읽을 수 없고 쓰기 성능이 뛰어납니다.
//렌더링 결과를 빠르게 저장하고 교환하는 데 적합합니다.
//    */
//
//    /*
//    * >> Frame Buffer 사용 방법
//    • We have to attach at least one buffer (color, depth or stencil buffer).
//    • There should be at least one color attachment.
//    - 이때 attachment 란, frame buffer 에서 buffer 로 사용되는 memory location 이다.
//    ex) image 같은 것.
//    보통 attachment 를 사용할 때는 2가지 옵션을 사용한다 : texture 혹은 render buffer object
//    • All attachments should be complete as well (reserved memory).
//    • Each buffer should have the same number of samples
//    */
//    
//    // framebuffer configuration
//    unsigned int framebuffer;
//    glGenFramebuffers(1, &framebuffer); // create frame buffer object
//
//    // 해당 Bind 를 호출한 이후, 모든 framebuffer operation 은 해당 framebuffer 에 대해 이루어진다.
//    // 참고 : 해당 frame buffer 를 read, write target 으로 bind 할 수 있다.
//    //      옵션 -> GL_READ_FRAMEBUFFER, GL_DRAW_FRAMEBUFFER
//    //      GL_READ_FRAMEBUFFER : glReadPixels 를 통해 ㅏㄱㅄ을 읽어올 수 있다.
//    //      GL_DRAW_FRAMEBUFFER : rendering 을 위한 target 으로 사용된다.
//    //      보통 GL_FRAMEBUFFER 를 사용. 위 두 옵션을 모두 제공
//    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); // bind it as framebuffer
//
//    // 
//     // create a color attachment texture (texture for framebuffer)
//    unsigned int textureColorbuffer;
//    glGenTextures(1, &textureColorbuffer);
//    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
//    glTexImage2D(GL_TEXTURE_2D,
//                 0,
//                 GL_RGB,
//                 SCR_WIDTH, // screen 크기만큼 설정
//                 SCR_HEIGHT,
//                 0,
//                 GL_RGB,
//                 GL_UNSIGNED_BYTE,
//                 NULL // Texture Data 에 null. 아직 데이터 채워지지 않음. 여기에 render 하면서 채워줄 거임
//    );
//    
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    // texture 를 생성하면 실제 frame buffer 에 attach 해야 한다.
//    // 참고 : 현재 texture 를 attach 하듯이, depth, stencil texture 도 attack 가능하다.
//    glFramebufferTexture2D(
//        GL_FRAMEBUFFER, // target frame buffer type ex) draw, read, both
//
//        // type of attachment
//        // 현재 우리는 color attachment 를 하는 중. 맨 끝에 '0' 이라는 것을 보면
//        // 알 수 있듯이, 우리는 1 개 보다 더 많은 color attachment 를 할 수 있다.
//        GL_COLOR_ATTACHMENT0, 
//                                    
//        GL_TEXTURE_2D, // type of texture
//        textureColorbuffer, // actual texture to attach
//        0); // mipmapping level
//    
//    // 자. 이제 해당 frame buffer 에 depth, stencil buffer 정보도 attach 하고 싶다
//    // 1) 일반 texture 가 아니라 최적화된 ? 메모리인 render buffer object
//    // 2) 32 bit 안에 24, 8 나눠서 각각 depth, stencil buffer 로 사용
//    // create a renderbuffer object for depth and stencil attachment (samping x)
//    unsigned int rbo;
//    glGenRenderbuffers(1, &rbo);
//    
//    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
//
//    // create a depth and stencil renderbuffer object
//    glRenderbufferStorage(
//        GL_RENDERBUFFER,
//        // depth, stencil attachment 를 위한 format (32 중 24 bit depth, 8 bit stencil)
//        GL_DEPTH24_STENCIL8,
//        SCR_WIDTH,
//        SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
//    
//    // attach the renderbuffer to the framebuffer object
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
//                              GL_DEPTH_STENCIL_ATTACHMENT,
//                              GL_RENDERBUFFER,
//                              rbo); // now actually attach it
//
//
//    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
//    // framebuffer 생성이 정상적으로 되었는지 확인
//    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//       std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
//   
//    // rendering 이 default framebuffer 에 되도록 다시 설정
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//    #pragma endregion
//
//    #pragma region CUBEMAP
//
//    /*
//    >> CubeMap 생성 방식
//- 6개의 texture 를 각 면에 대해서 만들어야 한다.
//ex) unsigned int textureID;
//glGenTextures(1, &textureID);
//glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
//
//>> CubeMap 6개 면 texture 각 생성
//- glTexImage2D 함수를 사용하여 각 면에 대한 texture 를 생성한다.
//- 이때, 어떤 면에 대한 것을 생성하는지에 대한 인자도 넘겨줘야 한다.
//GL_TEXTURE_CUBE_MAP_POSITIVE_X Right
//GL_TEXTURE_CUBE_MAP_NEGATIVE_X Left
//GL_TEXTURE_CUBE_MAP_POSITIVE_Y Top
//GL_TEXTURE_CUBE_MAP_NEGATIVE_Y Bottom
//GL_TEXTURE_CUBE_MAP_POSITIVE_Z Back
//GL_TEXTURE_CUBE_MAP_NEGATIVE_Z Front
//
//ex) 
//int width, height, nrChannels;
//unsigned char *data;
//for(unsigned int i = 0; i < textures_faces.size(); i++)
//{
//data = stbi_load(textures_faces[i].c_str(), &width, &height,
//&nrChannels, 0);
//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,
//height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//
//>> 다른 Texture 처럼 Wrap, Filter 설정 
//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//}
//
//
//    */
//
//    #pragma endregion
//
//    // transparent vegetation locations
//    // --------------------------------
//    std::vector<glm::vec3> vegetation{glm::vec3(-1.5f, 0.0f, -0.48f),
//                                 glm::vec3(1.5f, 0.0f, 0.51f),
//                                 glm::vec3(0.0f, 0.0f, 0.7f),
//                                 glm::vec3(-0.3f, 0.0f, -2.3f),
//                                 glm::vec3(0.5f, 0.0f, -0.6f)};
//
//    // 1) scene 을 new frame buffer 에 render 한다.
//    // 2) bind to default framebuffer
//    // 3) 전체 screen 크기의 quad 를 그린다.
//    // 단, new frame buffer 의 color buffer 를 texture 로 사용하여 그린다.
//
//    // render loop
//    // -----------
//    while (!glfwWindowShouldClose(window))
//    {
//        // per-frame time logic
//        // --------------------
//        float currentFrame = static_cast<float>(glfwGetTime());
//        deltaTime = currentFrame - lastFrame;
//        lastFrame = currentFrame;
//
//        // input
//        // -----
//        processInput(window);
//
//        glStencilMask(0x00); 
//
//        // (만약 new frame 을 사용하는 경우)
//        // render (NewFrame Buffer 에 먼저 그린다)
//        // bind to framebuffer and draw scene as we normally would to color texture
//        // glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
//        // glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
//
//        // render
//        // ------
//        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | 
//            GL_DEPTH_BUFFER_BIT |
//                GL_STENCIL_BUFFER_BIT);
//
//        glm::mat4 model = glm::mat4(1.0f);
//        glm::mat4 view = camera.GetViewMatrix();
//        glm::mat4 projection =
//            glm::perspective(glm::radians(camera.Zoom),
//                             (float)SCR_WIDTH / (float)SCR_HEIGHT,
//                             0.1f,
//                             100.0f);
//
//        shader.use();
//        shader.setVec3f("cameraPos", camera.Position);
//        shader.setMat4("view", view);
//        shader.setMat4("projection", projection);
//
//        simpleBlendingDiscard.use();
//        simpleBlendingDiscard.setMat4("view", view);
//        simpleBlendingDiscard.setMat4("projection", projection);
//
//        shaderSingleColor.use();
//        shaderSingleColor.setMat4("view", view);
//        shaderSingleColor.setMat4("projection", projection);
//
//        {
//            shader.use();
//
//            glBindVertexArray(cubeVAO);
//            glActiveTexture(GL_TEXTURE0);
//            glBindTexture(GL_TEXTURE_2D, cubeTexture);
//
//            // 1st container
//            model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
//            shader.setMat4("model", model);
//            glDrawArrays(GL_TRIANGLES, 0, 36);
//
//            // 2nd container
//            model = glm::mat4(1.0f);
//            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
//            shader.setMat4("model", model);
//            glDrawArrays(GL_TRIANGLES, 0, 36);
//
//            
//            glBindVertexArray(planeVAO);
//            glBindTexture(GL_TEXTURE_2D, floorTexture);
//            shader.setMat4("model", glm::mat4(1.0f));
//            glDrawArrays(GL_TRIANGLES, 0, 6);
//            glBindVertexArray(0);
//        }
//
//        {
//            // draw window
//            simpleBlendingDiscard.use();
//            glBindVertexArray(transparentVAO);
//            glBindTexture(GL_TEXTURE_2D, blendingTexture);
//            
//             // sort the transparent windows before rendering
//             // (카메라 기준, 뒤에 있는 것부터 그려내기 위함이다)
//            // ---------------------------------------------
//            std::map<float, glm::vec3> sorted;
//            for (unsigned int i = 0; i < vegetation.size(); i++)
//            {
//                float distance = glm::length(camera.Position - vegetation[i]);
//                sorted[distance] = vegetation[i];
//            }
//
//            for (std::map<float, glm::vec3>::reverse_iterator it =
//                     sorted.rbegin();
//                 it != sorted.rend();
//                 ++it)
//            {
//                model = glm::mat4(1.0f);
//                model = glm::translate(model, it->second);
//                simpleBlendingDiscard.setMat4("model", model);
//                glDrawArrays(GL_TRIANGLES, 0, 6);
//            }
//        }
//
//        
//        {
//            // 값이 같거나 작으면 pass 시킨다
//            // 자. 이게 없으면 아예 sky box 가 안그려진다.
//            // sky box vertex shader 코드 참고 : z 값이 '1' 이 된다.
//            glDepthFunc(GL_LEQUAL);
//        
//            // sky box 를 가장 마지막에 그린다.
//            skyBoxShader.use();
//        
//            // remove translation from the view matrix
//            // 왜 ? sky box 가 항상 player 를 중심위치로 두고 싶다
//            // 이에 따라 player 움직임과 관계없이 sky box 가 더 가까워지거나 하지
//            // 않기 때문이다. 단 회전으로 인한 변화는 적용하여 sky box 를 둘러볼 수 있도록 한다.
//            // 그 방법은 4x4 중에서 3x3 (L|t 에서 L 부분)만 취하면 된다.
//            view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
//        
//            skyBoxShader.setMat4("view", view);
//            skyBoxShader.setMat4("projection", projection);
//        
//            // skybox cube
//            glBindVertexArray(skyboxVAO);
//            glActiveTexture(GL_TEXTURE0);
//            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
//            glDrawArrays(GL_TRIANGLES, 0, 36);
//            glBindVertexArray(0);
//        
//            // set depth function back to default
//            glDepthFunc(GL_LESS);
//        }
//
//
//        // {
//        //     // default framebuffer 으로 다시 rendering
//        //     glBindFramebuffer(GL_FRAMEBUFFER, 0);
//        // 
//        //     // disable depth test so screen-space quad
//            //  isn't discarded due to depth test.
//        //     glDisable(
//        //         GL_DEPTH_TEST); 
//        // 
//        //     // clear all relevant buffers
//        //     glClearColor(
//        //         1.0f,
//        //         1.0f,
//        //         1.0f,
//        //         1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
//        //     glClear(GL_COLOR_BUFFER_BIT);
//        // 
//        //     screenShader.use();
//        // 
//        //     glBindVertexArray(quadVAO);
//        // 
//        //     glBindTexture(
//        //         GL_TEXTURE_2D,
//        //         textureColorbuffer); // use the color attachment texture as the texture of the quad plane
//        //     
//        //     glDrawArrays(GL_TRIANGLES, 0, 6);
//        // }
//
//        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//        // -------------------------------------------------------------------------------
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//     // optional: de-allocate all resources once they've outlived their purpose:
//    // ------------------------------------------------------------------------
//    glDeleteVertexArrays(1, &cubeVAO);
//    glDeleteVertexArrays(1, &planeVAO);
//    glDeleteVertexArrays(1, &quadVAO);
//    glDeleteVertexArrays(1, &skyboxVAO);
//
//    glDeleteBuffers(1, &cubeVBO);
//    glDeleteBuffers(1, &planeVBO);
//    glDeleteBuffers(1, &quadVBO);
//    glDeleteBuffers(1, &skyboxVBO);
//    
//    glDeleteRenderbuffers(1, &rbo);
//    glDeleteFramebuffers(1, &framebuffer);
//
//    glfwTerminate(); // 모든 glfw 자원 지운다.
//
//    return 0;
//}
//
//void framebuffer_size_callback(GLFWwindow *window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//}
//
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//    /*
//    1. Calculate the mouse’s offset since the last frame.
//    2. Add the offset values to the camera’s yaw and pitch values.
//    3. Add some constraints to the minimum/maximum pitch values.
//    4. Calculate the direction vector.
//    */
//
//    if (firstMouse) // initially set to true
//    {
//        lastMouseX = xpos;
//        lastMouseY = ypos;
//        firstMouse = false; // 맨 처음 window 창에 들어설 때 카메라 확 튀는 것 방지위함
//    }
//
//    float xoffset = xpos - lastMouseX; // calculate movement
//    float yoffset = lastMouseY - ypos; // reversed: y ranges bottom to top
//    lastMouseX = xpos;
//    lastMouseY = ypos;
//    const float sensitivity = 0.1f;
//    xoffset *= sensitivity;
//    yoffset *= sensitivity;
//
//    /*
//    yaw += xoffset; // yaw, pitch update
//    pitch += yoffset;
//
//    if (pitch > 89.0f)  // constrain to camera
//        pitch = 89.0f;
//    if (pitch < -89.0f)
//        pitch = -89.0f;
//
//    glm::vec3 direction; // calculate actual directon vector
//    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//    direction.y = sin(glm::radians(pitch));
//    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//    cameraFront = glm::normalize(direction);
//    */
//    camera.ProcessMouseMovement(xoffset, yoffset);
//}
//
//void processInput(GLFWwindow *window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//    const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
//
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//    {
//        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
//        // cameraPos += cameraSpeed * cameraFront; // 앞쪽 이동
//    }
//
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//    {
//        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
//        // cameraPos -= cameraSpeed * cameraFront; // 뒤쪽 이동
//    }
//
//    // glm::cross(cameraFront, cameraUp) : negative x 축 (left 방향 vector)
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//    {
//        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
//        // glm::vec3 crossVector = glm::cross(cameraFront, cameraUp); // 오른쪽
//        // cameraPos -= glm::normalize(crossVector) * cameraSpeed;
//    }
//
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//    {
//        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
//        // glm::vec3 crossVector = glm::cross(cameraFront, cameraUp);
//        // cameraPos += glm::normalize(crossVector) * cameraSpeed;
//    }
//}
//
//void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
//{
//    // float sensitivity = 0.05f;
//    // Zoom -= (float)yoffset * sensitivity;
//    // if (Zoom < 1.0f)
//    //     Zoom = 1.0f;
//    // if (Zoom > 45.0f)
//    //     Zoom = 45.0f;
//    // fov = Zoom;
//
//     camera.ProcessMouseScroll(static_cast<float>(yoffset));
//}
//
//// utility function for loading a 2D texture from file
//// ---------------------------------------------------
//unsigned int loadTexture(char const *path)
//{
//     unsigned int textureID;
//     glGenTextures(1, &textureID);
//
//     int width, height, nrComponents;
//     unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
//     if (data)
//     {
//        GLenum format;
//        if (nrComponents == 1)
//            format = GL_RED;
//        else if (nrComponents == 3)
//            format = GL_RGB;
//        else if (nrComponents == 4)
//            format = GL_RGBA;
//
//        glBindTexture(GL_TEXTURE_2D, textureID);
//        glTexImage2D(GL_TEXTURE_2D,
//                     0,
//                     format,
//                     width,
//                     height,
//                     0,
//                     format,
//                     GL_UNSIGNED_BYTE,
//                     data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D,
//                        GL_TEXTURE_MIN_FILTER,
//                        GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        stbi_image_free(data);
//     }
//     else
//     {
//        std::cout << "Texture failed to load at path: " << path << std::endl;
//        stbi_image_free(data);
//     }
//
//     return textureID;
//}
//
//unsigned int loadCubemap(std::vector<std::string> faces)
//{
//     unsigned int textureID;
//     glGenTextures(1, &textureID);
//     glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
//
//     int width, height, nrChannels;
//
//     for (unsigned int i = 0; i < faces.size(); i++)
//     {
//        unsigned char *data =
//            stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
//        if (data)
//        {
//            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
//                         0,
//                         GL_RGB,
//                         width,
//                         height,
//                         0,
//                         GL_RGB,
//                         GL_UNSIGNED_BYTE,
//                         data);
//            stbi_image_free(data);
//        }
//        else
//        {
//            std::cout << "Cubemap failed to load at path: " << faces[i]
//                      << std::endl;
//            stbi_image_free(data);
//        }
//     }
//
//     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//     return textureID;
//}