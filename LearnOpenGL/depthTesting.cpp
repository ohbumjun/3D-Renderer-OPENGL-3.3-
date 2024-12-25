//#include <GLFW/glfw3.h>
//#include <glad/glad.h>
//#include "FileSystem.h"
//#include <iostream>
//#include <stb_image.h>
//#include "Shader.h"
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
//
//// // Camera 의 World Pos
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
//
//    std::string fragShaderPath =
//        "D:\\OpenGL\\LearnOpenGL\\LearnOpenGLSrc\\LearnOpenGL\\DepthTestingFrag.glsl";
//
//    /*
//    * >> Light 적용 없는 Shader
//    */
//    Shader depthTestingShader(vrxShaderPath.c_str(), fragShaderPath.c_str());
//    
//
//    #pragma endregion
//    
//    #pragma region Vertex
//    unsigned int cubeVAO;
//    /*
//        * VBO 란, vertex data 를 담은 opengl 상의 메모리이다.
//        */
//    unsigned int VBO;
//
//    {
//        float texCoords[] = {
//            0.0f,
//            0.0f, // lower-left corner
//            1.0f,
//            0.0f, // lower-right corner
//            0.5f,
//            1.0f // top-center corner
//        };
//
//        // vetex  + color
//        // float vertices[] = {
//        //     // positions        // colors
//        //     0.5f,-0.5f,0.0f,    1.0f,0.0f,0.0f, // bottom right
//        //     -0.5f,
//        //     -0.5f,
//        //     0.0f,
//        //     0.0f,
//        //     1.0f,
//        //     0.0f, // bottom left
//        //     0.0f,
//        //     0.5f,
//        //     0.0f,
//        //     0.0f,
//        //     0.0f,
//        //     1.0f // top
//        // };
//
//        // 4 point Rectangle
//        // -> glDrawElements 에서 6 개의 정점을 넘겨주면 된다.
//        // -> 2개의 삼각형. 총 6개의 정점
//        // float vertices[] = {
//        //     // positions // colors // texture coords
//        //     0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
//        //     0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
//        //     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
//        //     -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
//        // };
//
//        // 6 개의 면, 36 개의 vertices
//        // ex) glDrawArrays(GL_TRIANGLES, 0, 36);
//        // float vertices[] = {
//        //     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
//        //     0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
//        //     0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 
//        //     0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
//        // 
//        //     -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 
//        //     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
//        //     -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 
//        //     0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
//        // 
//        //     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 
//        //     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//        //     -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, 
//        //     -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,
//        // 
//        //     -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, 
//        //     -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
//        //     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
//        //     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
//        // 
//        //     -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 
//        //     -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,
//        //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 
//        //     0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
//        // 
//        //     0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 
//        //     0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
//        //     0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 
//        //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//        // 
//        //     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
//        //     0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
//        //     0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 
//        //     0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
//        // 
//        //     -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 
//        //     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
//        //     -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 
//        //     0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
//        // 
//        //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 
//        //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//        //     -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, 
//        //     -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f
//        // };
//
//       // float vertices[] = {
//       //     // positions             // normals 
//       //     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//       //      0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//       //      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//       //      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//       //     -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//       //     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//       // 
//       //     -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//       //      0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//       //      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//       //      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//       //     -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//       //     -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//       // 
//       //     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//       //     -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//       //     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//       //     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//       //     -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//       //     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//       // 
//       //      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//       //      0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//       //      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//       //      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//       //      0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//       //      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//       // 
//       //     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//       //      0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//       //      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//       //      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//       //     -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//       //     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//       // 
//       //     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//       //      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//       //      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//       //      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//       //     -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//       //     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
//       // };
//
//        float vertices[] = {
//            // positions          // normals           // texture coords
//            -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,
//            0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f,
//            0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f,
//            0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f,
//            -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 1.0f,
//            -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,
//
//            -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
//            0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
//            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
//            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
//            -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
//            -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
//
//            -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,
//            -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 1.0f,
//            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
//            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
//            -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 0.0f,
//            -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,
//
//            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
//            0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
//            0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
//            0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
//            0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
//            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
//
//            -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f,
//            0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f, 1.0f,
//            0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f,
//            0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f,
//            -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f, 0.0f,
//            -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f,
//
//            -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
//            0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
//            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
//            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
//            -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
//            -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f
//        };
//
//        unsigned int indices[] = {
//            // note that we start from 0!
//            0,
//            1,
//            3, // first triangle
//            1,
//            2,
//            3 // second triangle
//        };
//
//        /*
//        * VAO
//        * - 속성 ~ VBO configure 정보를 한번에 담는 object
//        * - 이것만 bind 시키기만 하면 된다.
//        * - 뿐만 아니라, EBO 정보도 같이 가지고 있는다.
//        */
//        glGenVertexArrays(1, &cubeVAO);
//
//        // VAO 를 먼저 bind 시킨다.
//        // 그 다음에 비로소 vbo, 속성을 configure 한다.
//        glBindVertexArray(cubeVAO);
//
//        /*
//        * indexed buffer 를 위한 object :GL_ELEMENT_ARRAY_BUFFER
//        */
//        unsigned int EBO;
//        glGenBuffers(1, &EBO);
//
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
//                     sizeof(indices),
//                     indices,
//                     GL_STATIC_DRAW);
//
//        glGenBuffers(1, &VBO);
//
//        // Opengl 은 여러 type 의 buffer object 를 가지고 있다.
//        // 그리고 'vetex buffer object' 의 type 이 GL_ARRAY_BUFFER 이다.
//        // opengl 은, 서로 다른 type의 여러 개 buffer 를 bind 할 수 있게 해준다.
//        glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//        // 해당 VBO 메모리 공간에, 현재 vertices 의 데이터를 복사한다.
//        glBufferData(GL_ARRAY_BUFFER,
//                     sizeof(vertices),
//                     vertices,
//                     /*
//                    * 그래픽 카드가 우리가 제공한 데이터를 어떤식으로 다룰지를 결정한다.
//                    * 
//                    * > GL_STATIC_DRAW : data 는 한번 set, 여러 번 사용됨
//                    * > GL_DYNAMIC_DRAW : data 는 여러 번 set, 여러 번 사용됨
//                    * > GL_STREAM_DRAW : data 는 한번 set, 정말 적게 사용됨
//                    * 
//                    * 우리가 현재 넘겨주는 정점 정보는 , 변하지 않고, 여러번 사용된다.
//                    * 만약 자주 변하는 정보라면 GL_DYNAMIC_DRAW 을 사용하여
//                    * Opengl 이 빠르게 쓸 수 있는 메모리 공간으로 데이터를 위치시킬 수 있게 
//                    * 해야 한다.
//                    */
//                     GL_STATIC_DRAW);
//
//        // shader 을 준비하는 것만으로는 부족하다
//        // vertex shader 는, 아직 메모리 상의 vertex data 를
//        // 어떻게 해석해야 할지 모른다. 따라서 vertex attribute 을
//        // 통해서 이를 알려주어야 한다.
//        glVertexAttribPointer(
//            0, // 어떤 vertex attribute 을 configure 하는 것인가 // ex) layout(location = 0) in vec3 aPos;
//            3,                 // vertex attribute 의 크기
//            GL_FLOAT,          // 데이터의 type
//            GL_FALSE,          // 정규화 여부
//            vertexInfoSize * sizeof(float), // 각 정점 데이터 사이의 간격
//            (void *)0 // 메모리 상에서 data 가 시작하는 offset
//        );
//
//        // 해당 속성을 enable 시켜야 한다.
//        // vertex 속성들은, 기본적으로 disable 되어 있다.
//        glEnableVertexAttribArray(0);
//
//        // color attribute
//        // glVertexAttribPointer(1,
//        //                       3,
//        //                       GL_FLOAT,
//        //                       GL_FALSE,
//        //                       8 * sizeof(float),
//        //     // color 정보는, vertex 정보 다음에 위치하므로 아래와 같은 offset 세팅
//        //                       (void *)(3 * sizeof(float)));
//        // glEnableVertexAttribArray(1);
//        glVertexAttribPointer(1,
//                              3,
//                              GL_FLOAT,
//                              GL_FALSE,
//                              vertexInfoSize * sizeof(float),
//                              (void *)(3 * sizeof(float)));
//
//        glEnableVertexAttribArray(1);
//
//       glVertexAttribPointer(2,
//                              2,
//                              GL_FLOAT,
//                              GL_FALSE,
//                              vertexInfoSize * sizeof(float),
//                              (void *)(6 * sizeof(float)));
//
//        glEnableVertexAttribArray(2);
//
//        // glBindBuffer(GL_ARRAY_BUFFER, 0);
//        glBindVertexArray(0); // VAO unbind 시키기
//    }   
//    #pragma endregion
//
//    #pragma region Light Source Shader Setting
//    // 실제 광원에 해당하는 object 를 위한  VAO, attribute 생성
//    // 광원을 기존의 VAO 에 설정하여 그릴 수 있다. 하지만 이후
//    // vertex data 와 attribute 를 자주 변경할 것이고
//    // 이러한 변화들이 광원 물체에 영향 주지 않게 하기 위해서
//    // 별도의 VAO 를 설정할 것이다.
//    unsigned int lightSourceVAO;
//    glGenVertexArrays(1, &lightSourceVAO);
//    glBindVertexArray(lightSourceVAO);
//    // we only need to bind to the VBO, the container’s VBO’s data
//    // already contains the data.
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    // set the vertex attribute
//    glVertexAttribPointer(0,
//                          3,
//                          GL_FLOAT,
//                          GL_FALSE,
//                          // 3 * sizeof(float),
//                          vertexInfoSize * sizeof(float),
//                          (void *)0);
//    glEnableVertexAttribArray(0);
//
//    #pragma endregion
//
//    #pragma region Texture
//    int width, height, nrChannels;
//    
//    // OPENGL 은 Texture Coord 가 아래->위 방향으로 증가한다고 계산
//    // 하지만 stbl 은 위에서 아래 방향으로 증가한다고 계산
//    // 따라서 그 값들을 뒤집어 줘야 한다.
//    stbi_set_flip_vertically_on_load(1);
//
//    unsigned char *data = stbi_load(RESOURCE_ROOT "SampleImage.jpg",
//                                    &width,
//                                    &height,
//                                    &nrChannels,
//                                    0);
//
//    // index buffer, vertex buffer 처럼 마찬가지로 texture object 를 생성한다.
//    unsigned int texture1; // unsigned int 배열이 될 수도 있다. 여러 개의 texture 담을 경우
//    glGenTextures(
//        1, // 만들고자 하는 texture 들의 개수
//        &texture1);
//
//    // texture 를 bind 시켜서
//    // 이후의 subsequent texture 관련 작업이 이 texture 에 대해 이루어지게 한다.
//    glBindTexture(GL_TEXTURE_2D, texture1);
//
//    // 이제 실제 texture 메모리를 할당한다.
//    // 해당 함수 까지 호출하면, texture object 에 texture image 가 실제 attach 된 것
//    // 하지만, 여기까지만 하면 texture image 의 base-level 만 존재하는 것
//    glTexImage2D(
//        GL_TEXTURE_2D, // texture target
//                 0, // mipmap level
//                 GL_RGB, // format
//                 width,
//                 height,
//                 0, // 항상 0
//                 GL_RGB, // source image 의 format
//                 GL_UNSIGNED_BYTE,
//                 data);
//
//    // Set texture wrapping mode to GL_CLAMP_TO_EDGE
//    // wrapping_mode 란, texture 좌표 가 넘어갔을 때 어떻게 처리할지를 
//    // GL_CLAMP_TO_EDGE : 0 보다 작거나, 1 보다 큰 값은 가장 자리 픽셀의 색상으로 채워진다.
//    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//
//    // set texture filtering parameters
//    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    // mipmap 을 생성한다.
//    // 현재 bind 된 texture 에 대해 mipmap 을 생성한다.
//    glGenerateMipmap(GL_TEXTURE_2D);
//
//    unsigned char *data2 = stbi_load(RESOURCE_ROOT "curious.png",
//                                    &width,
//                                    &height,
//                                    &nrChannels,
//                                    0);
//
//    // index buffer, vertex buffer 처럼 마찬가지로 texture object 를 생성한다.
//    unsigned int
//        texture2; // unsigned int 배열이 될 수도 있다. 여러 개의 texture 담을 경우
//    glGenTextures(1, // 만들고자 하는 texture 들의 개수
//                  &texture2);
//
//    // texture 를 bind 시켜서
//    // 이후의 subsequent texture 관련 작업이 이 texture 에 대해 이루어지게 한다.
//    glBindTexture(GL_TEXTURE_2D, texture2);
//
//    // 이제 실제 texture 메모리를 할당한다.
//    // 해당 함수 까지 호출하면, texture object 에 texture image 가 실제 attach 된 것
//    // 하지만, 여기까지만 하면 texture image 의 base-level 만 존재하는 것
//    glTexImage2D(GL_TEXTURE_2D, // texture target
//                 0,             // mipmap level
//                 GL_RGB,        // format
//                 width,
//                 height,
//                 0,      // 항상 0
//                 GL_RGBA, // source image 의 format -> png 은 alpha channel 이 있으므로
//                 GL_UNSIGNED_BYTE,
//                 data2);
//
//    // Set texture wrapping mode to GL_CLAMP_TO_EDGE
//    // wrapping_mode 란, texture 좌표 가 넘어갔을 때 어떻게 처리할지를
//    // GL_CLAMP_TO_EDGE : 0 보다 작거나, 1 보다 큰 값은 가장 자리 픽셀의 색상으로 채워진다.
//    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//
//    // set texture filtering parameters
//    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    // mipmap 을 생성한다.
//    // 현재 bind 된 texture 에 대해 mipmap 을 생성한다.
//    glGenerateMipmap(GL_TEXTURE_2D);
//
//    stbi_image_free(data);
//    stbi_image_free(data2);
//
//    #pragma endregion
//
//    #pragma region Texture Unit
//
//    // glUseProgram(shaderProgram); // don’t forget to activate the shader first!
//    depthTestingShader.use();
//
//    // uniform sampler2D texture1 에 세팅
//    // glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0); 
//    depthTestingShader.setInt("texture1", 0); // or with shader class
//
//    // glUniform1i(glGetUniformLocation(ourShader.ID, "texture2"),1); // or with shader class
//    depthTestingShader.setInt("texture2", 1); // or with shader class
//
//    #pragma endregion
//
//    #pragma region DEPTH TEST
//    // enable depth test
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(
//        GL_ALWAYS); // always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
//    #pragma endregion
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
//    while (!glfwWindowShouldClose(window))
//    {
//        float currentFrame = glfwGetTime();
//        deltaTime = currentFrame - lastFrame;
//        lastFrame = currentFrame;
//
//        processInput(window);
//
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        // glClear(GL_COLOR_BUFFER_BIT); // clear the color buffer
//        // glClear(GL_DEPTH_BUFFER_BIT); // clear the depth buffer
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear both color, depth
//
//        float timeValue = glfwGetTime();
//        float mixValue = (sin(timeValue) / 2.0f) + 0.5f; //0  ~ 1
//
//        // model : local space -> world space 로 변환
//        glm::mat4 model = glm::mat4(1.0f); 
//        glm::mat4 view;
//        glm::mat4 projection;
//
//       model = glm::translate(
//             model,
//             glm::vec3(
//                 0.0f,
//                 0.0f,
//                 0.0f)); // translate it down so it's at the center of the scene
//         model = glm::scale(
//             model,
//             glm::vec3(
//                 1.0f,
//                 1.0f,
//                 1.0f)); // it's a bit too big for our scene, so scale it down
//        
//         depthTestingShader.setMat4("model", model);
//         depthTestingShader.setMat4("view", view);
//         depthTestingShader.setMat4("projection", projection);
//
//        depthTestingShader.use();
//
//        // wire frame mode 로 그리기
//        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : wire frame 해제
//
//        // indexed 형태로 그리는 함수
//        // glDrawElements(GL_TRIANGLES,
//        //     36, // 그리고 싶은 element 의 개수 (ex. 6개 정점을 그리고 싶다)
//        //     GL_UNSIGNED_INT,  // type of indices
//        //     0 // offset in ebo
//        // );
//
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//        // glDrawArrays(GL_TRIANGLES, 0, 36);
//
//        // // check and call events and swap the buffers
//        glfwSwapBuffers(window); // double buffering
//        glfwPollEvents(); // event 가 발생했는지 계속해서 검사
//    }
//
//     // optional: de-allocate all resources once they've outlived their purpose:
//    // ------------------------------------------------------------------------
//    glDeleteVertexArrays(1, &cubeVAO);
//    glDeleteVertexArrays(1, &lightSourceVAO);
//    glDeleteBuffers(1, &VBO);
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
