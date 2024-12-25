#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "FileSystem.h"
#include <iostream>
#include <stb_image.h>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*
* >> GLFW ? GLAD ?
penGL의 specification을 OS에 상관없이 
쉽게 implementation하게 도와주는 것이 GLFW와 GLAD

>> GLFW ?
LFW는 OpenGL 컨텍스트를 사용하여 창을 만들고 관리 할 수있는 작은 C 라이브러리

>> GLAD ?
 GLAD는 흩어진 function의 위치를 찾아 함수포인터를 가져올 때에 
 도움을 준다는 점
*/

/*
* window 창이 resize 될 때마다 호출되는 콜백 함수
*/
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
unsigned int loadTexture(const char *path);

// // Camera 의 World Pos
// glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
// // Camer Direction
// glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
// // glm::cross 를 통해서 Camera Right 을 얻기 위한 up vector
// glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
// float yaw = -90.0f;
// float pitch = 0.0f;
// float Zoom = 45.f;
// float fov = 45.f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float deltaTime = 0.0f; // Time between current frame and last frame

float lastFrame = 0.0f; // Time of last frame


float lastMouseX = 0.f, lastMouseY = 0.f; // store last pos

bool firstMouse = true;

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

size_t vertexInfoSize = 8; //

glm::vec3 dirLightDir(-0.2f, -1.f, -03.f);
glm::vec3 dirLightPos(0.f, 5.f, 0.f);

