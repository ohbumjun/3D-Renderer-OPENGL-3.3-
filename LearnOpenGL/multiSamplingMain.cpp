#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "FileSystem.h"
#include "Model.h"
#include "Shader.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

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
    glfwSetScrollCallback(window, scroll_callback);

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

    glEnable(
        GL_MULTISAMPLE); // enabled by default on some drivers, but not all so always enable to make sure

    // build and compile shaders
    // -------------------------
    std::string vrxShaderPath = FileSystem::getPath("LearnOpenGL/AntiAliasingVS.glsl");
    std::string fragShaderPath = FileSystem::getPath("LearnOpenGL/AntiAliasingFS.glsl");
    Shader shader(vrxShaderPath.c_str(), fragShaderPath.c_str());

    vrxShaderPath =
        FileSystem::getPath("LearnOpenGL/AntiAliasingVS2.glsl");
    fragShaderPath =
        FileSystem::getPath("LearnOpenGL/AntiAliasingFS2.glsl");
    Shader aa2Shader(vrxShaderPath.c_str(), fragShaderPath.c_str());

    vrxShaderPath =
        FileSystem::getPath("LearnOpenGL/AntiAliasingPostVS.glsl");
    fragShaderPath =
        FileSystem::getPath("LearnOpenGL/AntiAliasingPostFS.glsl");
    Shader aaPostShader(vrxShaderPath.c_str(), fragShaderPath.c_str());


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
        0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f};
    // setup cube VAO
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

    // 현재 new frame buffer 를 사용 중
    // 화면 크기의 frame buffer 를 생성하고 싶다
    // 그러면 화면 크기의 mesh 에 어떤 값을 그리고자 하는 것이다.
    float quadVertices[] = {
        // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f, 1.0f, 0.0f, 1.0f,  -1.0f, -1.0f,
        0.0f,  0.0f, 1.0f, -1.0f, 1.0f,  0.0f,

        -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  -1.0f,
        1.0f,  0.0f, 1.0f, 1.0f,  1.0f,  1.0f};

    // setup screen VAO
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(quadVertices),
                 &quadVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          4 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          4 * sizeof(float),
                          (void *)(2 * sizeof(float)));

    // configure MSAA framebuffer
    // --------------------------
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer); // frame buffer 메모리를 생성
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);  // frame buffer 용으로 bind

    // create a multisampled color attachment texture
    unsigned int textureColorBufferMultiSampled;

    // texture 를 위한 texture objet 생성
    glGenTextures(1, &textureColorBufferMultiSampled);

    // GL_TEXTURE_2D_MULTISAMPLE : multi sampling 을 지원하는 
    //      형태로 bind
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 
        textureColorBufferMultiSampled);

    // 해당 texturre 에 실제 multi sampling 을 지원하는 texture 메모리 생성
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,
                            4, // num of samples
                            GL_RGB,
                            SCR_WIDTH,
                            SCR_HEIGHT,
                            GL_TRUE // image 가 각 texel 마다 같은 locatoin, number 의 subsamle 사용(??)
    );

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    // 새로 생성한 frame buffer 에 multisampled texture 를 binding 한다.
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D_MULTISAMPLE,
                           textureColorBufferMultiSampled,
                           0);

    // create a (also multisampled) renderbuffer object for depth and stencil attachments
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo); // render buffer object 생성

    // multisampling 을 지원하는 depth buffer(24) + stencil(8) rbo 생성
    glRenderbufferStorageMultisample(GL_RENDERBUFFER,
                                     4,
                                     GL_DEPTH24_STENCIL8,
                                     SCR_WIDTH,
                                     SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // 해당 rbo 를 새로운 frame buffer 에 binding
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER,
                              rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // configure second post-processing framebuffer
    // 자. 앞서 생성한 frame buffer 는 멀티 샘플링을 지원
    // 해당 frame buffer 에 multi sample 형태로 그린 이후에
    // 아래 또 새롭게 생성하는 frame buffer 에 복사하는 형태로 할 것이다.
    // 즉, 추가 frame buffer 를 2개 생성하고자 하는 것이다.
    unsigned int intermediateFBO;
    glGenFramebuffers(1, &intermediateFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

    // create a color attachment texture
    unsigned int screenTexture;
    glGenTextures(1, &screenTexture);
    glBindTexture(GL_TEXTURE_2D, screenTexture);

    // 위에 생성한 frame buffer 와 달리 multi sampling 을 지원하지 않는 texture 를 생성
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 SCR_WIDTH,
                 SCR_HEIGHT,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D,
                           screenTexture,
                           0); // we only need a color buffer

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!"
             << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    aaPostShader.use();
    aaPostShader.setInt("screenTexture", 0);

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

        
        // 1. draw scene as normal to  multisampled buffers (new frame buffer)
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // set transformation matrices
        aa2Shader.use();
        glm::mat4 projection =
            glm::perspective(glm::radians(camera.Zoom),
                             (float)SCR_WIDTH / (float)SCR_HEIGHT,
                             0.1f,
                             1000.0f);
        aa2Shader.setMat4("projection", projection);
        aa2Shader.setMat4("view", camera.GetViewMatrix());
        aa2Shader.setMat4("model", glm::mat4(1.0f));

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 2. now blit multisampled buffer(s) to 
        // normal colorbuffer of intermediate FBO. (2nd frame buffer)
        // Image is stored in screenTexture
        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
        glBlitFramebuffer(0,
                          0,
                          SCR_WIDTH,
                          SCR_HEIGHT,
                          0,
                          0,
                          SCR_WIDTH,
                          SCR_HEIGHT,
                          GL_COLOR_BUFFER_BIT, // color buffer 측에 내용 복사
                          GL_NEAREST);

        // 3. now render quad with scene's visuals as its texture image
        // 자. 이제 default frame buffer 에 그리고자 하는 것이다.
        // 위에서 복사해준 screen 크기의 frame buffer 를 screen size 에 맞게 그린다.
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // // disable depth test so screen-space quad isn't discarded due to depth test.
        // 자. 이제 화면 전체를 덮을 quad 를 그리고자 하는 것이다
        // 만약 depth test 가 enabler 되면, depth  buffer 에 이미 존재하는
        // 값과 비교가 될 것이다. 만약 default frame buffer 에 이미 어떤 내용이
        // draw 가 되어 있었고 해당 pixel 에서의 z 값이 depth buffer 에 존재한다면
        // 현재 quad 를 그릴 때, 해당 pixel 은 그려지지 않을 것이다.
        glDisable(GL_DEPTH_TEST); 

        // draw Screen quad
        aaPostShader.use();
        glBindVertexArray(quadVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(
            GL_TEXTURE_2D,
            screenTexture); // use the now resolved color attachment as the quad's texture
        
        glDrawArrays(GL_TRIANGLES, 0, 6); // 사각형 그린다(2개의 삼각형)

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
