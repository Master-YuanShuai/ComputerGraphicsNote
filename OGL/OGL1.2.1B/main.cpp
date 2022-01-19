#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "cmath"
#include "shader/Shader.hpp"
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

    Shader Shader1("D:\\Study\\TA\\OGL\\OGL1.2.2\\glsl\\vertexShaderSource1.vert", "D:\\Study\\TA\\OGL\\OGL1.2.2\\glsl\\fragmentShaderSource1.frag");
    Shader Shader2("D:\\Study\\TA\\OGL\\OGL1.2.2\\glsl\\vertexShaderSource2.vert", "D:\\Study\\TA\\OGL\\OGL1.2.2\\glsl\\fragmentShaderSource2.frag");

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
    //注意步长为6 但是还是渲染两个三角形
    float vertices2[] = {
            //位置                        //颜色
            0.2f,0.5f,0.0f, 1.0f, 0.0f, 0.0f,
            0.2f,-0.5f,0.0f, 0.0f, 1.0f, 0.0f,
            0.7f,-0.5f,0.0f,0.0f, 0.0f, 1.0f,
            0.3f,0.5f,0.0f,1.0f, 0.0f, 0.0f,
            0.8f,-0.5f,0.0f,0.0f, 1.0f, 0.0f,
            0.8f,0.5f,0.0f,0.0f, 0.0f, 1.0f
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        double timeValue = glfwGetTime();
        float colorValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);

        Shader1.Use();
        Shader1.setVec4("ourColor",colorValue, colorValue, 0.5f, 1.0f);
        //绘制三角形
        glBindVertexArray(VAOs[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        Shader2.Use();
        glBindVertexArray(VAOs[1]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteBuffers(2, EBOs);
    Shader1.DeleteProgram();
    Shader2.DeleteProgram();
    // ------------------------------------------------------------------
    glfwTerminate();
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

