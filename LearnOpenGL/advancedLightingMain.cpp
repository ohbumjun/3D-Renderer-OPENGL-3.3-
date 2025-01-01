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
unsigned int loadTexture(const char *path, bool gammaCorrection);
void renderScene(const Shader &shader);
void renderCube();
void renderQuad();
void renderGamma(Shader &gammaCorrectionShader,
                 unsigned int floorTexture,
                 unsigned int floorTextureGmaCorrected);

    // settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Blinn
bool blinn = false;
bool blinnKeyPressed = false;

// Gamma Correction
bool gammaEnabled = false;
bool gammaKeyPressed = false;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// meshes
unsigned int planeVAO;

// gamma lighting info
glm::vec3 lightPositions[] = {glm::vec3(-3.0f, 0.0f, 0.0f),
                              glm::vec3(-1.0f, 0.0f, 0.0f),
                              glm::vec3(1.0f, 0.0f, 0.0f),
                              glm::vec3(3.0f, 0.0f, 0.0f)};
glm::vec3 lightColors[] = {glm::vec3(0.25),
                           glm::vec3(0.50),
                           glm::vec3(0.75),
                           glm::vec3(1.00)};

glm::vec3 depthMapDirectionalLightPos(-2.0f, 4.0f, -1.0f);

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // build and compile shaders
    // -------------------------
    std::string vrxShaderPath =
        FileSystem::getPath("LearnOpenGL/BlihnModelVS.glsl");
    std::string fragShaderPath =
        FileSystem::getPath("LearnOpenGL/BlihnModelFS.glsl");
    
    Shader BlihnModelOnlyShader(vrxShaderPath.c_str(), fragShaderPath.c_str());

    vrxShaderPath =
        FileSystem::getPath("LearnOpenGL/GammaCorrectionVS.glsl");
    fragShaderPath =
        FileSystem::getPath("LearnOpenGL/GammaCorrectionFS.glsl");

    Shader gammaCorrectionShader(vrxShaderPath.c_str(), fragShaderPath.c_str());

    vrxShaderPath = 
        FileSystem::getPath("LearnOpenGL/ShadowDubugVS.glsl");
    fragShaderPath = 
        FileSystem::getPath("LearnOpenGL/ShadowDubugFS.glsl");

    Shader simpleDebugShader(vrxShaderPath.c_str(), fragShaderPath.c_str());
    
    vrxShaderPath = 
        FileSystem::getPath("LearnOpenGL/ShadowMapDepthVS.glsl");
    fragShaderPath = 
        FileSystem::getPath("LearnOpenGL/ShadowMapDepthFS.glsl");

    Shader depthMapShader(vrxShaderPath.c_str(), fragShaderPath.c_str());

    vrxShaderPath = FileSystem::getPath("LearnOpenGL/ShadowMapVS.glsl");
    fragShaderPath = FileSystem::getPath("LearnOpenGL/ShadowMapFS.glsl");

    Shader shadowMapShader(vrxShaderPath.c_str(), fragShaderPath.c_str());

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float planeVertices[] = {
        // positions            // normals         // texcoords
        10.0f,  -0.5f, 10.0f,  0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
        -10.0f, -0.5f, 10.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f,
        -10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f,  10.0f,

        10.0f,  -0.5f, 10.0f,  0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
        -10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f,  10.0f,
        10.0f,  -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f};

    // plane VAO
    unsigned int planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(planeVertices),
                 planeVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          8 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          8 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          8 * sizeof(float),
                          (void *)(6 * sizeof(float)));
    glBindVertexArray(0);

    // load textures
    // -------------
    unsigned int floorTexture =
        loadTexture(FileSystem::getPath("BJResource/wood.png").c_str(), false);
    unsigned int floorTextureGmaCorrected =
        loadTexture(FileSystem::getPath("BJResource/wood.png").c_str(), true);


    // configure depth map FBO
    // -----------------------
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

     // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
        // GL_DEPTH_COMPONENT : 오로지 depth 값만 필요하므로
        // ㅐㅎ당 설정을 한다.
                 GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH,
                 SHADOW_HEIGHT,
                 0,
                 GL_DEPTH_COMPONENT,
                 GL_FLOAT,
                 NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // OverSampling 문제를 해결하기 위해 GL_CLAMP_TO_BORDER 사용
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f}; // 0 ~ 1 범위를 벗어나는 값은 1로 설정 (depth 값 1)
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // attach depth texture as FBO's depth buffer
    // 새로운 FB 에 depthMap Texture 을 attach 한다.
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D,
                           depthMap,
                           0);

    // color 값은 필요없다. depth 값만 쓰면 된다.
    // 그래서 frame buffer 측에다가 color data 를 사용하지 않겠다라고
    // 반드시 명시해줘야 한다. 그래서 read, draw buffer 를 none 으로 설정한다.
    {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    {
        // shader configuration
        // ----------------------
        simpleDebugShader.use();
        simpleDebugShader.setInt("depthMap", 0);

        BlihnModelOnlyShader.use();
        BlihnModelOnlyShader.setInt("texture1", 0);

        gammaCorrectionShader.use();
        gammaCorrectionShader.setInt("floorTexture", 0);

        shadowMapShader.use();
        shadowMapShader.setInt("diffuseTexture", 0);
        shadowMapShader.setInt("shadowMap", 1);
    }

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

        // 1) Gamma Correction 예시
        // renderGamma(gammaCorrectionShader, floorTexture, floorTextureGmaCorrected);

        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 7.5f;

        {
            // Depth Map 그리기
            // 1. render depth of scene to texture (from light's perspective)


            // 현재 코드는 directional light 관점에서의 depth map 을 만들고 있다.
            // 이로 인해 모든 light ray 는 같은 방향을 가진다.
            // 따라서 광원의 projection matrix 는 orthographic 을 사용한다.
            // 우리가 평소에 원근법을 적용하게 되면,  눈에서 각 물체로
            // ray cast 를 한다고 가정하면, 각 물체로 가는 ray 의 방향이 모두 다르다
            // 하지만 orthographic 을 사용하게 되면, 모든 ray 의 방향이 같다고 가정한다.
            // 이러한 원리를 directional 광원에게도 동일하게 적용한다고 생각하면
            // 이해할 수 있다.
            float near_plane = 1.0f, far_plane = 7.5f;
            lightProjection =
                glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
            lightView = glm::lookAt(depthMapDirectionalLightPos,
                                    glm::vec3(0.0f),
                                    glm::vec3(0.0, 1.0, 0.0));

            // light 공간에서 clip space 로 변환해주는 matrix
            lightSpaceMatrix = lightProjection * lightView;

            // render scene from light's point of view
            depthMapShader.use();
            depthMapShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
            
            glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

            // depth 값을 기록하는 shader 를 실행하고자 한다.
            // 그래서 기존에 frame buffer object 에 기록된 depth buffer 값만 비운다.
            glClear(GL_DEPTH_BUFFER_BIT);

            glActiveTexture(GL_TEXTURE0);

            glBindTexture(GL_TEXTURE_2D, floorTexture);

            renderScene(depthMapShader);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        {
            // reset viewport
            // 왜 viewport 를 reset 해야 하는 것일까 ?
            // 가끔 shader map 의 크기가 실제 우리가 그리는 scene 크기와 다를 수 있다
            // 그래서 shadow map 과 실제 scene 을 그리기 전에 각각의 크기에 맞게
            // viewport 를 조정해줘야 한다.
            glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


            shadowMapShader.use();
            glm::mat4 projection =
                glm::perspective(glm::radians(camera.Zoom),
                                 (float)SCR_WIDTH / (float)SCR_HEIGHT,
                                 0.1f,
                                 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            shadowMapShader.setMat4("projection", projection);
            shadowMapShader.setMat4("view", view);
            // set light uniforms
            shadowMapShader.setVec3f("viewPos", camera.Position);
            shadowMapShader.setVec3f("lightPos", depthMapDirectionalLightPos);
            shadowMapShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, floorTexture);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, depthMap);

            glCullFace(GL_FRONT);
            renderScene(shadowMapShader);
            glCullFace(GL_BACK); // don’t forget to reset original culling face
        }

        {

            // render Depth map to quad for visual debugging
            // ---------------------------------------------
            simpleDebugShader.use();
            simpleDebugShader.setFloat("near_plane", near_plane);
            simpleDebugShader.setFloat("far_plane", far_plane);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, depthMap);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);

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

    // ex 1) Blihn model
    // if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinnKeyPressed)
    // {
    //     blinn = !blinn;
    //     blinnKeyPressed = true;
    // }
    // if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
    // {
    //     blinnKeyPressed = false;
    // }

    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !gammaKeyPressed)
    {
        gammaEnabled = !gammaEnabled;
        gammaKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
    {
        gammaKeyPressed = false;
    }
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

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const *path, bool gammaCorrection)
{

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum internalFormat;
        GLenum dataFormat;
        if (nrComponents == 1)
        {
            internalFormat = dataFormat = GL_RED;
        }
        else if (nrComponents == 3)
        {
            internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
            dataFormat = GL_RGB;
        }
        else if (nrComponents == 4)
        {
            internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
            dataFormat = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     internalFormat,
                     width,
                     height,
                     0,
                     dataFormat,
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

void renderGamma(Shader &gammaCorrectionShader, unsigned int floorTexture,
                 unsigned int floorTextureGmaCorrected)
{
    // draw objects
    gammaCorrectionShader.use();
    glm::mat4 projection =
        glm::perspective(glm::radians(camera.Zoom),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT,
                         0.1f,
                         100.0f);

    glm::mat4 view = camera.GetViewMatrix();
    gammaCorrectionShader.setMat4("projection", projection);
    gammaCorrectionShader.setMat4("view", view);

    // ex 1) Blihn model
    // set light uniforms
    // BlihnModelOnlyShader.setVec3f("viewPos", camera.Position);
    // BlihnModelOnlyShader.setVec3f("lightPos", lightPos);
    // BlihnModelOnlyShader.setInt("blinn", blinn);

    // set light uniforms
    glUniform3fv(
        glGetUniformLocation(gammaCorrectionShader.ID, "lightPositions"),
        4,
        &lightPositions[0][0]);
    glUniform3fv(glGetUniformLocation(gammaCorrectionShader.ID, "lightColors"),
                 4,
                 &lightColors[0][0]);
    gammaCorrectionShader.setVec3f("viewPos", camera.Position);
    gammaCorrectionShader.setInt("gamma", gammaEnabled);

    // floor
    glBindVertexArray(planeVAO);
    glActiveTexture(GL_TEXTURE0);

    /*
        >> 중요한 점이 있다. 
        자 Gamma Correction을 해결하는 여러 방법이 있다
        그 중 하나는 OpenGL 내부의 build in 변수를 사용하는 것
        ex) glEnable(GL_FRAMEBUFFER_SRGB);
        이 옵션을 켜게 되면
        opengl 이 알아서 색상을 color buffer 에 저장하기 전에
        gamma correction 을 해준다.
        */

    glBindTexture(GL_TEXTURE_2D,
                  gammaEnabled ? floorTextureGmaCorrected : floorTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // 1) Blinn-Phong
    // std::cout << (blinn ? "Blinn-Phong" : "Phong") << std::endl;

    std::cout << (gammaEnabled ? "Gamma enabled" : "Gamma disabled")
              << std::endl;
}

// renders the 3D scene
// --------------------
void renderScene(const Shader &shader)
{
    // floor
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // cubes
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
    model = glm::rotate(model,
                        glm::radians(60.0f),
                        glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.25));
    shader.setMat4("model", model);
    renderCube();
}


// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f,
            -1.0f,
            -1.0f,
            0.0f,
            0.0f,
            -1.0f,
            0.0f,
            0.0f, // bottom-left
            1.0f,
            1.0f,
            -1.0f,
            0.0f,
            0.0f,
            -1.0f,
            1.0f,
            1.0f, // top-right
            1.0f,
            -1.0f,
            -1.0f,
            0.0f,
            0.0f,
            -1.0f,
            1.0f,
            0.0f, // bottom-right
            1.0f,
            1.0f,
            -1.0f,
            0.0f,
            0.0f,
            -1.0f,
            1.0f,
            1.0f, // top-right
            -1.0f,
            -1.0f,
            -1.0f,
            0.0f,
            0.0f,
            -1.0f,
            0.0f,
            0.0f, // bottom-left
            -1.0f,
            1.0f,
            -1.0f,
            0.0f,
            0.0f,
            -1.0f,
            0.0f,
            1.0f, // top-left
            // front face
            -1.0f,
            -1.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            0.0f,
            0.0f, // bottom-left
            1.0f,
            -1.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            0.0f, // bottom-right
            1.0f,
            1.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            1.0f, // top-right
            1.0f,
            1.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            1.0f, // top-right
            -1.0f,
            1.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            0.0f,
            1.0f, // top-left
            -1.0f,
            -1.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            0.0f,
            0.0f, // bottom-left
            // left face
            -1.0f,
            1.0f,
            1.0f,
            -1.0f,
            0.0f,
            0.0f,
            1.0f,
            0.0f, // top-right
            -1.0f,
            1.0f,
            -1.0f,
            -1.0f,
            0.0f,
            0.0f,
            1.0f,
            1.0f, // top-left
            -1.0f,
            -1.0f,
            -1.0f,
            -1.0f,
            0.0f,
            0.0f,
            0.0f,
            1.0f, // bottom-left
            -1.0f,
            -1.0f,
            -1.0f,
            -1.0f,
            0.0f,
            0.0f,
            0.0f,
            1.0f, // bottom-left
            -1.0f,
            -1.0f,
            1.0f,
            -1.0f,
            0.0f,
            0.0f,
            0.0f,
            0.0f, // bottom-right
            -1.0f,
            1.0f,
            1.0f,
            -1.0f,
            0.0f,
            0.0f,
            1.0f,
            0.0f, // top-right
                  // right face
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            0.0f, // top-left
            1.0f,
            -1.0f,
            -1.0f,
            1.0f,
            0.0f,
            0.0f,
            0.0f,
            1.0f, // bottom-right
            1.0f,
            1.0f,
            -1.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            1.0f, // top-right
            1.0f,
            -1.0f,
            -1.0f,
            1.0f,
            0.0f,
            0.0f,
            0.0f,
            1.0f, // bottom-right
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            0.0f, // top-left
            1.0f,
            -1.0f,
            1.0f,
            1.0f,
            0.0f,
            0.0f,
            0.0f,
            0.0f, // bottom-left
            // bottom face
            -1.0f,
            -1.0f,
            -1.0f,
            0.0f,
            -1.0f,
            0.0f,
            0.0f,
            1.0f, // top-right
            1.0f,
            -1.0f,
            -1.0f,
            0.0f,
            -1.0f,
            0.0f,
            1.0f,
            1.0f, // top-left
            1.0f,
            -1.0f,
            1.0f,
            0.0f,
            -1.0f,
            0.0f,
            1.0f,
            0.0f, // bottom-left
            1.0f,
            -1.0f,
            1.0f,
            0.0f,
            -1.0f,
            0.0f,
            1.0f,
            0.0f, // bottom-left
            -1.0f,
            -1.0f,
            1.0f,
            0.0f,
            -1.0f,
            0.0f,
            0.0f,
            0.0f, // bottom-right
            -1.0f,
            -1.0f,
            -1.0f,
            0.0f,
            -1.0f,
            0.0f,
            0.0f,
            1.0f, // top-right
            // top face
            -1.0f,
            1.0f,
            -1.0f,
            0.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f, // top-left
            1.0f,
            1.0f,
            1.0f,
            0.0f,
            1.0f,
            0.0f,
            1.0f,
            0.0f, // bottom-right
            1.0f,
            1.0f,
            -1.0f,
            0.0f,
            1.0f,
            0.0f,
            1.0f,
            1.0f, // top-right
            1.0f,
            1.0f,
            1.0f,
            0.0f,
            1.0f,
            0.0f,
            1.0f,
            0.0f, // bottom-right
            -1.0f,
            1.0f,
            -1.0f,
            0.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f, // top-left
            -1.0f,
            1.0f,
            1.0f,
            0.0f,
            1.0f,
            0.0f,
            0.0f,
            0.0f // bottom-left
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(vertices),
                     vertices,
                     GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              8 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              8 * sizeof(float),
                              (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              8 * sizeof(float),
                              (void *)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        // 화면 크기의 frame buffer 를 그려내기 위한 quad
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
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
    }
    glBindVertexArray(quadVAO);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
}
