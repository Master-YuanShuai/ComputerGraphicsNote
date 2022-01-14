#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
using namespace std;

//调整窗口的回调函数 此回调函数会在窗口每次大小被调整的时候调用
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
    std::cout << "ERROR: " << err_str << std::endl;
}

int main() {

    glfwInit(); //初始化GLFW
    // 此处确保当用户的OGL版本不吻合时无法运行
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3); //配置GLFW 主版本号设为3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3); //配置GLFW 次版本号设为3
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);//配置GLFW 使用的是核心模式 且只能使用OGL功能的一个子集
    // glfwWindowHint(GLFW_DECORATED, GL_FALSE);//设置无边框
    //Mac os系统打开这行 glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //创建一个窗口对象 存放有关窗口的相关数据
    GLFWwindow* window = glfwCreateWindow(800, 600, "OGLobj", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //调用任何ogl函数前先初始化glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //注册修改窗口参数的回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //渲染一个简单的循环Window界面 类似于Winmain
    //此处就是渲染循环了 我们要把渲染的代码放入渲染循环中
    while(!glfwWindowShouldClose(window))
    {
        //输入
        processInput(window);

        //Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //设置清空屏幕的颜色
        glClear(GL_COLOR_BUFFER_BIT);//清空状态 使用的是设置的颜色
        //
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    //释放之前调用的资源
    glfwTerminate();

    //捕捉异常
    glfwSetErrorCallback(error_callback);
    return 0;

}