int main(int argc, char *argv[])
{
    #pragma region OpenGL 초기화
    // GLFW 초기화
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLAD 초기화
    // GLAD manages function pointers for OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell OpenGL the size of the rendering window
    // 0,0 : lower left corner
    // 800, 600 : width, height
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // 콜백함수 등록
    // > window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // > Mouse
    glfwSetCursorPosCallback(window, mouse_callback);
    // > Scroll
    glfwSetScrollCallback(window, scroll_callback);

    // vertex attribute 의 최대 개수
    // - 보통 vertex shader 의 input 을 얘기할 때
    // 'vertex attribute' 이라고 한다.
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes
              << std::endl;

    #pragma endregion

    struct point
    {
        int x;
        int y;
    };

    #pragma region Shader Setting

    // shader
    std::string vrxShaderPath =
        "D:\\OpenGL\\LearnOpenGL\\LearnOpenGLSrc\\LearnOpenGL\\DepthTestingVertex.glsl";

    std::string fragShaderPath =
        "D:\\OpenGL\\LearnOpenGL\\LearnOpenGLSrc\\LearnOpenGL\\DepthTestingFrag.glsl";

    /*
    * >> Light 적용 없는 Shader
    */
    Shader shader(vrxShaderPath.c_str(), fragShaderPath.c_str());
    

    #pragma endregion
    
    #pragma region Vertex
    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};
    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
        5.0f, -0.5f, 5.0f,  2.0f,  0.0f,  -5.0f, -0.5f, 5.0f,
        0.0f, 0.0f,  -5.0f, -0.5f, -5.0f, 0.0f,  2.0f,

        5.0f, -0.5f, 5.0f,  2.0f,  0.0f,  -5.0f, -0.5f, -5.0f,
        0.0f, 2.0f,  5.0f,  -0.5f, -5.0f, 2.0f,  2.0f};
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
                          5 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glBindVertexArray(0);
    // plane VAO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(planeVertices),
                 &planeVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          5 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glBindVertexArray(0);

    #pragma endregion

    #pragma region Texture

    // load textures
    // -------------
    unsigned int cubeTexture = loadTexture(
        FileSystem::getPath("BJResource/marble.jpg").c_str());
    unsigned int floorTexture = loadTexture(
        FileSystem::getPath("BJResource/metal.png").c_str());
    #pragma endregion
    // glUseProgram(shaderProgram); // don’t forget to activate the shader first!
    shader.use();

    // uniform sampler2D texture1 에 세팅
    // glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0); 
    shader.setInt("texture1", 0); // or with shader class

    #pragma region DEPTH TEST
    // enable depth test
    glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_ALWAYS); // always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
    #pragma endregion

    #pragma region Camera

    /*
    // Camera 의 World Pos
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

    // Camer Direction
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

    // Camera Dir : nagative z 축 방향 (오른손 좌표계)
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

    // 아래의 camera right 을 얻기 위해서 up 을 정의 (일종의 trick)
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    // positive x- axis
    // 참고 : 만약 negative x- axis 를 원한다면, cross 순서를 변경 ex) glm::cross(cameraDirection, up)
    // ex) https://www.youtube.com/watch?v=eu6i7WJeinw 5:55
    // ex) https://stackoverflow.com/questions/37519476/how-do-you-use-the-right-hand-rule-for-glmcross
    // ex) glm::cross(x,y) = z
    // ex) glm::cross(2번째 손가락, 3번째 손가락) = 엄지 손가락 (이것을 기준으로 생각)
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

    // positive y- axis
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight); // y

    // LookAt matrix
    // 3개의 view / camera space vector 를 정의했다.
    // 해당 matrix 들을 활용하여 LookAt matrix 를 만들 수 있다.
    // 방법
    // - 1) camera space origin 을 원점으로 이동시키는 translation matrix
    //  ex) 현재 camera pos 가 (0,0,3) 이라면, -cameraPos 만큼 이동시키는 matrix
    // - 2) camera space 의 axes 를 world space 의 axes 로 mapping 시키는 rotation matrix
    //  ex) right, up, direction 을 row 로 하는 matrix !
    // 이를 통해 만들어지는 LookAt matrix 는 모든 world 좌표계를 camera 좌표계로 이동시킨다.
    glm::mat4 view;
    
    // sample
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), // Camera Pos
                       glm::vec3(0.0f, 0.0f, 0.0f), // Camera Target Pos
                       glm::vec3(0.0f, 1.0f, 0.0f)); // Camera Right 을 계산하기 위해 사용한 'up'
    */

    #pragma endregion

    #pragma region Zoom

    /*
    * FOV (field of view) 가 Scene 에서 우리가 얼만큼 볼 수 있는지를 결정한다.
    * fov 값이 작아지면, Scene 에 project 된 (투영된) 공간의 크기는 작아진다.
    * 같은 NDC 공간에, 더 작은 공간이 투영되는 것이다.
    * 이를 통해 zoom in 하는 것처럼 보이게 할 수 있다.
    */

    #pragma endregion

    #pragma region Euler Angle
    /*
    * >> Euler Angle (p101)
    * - 흔히 
    1) pitch (x축 기준 회전 : 위,아래 ) 
    2) yaw (y축 기준 회전 : 좌,우)
    3) roll (z축 기준 회전 : 알아서 상상하삼) 을 사용하여 카메라를 회전시키는 방법
    * - 각 축 ? 에서의 회전을 "combine" 시킨 하나의 single value 를 의미하게 된다.
    * 
    * Pitch, Yaw 만 일단 고려해본다.
    * Pitch 와 Yaw 가 주어지면 새로운 direction 3d vector 를 얻을 수 있다.
    * 
    * >> Yaw 기준 회전 
    * - xz 평명
    * - 각도 : 반시계 방향 양수
    * - x 축 : cos 각도 , z 축 : sin 각도

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)); // convert to radians first
    direction.z = sin(glm::radians(yaw));

    * >> Pitch 기준 회전
    * - yz 평면
    * - 각도 : 위로 양수
    * - y 축 : sin 각도 , z 축 : cos 각도
    direction.y = sin(glm::radians(pitch));

    >> 종합
    - 그런데 x,z 평면은 cos (pitch) 에 의해 영향을 받는다.

    (부가 설명)
    >> 삼각함수의 성질:
    직각삼각형에서 빗변의 길이가 1일 때, 
    한 예각의 크기가 θ라면 빗변에 인접한 변의 길이는 cos(θ), 
    빗변에 대변하는 변의 길이는 sin(θ)가 됩니다.

    >> 피치 각과의 관계:
    피치 각이 변함에 따라, 원래 x축과 z축 방향을 가리키던 벡터가 
    x-z 평면 상에서 기울어지면서 새로운 직각삼각형이 형성됩니다.

    이 새로운 직각삼각형에서 빗변의 길이는 변하지 않지만, 
    빗변에 인접한 변의 길이(즉, x축과 z축 성분)는 cos(pitch)만큼 줄어들게 됩니다.

    따라서, 원래 x축과 z축 성분에 cos(pitch)를 곱해주어야 
    새로운 직각삼각형에서의 x축과 z축 성분을 정확하게 계산할 수 있습니다.

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));


    >> 카메라 방향 조절 : yaw = -90.0f;
    - 위 원리대로 한다면, 카메라는 맨 처음에 양의 x 축 방향을 향하게 된다.
    - 그런데 default 로 z 음의 방향을 바라보게 하고 싶다.
    - yaw 가 0 이면, 양의 x 축 방향을 가리키므로, 시계 방향 90도 회전을 시키면 된다.
    - 양의 각도는 반 시계 방향을 가리키므로, yaw : -90 도

    자. 여기서 의문일 수 있다. 아니 오른손 좌표계에서 y 축 기준 시계방향
    회전시키면 x 축은 z 축 양의 방향을 가리키는 거 아닌가 ?
    왜 그런데 저기서는 z 축 '음' 의 방향을 가라킨다고 얘기하는 거지 ?

    여기서 말하는 yaw, pitch 는 오른손 좌표계랑 개념이 아예 다르다.
    오른쪽 x, 위 y, 앞쪽이 z (즉, z 축이 opengl 좌표계랑 반대)

    */
    #pragma endregion

    #pragma region MouseInput

    /*
    * >> Hide the cursor & Capture the cursor
    - Hide the cursor : 커서가 화면상에 안보이게 해서 player 를 방해하지 않도록 한다.
    - Capturing Cursor : cursor 의 움직임이 window 창 안에 있을 때만 영향 받도록 한다.
            설령 window 창 밖에서 움직여도, cursor 은 window center 쪽으로 이동하게 된다.
    */
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    #pragma endregion

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

        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection =
            glm::perspective(glm::radians(camera.Zoom),
                             (float)SCR_WIDTH / (float)SCR_HEIGHT,
                             0.1f,
                             100.0f);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // floor
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

     // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);

    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);

    glfwTerminate(); // 모든 glfw 자원 지운다.

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    /*
    1. Calculate the mouse’s offset since the last frame.
    2. Add the offset values to the camera’s yaw and pitch values.
    3. Add some constraints to the minimum/maximum pitch values.
    4. Calculate the direction vector.
    */

    if (firstMouse) // initially set to true
    {
        lastMouseX = xpos;
        lastMouseY = ypos;
        firstMouse = false; // 맨 처음 window 창에 들어설 때 카메라 확 튀는 것 방지위함
    }

    float xoffset = xpos - lastMouseX; // calculate movement
    float yoffset = lastMouseY - ypos; // reversed: y ranges bottom to top
    lastMouseX = xpos;
    lastMouseY = ypos;
    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    /*
    yaw += xoffset; // yaw, pitch update
    pitch += yoffset;

    if (pitch > 89.0f)  // constrain to camera
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction; // calculate actual directon vector
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
    */
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
        // cameraPos += cameraSpeed * cameraFront; // 앞쪽 이동
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
        // cameraPos -= cameraSpeed * cameraFront; // 뒤쪽 이동
    }

    // glm::cross(cameraFront, cameraUp) : negative x 축 (left 방향 vector)
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
        // glm::vec3 crossVector = glm::cross(cameraFront, cameraUp); // 오른쪽
        // cameraPos -= glm::normalize(crossVector) * cameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
        // glm::vec3 crossVector = glm::cross(cameraFront, cameraUp);
        // cameraPos += glm::normalize(crossVector) * cameraSpeed;
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    // float sensitivity = 0.05f;
    // Zoom -= (float)yoffset * sensitivity;
    // if (Zoom < 1.0f)
    //     Zoom = 1.0f;
    // if (Zoom > 45.0f)
    //     Zoom = 45.0f;
    // fov = Zoom;

     camera.ProcessMouseScroll(static_cast<float>(yoffset));
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