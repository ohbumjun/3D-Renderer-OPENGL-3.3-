#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

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

const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "out vec4 vertexColor; \n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   vertexColor = vec4(0.5, 0.0, 0.0, 1.0); \n"
    "}\0";
const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n\0";

int main()
{
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

    unsigned int shaderProgram;

    // shader
    {
        // shader object 를 생성한다.
        unsigned int vertexShader;

        // 인자 : 어떤 type 의 shader 를 만들고자 하는지
        vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // shader source code 를 우리가 만들어낸 shader object 에 붙여준다.
        // 그러면 복사본이 넘어간다.
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

        // shder object 를 컴파일한다.
        glCompileShader(vertexShader);

        // Shader Compile 이 성공적으로 이루어졌는지 확인한다.
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }

        // fragment shader
        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }

        // A shader program object is the final linked version
        // of multiple shaders combined
        
        shaderProgram = glCreateProgram();

        // shader program 에 vertex shader 와 fragment shader 를 붙인다.
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);

        // shader program 을 link 한다.
        glLinkProgram(shaderProgram);

        // Link 성공 여부 판단하기
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

    }
    
    
    unsigned int VAO;
    {
        float vertices[] = {
            0.5f,
            0.5f,
            0.0f, // top right
            0.5f,
            -0.5f,
            0.0f, // bottom right
            -0.5f,
            -0.5f,
            0.0f, // bottom left
            -0.5f,
            0.5f,
            0.0f // top left
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
            3 * sizeof(float), // 각 정점 데이터 사이의 간격
            (void *)0 // 메모리 상에서 data 가 시작하는 offset
        );

        // 해당 속성을 enable 시켜야 한다.
        // vertex 속성들은, 기본적으로 disable 되어 있다.
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0); // VAO unbind 시키기
    }   
    
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f; //0  ~ 1

        // 못찾을 경우 -1 을 리턴
        // 중요 : shader 내에서 uniform 을 찾는 것은 꼭 shader 를 사용한 이후가
        // 아니어도 된다. 단, 해당 uniform 에 값을 세팅하는 것은 shader 사용 이후
        int vertexColorLocation =
            glGetUniformLocation(shaderProgram, "ourColor");

        // shader program 을 사용한다.
        // 해당 함수 호출 이후, 모든 shader 와 rendering call 은
        // 해당 shader program 을 사용하게 된다.
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // 반드시 해당 uniform 에 값을 사용하기 전에
        // shader program 을 사용해야 한다.
        glUniform4f(
            vertexColorLocation, 
            0.0f, 
            greenValue, 
            0.0f, 
            1.0f);

        // wire frame mode 로 그리기
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : wire frame 해제

        // indexed 형태로 그리는 함수
        glDrawElements(GL_TRIANGLES,
            6, // 그리고 싶은 element 의 개수 (6개 정점을 그리고 싶다)
            GL_UNSIGNED_INT,  // type of indices
            0 // offset in ebo
        );
        // glDrawArrays(GL_TRIANGLES, 0, 3);

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
