#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>
#include "Shader.h"

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

void processInput(GLFWwindow *window);

int main()
{
    #pragma region OpenGL 초기화
    // GLFW 초기화
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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
    glViewport(0, 0 , 800, 600);

    // 콜백함수 등록
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    

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

    void* h = (void *)0;

    // shader
    std::string vrxShaderPath =
        "D:\\OpenGL\\LearnOpenGL\\LearnOpenGLSrc\\LearnOpenGL\\VertexShader.glsl";

    std::string fragShaderPath =
        "D:\\OpenGL\\LearnOpenGL\\LearnOpenGLSrc\\LearnOpenGL\\FragmentShader.glsl";

    Shader ourShader(vrxShaderPath.c_str(), fragShaderPath.c_str());
    
    #pragma region Vertex
    unsigned int VAO;
    {
        float texCoords[] = {
            0.0f,
            0.0f, // lower-left corner
            1.0f,
            0.0f, // lower-right corner
            0.5f,
            1.0f // top-center corner
        };

        // vetex  + color
        // float vertices[] = {
        //     // positions        // colors
        //     0.5f,-0.5f,0.0f,    1.0f,0.0f,0.0f, // bottom right
        //     -0.5f,
        //     -0.5f,
        //     0.0f,
        //     0.0f,
        //     1.0f,
        //     0.0f, // bottom left
        //     0.0f,
        //     0.5f,
        //     0.0f,
        //     0.0f,
        //     0.0f,
        //     1.0f // top
        // };

        // 4 point Rectangle
        // -> glDrawElements 에서 6 개의 정점을 넘겨주면 된다.
        // -> 2개의 삼각형. 총 6개의 정점
        // float vertices[] = {
        //     // positions // colors // texture coords
        //     0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        //     0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        //     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        //     -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
        // };

        // 6 개의 면, 36 개의 vertices
        // ex) glDrawArrays(GL_TRIANGLES, 0, 36);
        float vertices[] = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 
            0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,

            -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 
            0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, 
            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

            -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, 
            -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 
            -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 
            0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,

            0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 
            0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 
            0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 
            0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, 
            -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f
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

        glGenVertexArrays(1, &VAO);

        // VAO 를 먼저 bind 시킨다.
        // 그 다음에 비로소 vbo, 속성을 configure 한다.
        glBindVertexArray(VAO);

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

        /*
        * VBO 란, vertex data 를 담은 opengl 상의 메모리이다.
        */
        unsigned int VBO;
        glGenBuffers(1, &VBO);

        // Opengl 은 여러 type 의 buffer object 를 가지고 있다.
        // 그리고 'vetex buffer object' 의 type 이 GL_ARRAY_BUFFER 이다.
        // opengl 은, 서로 다른 type의 여러 개 buffer 를 bind 할 수 있게 해준다.
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // 해당 VBO 메모리 공간에, 현재 vertices 의 데이터를 복사한다.
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(vertices),
                     vertices,
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
            0, // 어떤 vertex attribute 을 configure 하는 것인가
            // ex) layout(location = 0) in vec3 aPos;
            3,                 // vertex attribute 의 크기
            GL_FLOAT,          // 데이터의 type
            GL_FALSE,          // 정규화 여부
            5 * sizeof(float), // 각 정점 데이터 사이의 간격
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

        // texture attribute
        glVertexAttribPointer(1,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              5 * sizeof(float),
                              (void *)(3 * sizeof(float)));

        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0); // VAO unbind 시키기
    }   
    #pragma endregion

    #pragma region Texture
    int width, height, nrChannels;
    
    // OPENGL 은 Texture Coord 가 아래->위 방향으로 증가한다고 계산
    // 하지만 stbl 은 위에서 아래 방향으로 증가한다고 계산
    // 따라서 그 값들을 뒤집어 줘야 한다.
    stbi_set_flip_vertically_on_load(1);

    unsigned char *data = stbi_load(RESOURCE_ROOT "SampleImage.jpg",
                                    &width,
                                    &height,
                                    &nrChannels,
                                    0);

    // index buffer, vertex buffer 처럼 마찬가지로 texture object 를 생성한다.
    unsigned int texture1; // unsigned int 배열이 될 수도 있다. 여러 개의 texture 담을 경우
    glGenTextures(
        1, // 만들고자 하는 texture 들의 개수
        &texture1);

    // texture 를 bind 시켜서
    // 이후의 subsequent texture 관련 작업이 이 texture 에 대해 이루어지게 한다.
    glBindTexture(GL_TEXTURE_2D, texture1);

    // 이제 실제 texture 메모리를 할당한다.
    // 해당 함수 까지 호출하면, texture object 에 texture image 가 실제 attach 된 것
    // 하지만, 여기까지만 하면 texture image 의 base-level 만 존재하는 것
    glTexImage2D(
        GL_TEXTURE_2D, // texture target
                 0, // mipmap level
                 GL_RGB, // format
                 width,
                 height,
                 0, // 항상 0
                 GL_RGB, // source image 의 format
                 GL_UNSIGNED_BYTE,
                 data);

    // Set texture wrapping mode to GL_CLAMP_TO_EDGE
    // wrapping_mode 란, texture 좌표 가 넘어갔을 때 어떻게 처리할지를 
    // GL_CLAMP_TO_EDGE : 0 보다 작거나, 1 보다 큰 값은 가장 자리 픽셀의 색상으로 채워진다.
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // set texture filtering parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // mipmap 을 생성한다.
    // 현재 bind 된 texture 에 대해 mipmap 을 생성한다.
    glGenerateMipmap(GL_TEXTURE_2D);

    unsigned char *data2 = stbi_load(RESOURCE_ROOT "curious.png",
                                    &width,
                                    &height,
                                    &nrChannels,
                                    0);

    // index buffer, vertex buffer 처럼 마찬가지로 texture object 를 생성한다.
    unsigned int
        texture2; // unsigned int 배열이 될 수도 있다. 여러 개의 texture 담을 경우
    glGenTextures(1, // 만들고자 하는 texture 들의 개수
                  &texture2);

    // texture 를 bind 시켜서
    // 이후의 subsequent texture 관련 작업이 이 texture 에 대해 이루어지게 한다.
    glBindTexture(GL_TEXTURE_2D, texture2);

    // 이제 실제 texture 메모리를 할당한다.
    // 해당 함수 까지 호출하면, texture object 에 texture image 가 실제 attach 된 것
    // 하지만, 여기까지만 하면 texture image 의 base-level 만 존재하는 것
    glTexImage2D(GL_TEXTURE_2D, // texture target
                 0,             // mipmap level
                 GL_RGB,        // format
                 width,
                 height,
                 0,      // 항상 0
                 GL_RGBA, // source image 의 format -> png 은 alpha channel 이 있으므로
                 GL_UNSIGNED_BYTE,
                 data2);

    // Set texture wrapping mode to GL_CLAMP_TO_EDGE
    // wrapping_mode 란, texture 좌표 가 넘어갔을 때 어떻게 처리할지를
    // GL_CLAMP_TO_EDGE : 0 보다 작거나, 1 보다 큰 값은 가장 자리 픽셀의 색상으로 채워진다.
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // set texture filtering parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // mipmap 을 생성한다.
    // 현재 bind 된 texture 에 대해 mipmap 을 생성한다.
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    stbi_image_free(data2);

    #pragma endregion

    #pragma region Texture Unit

    // glUseProgram(shaderProgram); // don’t forget to activate the shader first!
    ourShader.use();

    // uniform sampler2D texture1 에 세팅
    // glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0); 
    ourShader.setInt("texture1", 0); // or with shader class

    // glUniform1i(glGetUniformLocation(ourShader.ID, "texture2"),1); // or with shader class
    ourShader.setInt("texture2", 1); // or with shader class

    #pragma endregion

    #pragma region DEPTH TEST
    // enable depth test
    glEnable(GL_DEPTH_TEST);
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

    glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(2.0f, 5.0f, -15.0f),
                                 glm::vec3(-1.5f, -2.2f, -2.5f),
                                 glm::vec3(-3.8f, -2.0f, -12.3f),
                                 glm::vec3(2.4f, -0.4f, -3.5f),
                                 glm::vec3(-1.7f, 3.0f, -7.5f),
                                 glm::vec3(1.3f, -2.0f, -2.5f),
                                 glm::vec3(1.5f, 2.0f, -2.5f),
                                 glm::vec3(1.5f, 0.2f, -1.5f),
                                 glm::vec3(-1.3f, 1.0f, -1.5f)};

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT); // clear the color buffer
        // glClear(GL_DEPTH_BUFFER_BIT); // clear the depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear both color, depth

        float timeValue = glfwGetTime();
        float mixValue = (sin(timeValue) / 2.0f) + 0.5f; //0  ~ 1

        // shader program 을 사용한다.
        // 해당 함수 호출 이후, 모든 shader 와 rendering call 은
        // 해당 shader program 을 사용하게 된다.
        // glUseProgram(shaderProgram);
        ourShader.use();

        // 마찬가지로, 아래 texture 함수들도 shader program 사용 이후에 호출되어야 한다.
        glActiveTexture(GL_TEXTURE0);  // activate texture unit first
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO);

        // model : local space -> world space 로 변환
        glm::mat4 model = glm::mat4(1.0f); 
        model = glm::rotate(model,
                         (float)glfwGetTime(),
                         glm::vec3(0.5f, 1.0f, 0.0f)); // 비스듬한 축 회전

        // view : world space -> view space 로 변환
        // glm::mat4 view = glm::mat4(1.0f);

        // note that we’re translating the scene in the reverse direction
        // 즉, 카메라를 z 축 방향 -3이 된다는 것은, Scene 이 그만큼 우리 정면으로 가까워진다는 것 ?
        // 왜냐면 카메라 이동 방향과 scene 의 movement 가 반대가 되는 개념이기 때문이다.
        // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        const float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;

        // 실시간으로 camera 위치 변경. 단 바라보는 target 은 고정
        view = glm::lookAt(glm::vec3(camX, 0.0, camZ),
                           glm::vec3(0.0, 0.0, 0.0),
                           glm::vec3(0.0, 1.0, 0.0));


        glm::mat4 projection;

        projection = glm::perspective(
            glm::radians(45.0f),
            800.0f / 600.0f,
            0.1f,
            100.0f
        );

        // 반드시 해당 uniform 에 값을 사용하기 전에
        // shader program 을 사용해야 한다.
        ourShader.setFloat("mixValue", mixValue);
       //  ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model,
                                (float)glfwGetTime()  + glm::radians(angle),
                                glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // wire frame mode 로 그리기
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : wire frame 해제

        // indexed 형태로 그리는 함수
        // glDrawElements(GL_TRIANGLES,
        //     36, // 그리고 싶은 element 의 개수 (ex. 6개 정점을 그리고 싶다)
        //     GL_UNSIGNED_INT,  // type of indices
        //     0 // offset in ebo
        // );

        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // // check and call events and swap the buffers
        glfwSwapBuffers(window); // double buffering
        glfwPollEvents(); // event 가 발생했는지 계속해서 검사
    }

    glfwTerminate(); // 모든 glfw 자원 지운다.

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
