#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

/*
* >> GLFW ? GLAD ?
penGL�� specification�� OS�� ������� 
���� implementation�ϰ� �����ִ� ���� GLFW�� GLAD

>> GLFW ?
LFW�� OpenGL ���ؽ�Ʈ�� ����Ͽ� â�� ����� ���� �� ���ִ� ���� C ���̺귯��

>> GLAD ?
 GLAD�� ����� function�� ��ġ�� ã�� �Լ������͸� ������ ���� 
 ������ �شٴ� ��
*/

/*
* window â�� resize �� ������ ȣ��Ǵ� �ݹ� �Լ�
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
    // GLFW �ʱ�ȭ
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

    // GLAD �ʱ�ȭ
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

    // �ݹ��Լ� ���
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    

    // vertex attribute �� �ִ� ����
    // - ���� vertex shader �� input �� ����� ��
    // 'vertex attribute' �̶�� �Ѵ�.
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes
              << std::endl;

    unsigned int shaderProgram;

    // shader
    {
        // shader object �� �����Ѵ�.
        unsigned int vertexShader;

        // ���� : � type �� shader �� ������� �ϴ���
        vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // shader source code �� �츮�� ���� shader object �� �ٿ��ش�.
        // �׷��� ���纻�� �Ѿ��.
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

        // shder object �� �������Ѵ�.
        glCompileShader(vertexShader);

        // Shader Compile �� ���������� �̷�������� Ȯ���Ѵ�.
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

        // shader program �� vertex shader �� fragment shader �� ���δ�.
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);

        // shader program �� link �Ѵ�.
        glLinkProgram(shaderProgram);

        // Link ���� ���� �Ǵ��ϱ�
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
        * - �Ӽ� ~ VBO configure ������ �ѹ��� ��� object
        * - �̰͸� bind ��Ű�⸸ �ϸ� �ȴ�.
        * - �Ӹ� �ƴ϶�, EBO ������ ���� ������ �ִ´�.
        */

        glGenVertexArrays(1, &VAO);

        // VAO �� ���� bind ��Ų��.
        // �� ������ ��μ� vbo, �Ӽ��� configure �Ѵ�.
        glBindVertexArray(VAO);

        /*
        * indexed buffer �� ���� object :GL_ELEMENT_ARRAY_BUFFER
        */
        unsigned int EBO;
        glGenBuffers(1, &EBO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(indices),
                     indices,
                     GL_STATIC_DRAW);

        /*
        * VBO ��, vertex data �� ���� opengl ���� �޸��̴�.
        */
        unsigned int VBO;
        glGenBuffers(1, &VBO);

        // Opengl �� ���� type �� buffer object �� ������ �ִ�.
        // �׸��� 'vetex buffer object' �� type �� GL_ARRAY_BUFFER �̴�.
        // opengl ��, ���� �ٸ� type�� ���� �� buffer �� bind �� �� �ְ� ���ش�.
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // �ش� VBO �޸� ������, ���� vertices �� �����͸� �����Ѵ�.
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(vertices),
                     vertices,

                     /*
        * �׷��� ī�尡 �츮�� ������ �����͸� ������� �ٷ����� �����Ѵ�.
        * 
        * > GL_STATIC_DRAW : data �� �ѹ� set, ���� �� ����
        * > GL_DYNAMIC_DRAW : data �� ���� �� set, ���� �� ����
        * > GL_STREAM_DRAW : data �� �ѹ� set, ���� ���� ����
        * 
        * �츮�� ���� �Ѱ��ִ� ���� ������ , ������ �ʰ�, ������ ���ȴ�.
        * ���� ���� ���ϴ� ������� GL_DYNAMIC_DRAW �� ����Ͽ�
        * Opengl �� ������ �� �� �ִ� �޸� �������� �����͸� ��ġ��ų �� �ְ� 
        * �ؾ� �Ѵ�.
        */
                     GL_STATIC_DRAW);

        // shader �� �غ��ϴ� �͸����δ� �����ϴ�
        // vertex shader ��, ���� �޸� ���� vertex data ��
        // ��� �ؼ��ؾ� ���� �𸥴�. ���� vertex attribute ��
        // ���ؼ� �̸� �˷��־�� �Ѵ�.
        glVertexAttribPointer(
            0, // � vertex attribute �� configure �ϴ� ���ΰ�
            // ex) layout(location = 0) in vec3 aPos;
            3,                 // vertex attribute �� ũ��
            GL_FLOAT,          // �������� type
            GL_FALSE,          // ����ȭ ����
            3 * sizeof(float), // �� ���� ������ ������ ����
            (void *)0 // �޸� �󿡼� data �� �����ϴ� offset
        );

        // �ش� �Ӽ��� enable ���Ѿ� �Ѵ�.
        // vertex �Ӽ�����, �⺻������ disable �Ǿ� �ִ�.
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0); // VAO unbind ��Ű��
    }   
    
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f; //0  ~ 1

        // ��ã�� ��� -1 �� ����
        // �߿� : shader ������ uniform �� ã�� ���� �� shader �� ����� ���İ�
        // �ƴϾ �ȴ�. ��, �ش� uniform �� ���� �����ϴ� ���� shader ��� ����
        int vertexColorLocation =
            glGetUniformLocation(shaderProgram, "ourColor");

        // shader program �� ����Ѵ�.
        // �ش� �Լ� ȣ�� ����, ��� shader �� rendering call ��
        // �ش� shader program �� ����ϰ� �ȴ�.
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // �ݵ�� �ش� uniform �� ���� ����ϱ� ����
        // shader program �� ����ؾ� �Ѵ�.
        glUniform4f(
            vertexColorLocation, 
            0.0f, 
            greenValue, 
            0.0f, 
            1.0f);

        // wire frame mode �� �׸���
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : wire frame ����

        // indexed ���·� �׸��� �Լ�
        glDrawElements(GL_TRIANGLES,
            6, // �׸��� ���� element �� ���� (6�� ������ �׸��� �ʹ�)
            GL_UNSIGNED_INT,  // type of indices
            0 // offset in ebo
        );
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        // // check and call events and swap the buffers
        glfwSwapBuffers(window); // double buffering
        glfwPollEvents(); // event �� �߻��ߴ��� ����ؼ� �˻�
    }

    glfwTerminate(); // ��� glfw �ڿ� �����.

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
