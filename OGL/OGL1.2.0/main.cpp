#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
using namespace std;

//前向声明
//调整窗口的回调函数 此回调函数会在窗口每次大小被调整的时候调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//按键检测
void processInput(GLFWwindow *window);
//捕捉异常函数回调
void error_callback(int, const char * err_str) ;
//检测着色器构建情况
void check_ShaderBuild(unsigned int shaderid);
//检测着色器程序构建情况
void check_ShaderProgrambuild(unsigned int shaderprogram);

// 窗口变量宏
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//暂时将顶点着色器的源代码硬编码在代码文件顶部的C风格字符串中，能够让OpenGL使用它，我们必须在运行时动态编译它的源代码。
//顶点着色器硬编码
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
//片段着色器硬编码
//黑色
const char *fragmentShaderSource1 = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
                                   "}\n\0";
//白色
const char *fragmentShaderSource2 = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
                                    "}\n\0";

int main()
{
    /******        进行初始化设置         ******/

    glfwInit(); //初始化GLFW
    // 此处确保当用户的OGL版本不吻合时无法运行
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3); //配置GLFW 主版本号设为3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3); //配置GLFW 次版本号设为3
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);//配置GLFW 使用的是核心模式 且只能使用OGL功能的一个子集
    // glfwWindowHint(GLFW_DECORATED, GL_FALSE);//设置无边框
    //线框模式
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#ifdef __APPLE__
    //IOS配置
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //创建一个窗口对象 存放有关窗口的相关数据
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "learnopengl", NULL, NULL);

    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //调用任何ogl函数前先初始化glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }
    //注册修改窗口参数的回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /******        编译并链接着色器，获取着色器对象，并回收着色器资源         ******/

    //** 根据OGL的渲染管线 我们可以定制三种着色器 顶点着色器 几何着色器 片段着色器 **//
    //** 首先依据需求 我们先定制两个最简化的shader 顶点着色器（Vertex Shader） 以及 片段着色器（Fragment Shader）**//
    // 构建vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); //创建一个着色器 并且用ID进行引用
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //附加到着色器对象上 编译硬编码
    glCompileShader(vertexShader);

    //构建Fragment Shader //黑色,白色
    unsigned int fragmentShader1,fragmentShader2;
    fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
    glCompileShader(fragmentShader1);

    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader2);
    //检测着色器构建情况
    check_ShaderBuild(vertexShader);
    check_ShaderBuild(fragmentShader1);
    check_ShaderBuild(fragmentShader2);

    //链接着色器 为什么着色器需要链接 是因为采用了分层模式这种设计模式
    unsigned int shaderProgram1,shaderProgram2;
    shaderProgram1 = glCreateProgram();
    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader1);
    glLinkProgram(shaderProgram1);

    shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);
    //检测着色器对象程序构建情况
    check_ShaderProgrambuild(shaderProgram1);
    check_ShaderProgrambuild(shaderProgram2);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader1);
    glDeleteShader(fragmentShader2);

    /******        编写顶点数据         ******/
    float vertices1[] = {
            -0.2f,  0.5f, 0.0f,
            -0.2f, -0.5f, 0.0f,
            -0.8f, -0.5f, 0.0f,
            -0.8f,  0.5f, 0.0f
    };
    unsigned int indices1[] = {
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
    };
    float vertices2[] = {
            0.2f,  0.5f, 0.0f, //0
            0.2f, -0.5f, 0.0f,
            0.7f,  -0.5f, 0.0f,
            0.3f, 0.5f, 0.0f,
            0.8f,-0.5f,0.0f,    //4
            0.8f,0.5f,0.0f  //5
    };
    unsigned int indices2[] = {
            0, 1, 2,  // first Triangle
            3, 4, 5   // second Triangle
    };
    /******        获取VAO,VBO,EBO         ******/
    //顶点缓冲对象 VBO
    //顶点数组对象 VAO
    //索引缓冲对象
    unsigned int VBOs[2], VAOs[2],EBOs[2];
    glGenVertexArrays(2, VAOs); //VAO前边那个数字是构建几个VAO
    glGenBuffers(2,VBOs);
    glGenBuffers(2,EBOs);
    /******        链接顶点         ******/
    //先绑定VAO 再绑定设置VBO 最后设置顶点属性 vertex attributes
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //线框模式
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /******        绘制模式        ******/
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //绘制三角形
        glUseProgram(shaderProgram1);
        glBindVertexArray(VAOs[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glUseProgram(shaderProgram2);
        glBindVertexArray(VAOs[1]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    /******        释放资源，提交异常        ******/
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteBuffers(2, EBOs);
    glDeleteProgram(shaderProgram1);
    glDeleteProgram(shaderProgram2);
    glfwTerminate();

    //捕捉异常
    glfwSetErrorCallback(error_callback);
    return 0;

}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //前两个参数控制左下角 位置 3 4 参数控制宽度跟高度
    glViewport(0, 0, width, height);
}
//返回此按键是否被按下
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
//捕捉异常函数回调
void error_callback(int, const char * err_str) {
    cout << "ERROR: " << err_str << endl;
}

void check_ShaderBuild(unsigned int shaderid) {
    int success;
    char infoLog[512];
    glGetShaderiv(shaderid, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderid, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }
}

void check_ShaderProgrambuild(unsigned int shaderprogram) {
    int success;
    char infoLog[512];
    glGetProgramiv(shaderprogram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderprogram, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }
}

