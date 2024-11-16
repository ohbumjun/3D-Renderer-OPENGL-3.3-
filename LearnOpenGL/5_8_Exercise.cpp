//#include <GLFW/glfw3.h>
//#include <glad/glad.h>
//#include <iostream>
//
///*
//* >> GLFW ? GLAD ?
//penGL�� specification�� OS�� ������� 
//���� implementation�ϰ� �����ִ� ���� GLFW�� GLAD
//
//>> GLFW ?
//LFW�� OpenGL ���ؽ�Ʈ�� ����Ͽ� â�� ����� ���� �� ���ִ� ���� C ���̺귯��
//
//>> GLAD ?
// GLAD�� ����� function�� ��ġ�� ã�� �Լ������͸� ������ ���� 
// ������ �شٴ� ��
//*/
//
///*
//* window â�� resize �� ������ ȣ��Ǵ� �ݹ� �Լ�
//*/
//void framebuffer_size_callback(GLFWwindow *window, int width, int height);
//
//void processInput(GLFWwindow *window);
//
//const char *vertexShaderSource =
//    "#version 330 core\n"
//    "layout (location = 0) in vec3 aPos;\n"
//    "void main()\n"
//    "{\n"
//    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//    "}\0";
//const char *fragmentShaderSource =
//    "#version 330 core\n"
//    "out vec4 FragColor;\n"
//    "void main()\n"
//    "{\n"
//    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//    "}\n\0";
//
//const char *fragmentShaderSource_red =
//    "#version 330 core\n"
//    "out vec4 FragColor;\n"
//    "void main()\n"
//    "{\n"
//    "   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
//    "}\n\0";
//
//int main()
//{
//    // GLFW �ʱ�ȭ
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//
//    // GLAD �ʱ�ȭ
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
//    glViewport(0, 0 , 800, 600);
//
//    // �ݹ��Լ� ���
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//    
//    int success;
//    char infoLog[512];
//
//    // shader object �� �����Ѵ�.
//    unsigned int vertexShader;
//
//    unsigned int shaderProgram1;
//    unsigned int shaderProgram2;
//
//    // vertex
//    #pragma region VERTEX SHARED
//    {
//        // ���� : � type �� shader �� ������� �ϴ���
//        vertexShader = glCreateShader(GL_VERTEX_SHADER);
//
//        // shader source code �� �츮�� ���� shader object �� �ٿ��ش�.
//        // �׷��� ���纻�� �Ѿ��.
//        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//
//        // shder object �� �������Ѵ�.
//        glCompileShader(vertexShader);
//
//        // Shader Compile �� ���������� �̷�������� Ȯ���Ѵ�.
//
//        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//
//        if (!success)
//        {
//            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
//                      << infoLog << std::endl;
//        }
//    }
//    #pragma endregion
//
//    // fragment shader (yellow)
//
//    #pragma region frag_yellow
//    {
//        
//        unsigned int fragmentShader;
//        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//        glCompileShader(fragmentShader);
//
//        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//
//        if (!success)
//        {
//            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
//                      << infoLog << std::endl;
//        }
//
//        // A shader program object is the final linked version
//        // of multiple shaders combined
//        
//        shaderProgram1 = glCreateProgram();
//
//        // shader program �� vertex shader �� fragment shader �� ���δ�.
//        glAttachShader(shaderProgram1, vertexShader);
//        glAttachShader(shaderProgram1, fragmentShader);
//
//        // shader program �� link �Ѵ�.
//        glLinkProgram(shaderProgram1);
//
//        // Link ���� ���� �Ǵ��ϱ�
//        glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
//        if (!success)
//        {
//            glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
//            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
//                      << infoLog << std::endl;
//        }
//
//        glDeleteShader(fragmentShader);
//    }
//    #pragma endregion
//
//    #pragma region frag_red
//    // fragment shader (green)
//    {
//
//        unsigned int fragmentShader;
//        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//        glShaderSource(fragmentShader, 1, &fragmentShaderSource_red, NULL);
//        glCompileShader(fragmentShader);
//
//        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//
//        if (!success)
//        {
//            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
//                      << infoLog << std::endl;
//        }
//
//        // A shader program object is the final linked version
//        // of multiple shaders combined
//
//        shaderProgram2 = glCreateProgram();
//
//        // shader program �� vertex shader �� fragment shader �� ���δ�.
//        glAttachShader(shaderProgram2, vertexShader);
//        glAttachShader(shaderProgram2, fragmentShader);
//
//        // shader program �� link �Ѵ�.
//        glLinkProgram(shaderProgram2);
//
//        // Link ���� ���� �Ǵ��ϱ�
//        glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
//        if (!success)
//        {
//            glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
//            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
//                      << infoLog << std::endl;
//        }
//
//        glDeleteShader(fragmentShader);
//    }
//    #pragma endregion
//    
//   glDeleteShader(vertexShader);
//    
//     
//
//    unsigned int VAO1;
//    unsigned int VAO2;
//
//    #pragma region vao1
//    {
//        float vertices[] = {
//            -1.0f,
//            -0.5f,
//            0.0f, // left
//            0.0f,
//            -0.5f,
//            0.0f, // right
//            -0.5f,
//            0.5f,
//            0.0f // top
//        }; 
//        /*
//        * VAO
//        * - �Ӽ� ~ VBO configure ������ �ѹ��� ��� object
//        * - �̰͸� bind ��Ű�⸸ �ϸ� �ȴ�.
//        * - �Ӹ� �ƴ϶�, EBO ������ ���� ������ �ִ´�.
//        */
//
//        glGenVertexArrays(1, &VAO1);
//
//        // VAO �� ���� bind ��Ų��.
//        // �� ������ ��μ� vbo, �Ӽ��� configure �Ѵ�.
//        glBindVertexArray(VAO1);
//
//        /*
//        * VBO ��, vertex data �� ���� opengl ���� �޸��̴�.
//        */
//        unsigned int VBO;
//        glGenBuffers(1, &VBO);
//
//        // Opengl �� ���� type �� buffer object �� ������ �ִ�.
//        // �׸��� 'vetex buffer object' �� type �� GL_ARRAY_BUFFER �̴�.
//        // opengl ��, ���� �ٸ� type�� ���� �� buffer �� bind �� �� �ְ� ���ش�.
//        glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//        // �ش� VBO �޸� ������, ���� vertices �� �����͸� �����Ѵ�.
//        glBufferData(GL_ARRAY_BUFFER,
//                     sizeof(vertices),
//                     vertices,
//
//                     /*
//        * �׷��� ī�尡 �츮�� ������ �����͸� ������� �ٷ����� �����Ѵ�.
//        * 
//        * > GL_STATIC_DRAW : data �� �ѹ� set, ���� �� ����
//        * > GL_DYNAMIC_DRAW : data �� ���� �� set, ���� �� ����
//        * > GL_STREAM_DRAW : data �� �ѹ� set, ���� ���� ����
//        * 
//        * �츮�� ���� �Ѱ��ִ� ���� ������ , ������ �ʰ�, ������ ���ȴ�.
//        * ���� ���� ���ϴ� ������� GL_DYNAMIC_DRAW �� ����Ͽ�
//        * Opengl �� ������ �� �� �ִ� �޸� �������� �����͸� ��ġ��ų �� �ְ� 
//        * �ؾ� �Ѵ�.
//        */
//                     GL_STATIC_DRAW);
//
//        // shader �� �غ��ϴ� �͸����δ� �����ϴ�
//        // vertex shader ��, ���� �޸� ���� vertex data ��
//        // ��� �ؼ��ؾ� ���� �𸥴�. ���� vertex attribute ��
//        // ���ؼ� �̸� �˷��־�� �Ѵ�.
//        glVertexAttribPointer(
//            0, // � vertex attribute �� configure �ϴ� ���ΰ�
//            // ex) layout(location = 0) in vec3 aPos;
//            3,                 // vertex attribute �� ũ��
//            GL_FLOAT,          // �������� type
//            GL_FALSE,          // ����ȭ ����
//            3 * sizeof(float), // �� ���� ������ ������ ����
//            (void *)0 // �޸� �󿡼� data �� �����ϴ� offset
//        );
//
//        // �ش� �Ӽ��� enable ���Ѿ� �Ѵ�.
//        // vertex �Ӽ�����, �⺻������ disable �Ǿ� �ִ�.
//        glEnableVertexAttribArray(0);
//
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//        glBindVertexArray(0); // VAO unbind ��Ű��
//    }   
//    #pragma endregion
//
//    #pragma region vao2
//    {
//        float vertices[] = {
//            0.0f,
//            -0.5f,
//            0.0f, // left
//            0.0f,
//            0.5f,
//            0.0f, // right
//            1.f,
//            0.5f,
//            0.0f // top
//        }; 
//        /*
//        * VAO
//        * - �Ӽ� ~ VBO configure ������ �ѹ��� ��� object
//        * - �̰͸� bind ��Ű�⸸ �ϸ� �ȴ�.
//        * - �Ӹ� �ƴ϶�, EBO ������ ���� ������ �ִ´�.
//        */
//
//        glGenVertexArrays(1, &VAO2);
//
//        // VAO �� ���� bind ��Ų��.
//        // �� ������ ��μ� vbo, �Ӽ��� configure �Ѵ�.
//        glBindVertexArray(VAO2);
//
//        /*
//        * VBO ��, vertex data �� ���� opengl ���� �޸��̴�.
//        */
//        unsigned int VBO2;
//        glGenBuffers(1, &VBO2);
//
//        // Opengl �� ���� type �� buffer object �� ������ �ִ�.
//        // �׸��� 'vetex buffer object' �� type �� GL_ARRAY_BUFFER �̴�.
//        // opengl ��, ���� �ٸ� type�� ���� �� buffer �� bind �� �� �ְ� ���ش�.
//        glBindBuffer(GL_ARRAY_BUFFER, VBO2);
//
//        // �ش� VBO �޸� ������, ���� vertices �� �����͸� �����Ѵ�.
//        glBufferData(GL_ARRAY_BUFFER,
//                     sizeof(vertices),
//                     vertices,
//
//                     /*
//        * �׷��� ī�尡 �츮�� ������ �����͸� ������� �ٷ����� �����Ѵ�.
//        * 
//        * > GL_STATIC_DRAW : data �� �ѹ� set, ���� �� ����
//        * > GL_DYNAMIC_DRAW : data �� ���� �� set, ���� �� ����
//        * > GL_STREAM_DRAW : data �� �ѹ� set, ���� ���� ����
//        * 
//        * �츮�� ���� �Ѱ��ִ� ���� ������ , ������ �ʰ�, ������ ���ȴ�.
//        * ���� ���� ���ϴ� ������� GL_DYNAMIC_DRAW �� ����Ͽ�
//        * Opengl �� ������ �� �� �ִ� �޸� �������� �����͸� ��ġ��ų �� �ְ� 
//        * �ؾ� �Ѵ�.
//        */
//                     GL_STATIC_DRAW);
//
//        // shader �� �غ��ϴ� �͸����δ� �����ϴ�
//        // vertex shader ��, ���� �޸� ���� vertex data ��
//        // ��� �ؼ��ؾ� ���� �𸥴�. ���� vertex attribute ��
//        // ���ؼ� �̸� �˷��־�� �Ѵ�.
//        glVertexAttribPointer(
//            0, // � vertex attribute �� configure �ϴ� ���ΰ�
//            // ex) layout(location = 0) in vec3 aPos;
//            3,                 // vertex attribute �� ũ��
//            GL_FLOAT,          // �������� type
//            GL_FALSE,          // ����ȭ ����
//            3 * sizeof(float), // �� ���� ������ ������ ����
//            (void *)0 // �޸� �󿡼� data �� �����ϴ� offset
//        );
//
//        // �ش� �Ӽ��� enable ���Ѿ� �Ѵ�.
//        // vertex �Ӽ�����, �⺻������ disable �Ǿ� �ִ�.
//        glEnableVertexAttribArray(0);
//
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//        glBindVertexArray(0); // VAO unbind ��Ű��
//    }   
//
//    #pragma endregion
//    
//    
//    while (!glfwWindowShouldClose(window))
//    {
//        processInput(window);
//
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT); 
//
//        // shader program �� ����Ѵ�.
//        // �ش� �Լ� ȣ�� ����, ��� shader �� rendering call ��
//        // �ش� shader program �� ����ϰ� �ȴ�.
//        glUseProgram(shaderProgram1);
//        glBindVertexArray(VAO1);
//
//        // wire frame mode �� �׸���
//        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : wire frame ����
//
//        // indexed ���·� �׸��� �Լ�
//        // glDrawElements(GL_TRIANGLES,
//        //     6, // �׸��� ���� element �� ���� (6�� ������ �׸��� �ʹ�)
//        //     GL_UNSIGNED_INT,  // type of indices
//        //     0 // offset in ebo
//        // );
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//
//        glBindVertexArray(0); // VAO unbind ��Ű��
//
//        glUseProgram(shaderProgram2);
//        glBindVertexArray(VAO2);
//
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//
//        glBindVertexArray(0); // VAO unbind ��Ű��
//
//        // // check and call events and swap the buffers
//        glfwSwapBuffers(window); // double buffering
//        glfwPollEvents(); // event �� �߻��ߴ��� ����ؼ� �˻�
//    }
//
//    glfwTerminate(); // ��� glfw �ڿ� �����.
//
//    return 0;
//}
//
//void framebuffer_size_callback(GLFWwindow *window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//}
//
//
//void processInput(GLFWwindow *window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//}
