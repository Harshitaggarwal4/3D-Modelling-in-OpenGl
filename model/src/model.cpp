#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <bits/stdc++.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 aCol;\n"
                                 "uniform mat4 model;\n"
                                 "uniform mat4 view;\n"
                                 "uniform mat4 proj;\n"
                                 "out vec3 outCol;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = proj*view*model*vec4(aPos,1.0);\n"
                                 "   outCol = aCol;\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "in vec3 outCol;\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(outCol, 1.0);\n"
                                   "}\n\0";

int main()
{
    int n;
    cout << "Input the number of edges:" << endl;
    cin >> n;
    if (n < 2)
    {
        cout << "Please enter bigger value of n." << endl;
        return 0;
    }
    vector<float> x, y;
    for (int i = 0; i < n; i++)
    {
        x.push_back((float)0.5 * sin(((float)6.28 / n) * (i)));
        y.push_back((float)0.5 * cos(((float)6.28 / n) * (i)));
    }
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
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
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
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[n * 2 * 6 + 6];
    for (int i = 0; i < n; i++)
    {
        vertices[i * 12 + 0] = x[i];
        vertices[i * 12 + 1] = y[i];
        vertices[i * 12 + 2] = (float)0.5;
        vertices[i * 12 + 3] = (float)i / n;
        vertices[i * 12 + 4] = (float)((i + 1) % n) / n;
        vertices[i * 12 + 5] = (float)((i + 2) % n) / n;
        vertices[i * 12 + 6] = x[i];
        vertices[i * 12 + 7] = y[i];
        vertices[i * 12 + 8] = (float)-0.5;
        vertices[i * 12 + 9] = (float)((i + 3) % n) / n;
        vertices[i * 12 + 10] = (float)((i + 4) % n) / n;
        vertices[i * 12 + 11] = (float)((i + 5) % n) / n;
    }
    vertices[n * 12] = 0.0f;
    vertices[n * 12 + 1] = 0.0f;
    vertices[n * 12 + 2] = -0.5f;
    vertices[n * 12 + 3] = 0.0f;
    vertices[n * 12 + 4] = 0.0f;
    vertices[n * 12 + 5] = 0.0f;
    // for (int i = 0; i < n * 2 + 1; i++)
    // {
    //     for (int j = 0; j < 6; j++)
    //     {
    //         cout << vertices[i * 6 + j] << " ";
    //     }
    //     cout << endl;
    // }
    unsigned int indices[(n - 2) * 6 + n * 6];
    for (int i = 0; i < n - 2; i++)
    {
        indices[i * 6] = 0;
        indices[i * 6 + 1] = ((i + 1) * 2) % (2 * n);
        indices[i * 6 + 2] = ((i + 2) * 2) % (2 * n);
        indices[i * 6 + 3] = 1;
        indices[i * 6 + 4] = ((i + 1) * 2 + 1) % (2 * n);
        indices[i * 6 + 5] = ((i + 2) * 2 + 1) % (2 * n);
    }
    for (int i = 0; i < n; i++)
    {
        indices[(n - 2) * 6 + i * 6 + 0] = i * 2 % (2 * n);
        indices[(n - 2) * 6 + i * 6 + 1] = ((i + 1) * 2) % (2 * n);
        indices[(n - 2) * 6 + i * 6 + 2] = (i * 2 + 1) % (2 * n);
        indices[(n - 2) * 6 + i * 6 + 3] = ((i * 2) + 1) % (2 * n);
        indices[(n - 2) * 6 + i * 6 + 4] = ((i + 1) * 2 + 1) % (2 * n);
        indices[(n - 2) * 6 + i * 6 + 5] = ((i + 1) * 2) % (2 * n);
    }
    unsigned int indices2[(n - 2) * 3 + n * 3];
    for (int i = 0; i < n - 2; i++)
    {
        indices2[i * 3] = 0;
        indices2[i * 3 + 1] = ((i + 1) * 2) % (2 * n);
        indices2[i * 3 + 2] = ((i + 2) * 2) % (2 * n);
    }
    for (int i = 0; i < n; i++)
    {
        indices2[(n - 2) * 3 + i * 3 + 0] = i * 2 % (2 * n);
        indices2[(n - 2) * 3 + i * 3 + 1] = ((i + 1) * 2) % (2 * n);
        indices2[(n - 2) * 3 + i * 3 + 2] = n * 2;
    }

    unsigned int VBO, EBO, VAO, EBO2;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &EBO2);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    // init the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // init the EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

    // declare attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    glm::vec3 position;
    glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    float speed = 0.1f;
    float sensitivity = 100.0f;
    float rotation = 0;
    bool pi_or_pr = false;
    bool wait = false;
    bool ignore = false;
    bool look = false;

    glEnable(GL_DEPTH_TEST);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);

        // float timeValue = glfwGetTime();
        // float greenValue = sin(timeValue) / 2.0f + 0.5f;
        // int vertexColorLocation = glGetUniformLocation(shaderProgram, "greenCol");
        // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        if (look == false)
        {
            view = glm::lookAt(position, position + orientation, up);
        }
        else
        {
            view = glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), up);
        }
        proj = glm::perspective(glm::radians(45.0f), (float)(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);
        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projLoc = glGetUniformLocation(shaderProgram, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
        if (wait == true)
        {
            sleep(1);
            wait = false;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            position += speed * orientation;
            look = true;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            position += speed * -glm::normalize(glm::cross(orientation, up));
            look = true;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            position += speed * -orientation;
            look = true;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            position += speed * glm::normalize(glm::cross(orientation, up));
            look = true;
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            position += speed * up;
            look = true;
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            position += speed * -up;
            look = true;
        }
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        {
            position += speed * orientation;
            look = false;
        }
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        {
            position += speed * -glm::normalize(glm::cross(orientation, up));
            look = false;
        }
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        {
            position += speed * -orientation;
            look = false;
        }
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        {
            position += speed * glm::normalize(glm::cross(orientation, up));
            look = false;
        }
        if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        {
            position += speed * up;
            look = false;
        }
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        {
            position += speed * -up;
            look = false;
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            rotation += 0.5f;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            speed = 0.4f;
        }
        else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
        {
            speed = 0.1f;
        }
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        {
            if (ignore == true)
            {
                ignore = false;
            }
            else
            {
                ignore = true;
                if (pi_or_pr == false)
                {
                    pi_or_pr = true;
                }
                else
                {
                    pi_or_pr = false;
                }
                wait = true;
            }
        }

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        if (pi_or_pr == false)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        }
        else
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);
            glDrawElements(GL_TRIANGLES, sizeof(indices2) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        }
        glBindVertexArray(0);
        // glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &EBO2);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}