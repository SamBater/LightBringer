#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GLFW/glfw3.h>
int w = 800, h = 800;
void processInput(GLFWwindow *window, unsigned char data[])
{
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        for (int y = 0; y < w; y++)
            for (int x = 0; x < h; x++)
            {
                int pixel = w * y * 3 + x * 3;
                data[pixel] += 0x10;
                data[pixel + 1] += 0x10;
                data[pixel + 2] += 0x10;
            }
    }
}
int main(int argc, const char **argv)
{
    GLFWwindow *window;

    if (!glfwInit())
    {
        printf("Couldn't init GLFW\n");
        return 1;
    }

    window = glfwCreateWindow(w, h, "Hello World", NULL, NULL);
    if (!window)
    {
        printf("Couldn't open window\n");
        return 1;
    }

    unsigned char *data = new unsigned char[w * h * 3];
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
        {
            int pixel = w * y * 3 + x * 3;
            auto color = x > y ? 0xff : 0xcc;
            data[pixel] = 0x40;
            data[pixel + 1] = 0x2f;
            data[pixel + 2] = 0x1f;
        }

    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window, data);
        glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, data);
        glfwSwapBuffers(window);
        glfwWaitEvents();
    }
    return 0;
}
