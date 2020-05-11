#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader.h>
#include <camera.h>

#include <iostream>

// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef __APPLE__
#include <unistd.h>
#endif

#include <model.h>
#define LIGHT_TYPE_DIRECTION 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

#define MOUSE_MODE_NO_ACTION 0
#define MOUSE_MODE_TRANSLATE 1
#define MOUSE_MODE_ROTATE 2

typedef unsigned int LightType;
typedef int MouseMode;

struct Light
{
    LightType type;

    glm::vec3 position;
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
};

const float vertices[] = {
    //Pos                   //texCoord      //Normal
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};

const glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)};

glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f, -3.2f, 2.0f),
    glm::vec3(2.3f, -3.3f, -3.0f),
    glm::vec3(-3.0f, 2.0f, -4.0f),
    glm::vec3(3.0f, 2.0f, 3.0f)};

glm::vec3 pointLightsColorAmbient[] = {
    glm::vec3(0.35f, 0.35f, 0.35f),
    glm::vec3(0.35f, 0.35f, 0.35f),
    glm::vec3(0.35f, 0.35f, 0.35f),
    glm::vec3(0.35f, 0.35f, 0.35f)};

glm::vec3 pointLightsColorDiffuse[] = {
    glm::vec3(0.2f, 0.2f, 0.2f),
    glm::vec3(0.2f, 0.2f, 0.2f),
    glm::vec3(0.2f, 0.2f, 0.2f),
    glm::vec3(0.2f, 0.2f, 0.2f)};

glm::vec3 pointLightsColorSpecular[] = {
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f)};
// global functions

void writeMVP(Shader &shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection, Camera &camera);
void setupLightsScene(Shader &shader, std::vector<Light> lights);
void setupFixedLights(std::vector<Light> &lights);
Light creatMovableSpotLight(glm::vec3 position, glm::vec3 direction);
void resetScene(Camera &camera);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void initMVP(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection, Camera &camera);
void updateMVP(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection, Camera &camera);

unsigned int loadTexture(const char *path);
// settings
int SCR_WIDTH = 1200;
int SCR_HEIGHT = 900;
int NrDirLights = 1;
int NrPointLights = 4;
int NrSpotLights = 1;
glm::vec3 center=glm::vec3(0.0f, -1.0f, -10.0f);
MouseMode LeftMouseMode = MOUSE_MODE_NO_ACTION;

std::vector<Light> lights;
float deltaTime = 0.f;
float lastFrame = 0.f;
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;
int LeftMouseDown = 0;
int mouseEntered = 0;
Camera camera(glm::vec3(0.0, 0.0, 3.0));

glm::mat4 model = glm::mat4(1.0);
glm::mat4 view = glm::mat4(1.0);
glm::mat4 projection = glm::mat4(1.0);
float backgroundColor[3] = {0.19f, 0.2f, 0.23f};

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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
    glfwSwapInterval(1); // Enable vsync

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, [](GLFWwindow *window, double xoffset, double yoffset) {camera.ProcessMouseScroll(yoffset);view =camera.GetViewMatrix(); });
    glfwSetCursorEnterCallback(window, [](GLFWwindow *window, int entered) { mouseEntered = entered; });
    glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            LeftMouseDown = 1;
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            LeftMouseDown = 0;
    });

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    if (gladLoadGL() == 0)
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    // build and compile our shader zprogram
    // ------------------------------------
    // Shader cubeShader("cubes.vert", "cubes.frag");
    // Shader lightShader("lights.vert", "lights.frag");
    setupFixedLights(lights);
    Light spotLight = creatMovableSpotLight(camera.Position, camera.Front);
    lights.push_back(spotLight);

    Model nanoModel("f15/f-15.obj");
    nanoModel.scale = 0.8f;
    initMVP(model, view, projection, camera);
    Shader nanoTexShader("shaders/nano.vert", "shaders/nano_texture.frag");
    Shader nanoNoTexShader("shaders/nano.vert", "shaders/nano_no_texture.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // render the loaded model
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {

        // input
        // -----
        // std::cout<< " mouse inside window "<< std::to_string(mouseEntered) <<std::endl;
        if (mouseEntered == 0)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }

        float currFrame = glfwGetTime();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        processInput(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {

            ImGui::Begin("Lights scene");                                     // Create a window called "Hello, world!" and append into it.
            ImGui::ColorEdit3("Background Color", (float *)&backgroundColor); // Edit 3 floats representing a color
            ImGui::RadioButton("Choose", &LeftMouseMode, MOUSE_MODE_NO_ACTION);
            ImGui::RadioButton("Translate Object", &LeftMouseMode, MOUSE_MODE_TRANSLATE);
            ImGui::RadioButton("Rotate Object", &LeftMouseMode, MOUSE_MODE_ROTATE);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        ImGui::Render();

        // render nano
        // ------
        glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);
        glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        Light spotLight = creatMovableSpotLight(camera.Position, camera.Front);
        lights[lights.size() - 1] = spotLight;
        setupLightsScene(nanoTexShader, lights);
        setupLightsScene(nanoNoTexShader, lights);

        writeMVP(nanoTexShader, model, view, projection, camera);
        writeMVP(nanoNoTexShader, model, view, projection, camera);

        nanoModel.Draw(nanoTexShader, nanoNoTexShader);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        resetScene(camera);
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

// glfw cursor callback function
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    static bool firstMouse = true;
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;
    lastX = xpos;
    lastY = ypos;
    auto _direction = glm::normalize(glm::vec3(-xoffset, yoffset, 0.f));
    float _distance = sqrt((xoffset * xoffset) + (yoffset * yoffset));

    if (mouseEntered && LeftMouseDown)
    {

        switch (LeftMouseMode)
        {
        case MOUSE_MODE_TRANSLATE:
        {
            camera.ProcessMouseMovement(-xoffset, yoffset);
        }
        break;
        case MOUSE_MODE_ROTATE:
        {
            model=glm::rotate(model, glm::radians(_distance), glm::cross((center-camera.Position), _direction));
        }
        break;
        default:
            break;
        }
    }
}
void resetScene(Camera &camera)
{
    camera = Camera(glm::vec3(0.0, 0.0, 3.0));
}

unsigned int loadTexture(char const *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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

void setupFixedLights(std::vector<Light> &lights)
{
    lights.clear();
    Light dirLight;
    dirLight.type = LIGHT_TYPE_DIRECTION;
    dirLight.direction = glm::vec3(0.2f, 1.0f, 1.3f);
    dirLight.ambient = glm::vec3(0.35f, 0.35f, 0.35f);
    dirLight.diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    dirLight.specular = glm::vec3(0.5f, 0.5f, 0.5f);

    lights.push_back(dirLight);

    Light pointLights[4];
    for (int i = 0; i < 4; i++)
    {
        pointLights[i].type = LIGHT_TYPE_POINT;
        pointLights[i].position = pointLightPositions[i];
        pointLights[i].ambient = pointLightsColorAmbient[i];
        pointLights[i].specular = pointLightsColorSpecular[i];
        pointLights[i].diffuse = pointLightsColorDiffuse[i];
        pointLights[i].constant = 1.0f;
        pointLights[i].linear = 0.09;
        pointLights[i].quadratic = 0.032;
        lights.push_back(pointLights[i]);
    }
}

void setupLightsScene(Shader &shader, std::vector<Light> lights)
{
    shader.use();
    shader.setFloat("material.shininess", 32.f);
    NrDirLights = 0, NrPointLights = 0, NrSpotLights = 0;
    for (int i = 0; i < lights.size(); i++)
    {
        if (lights[i].type == LIGHT_TYPE_DIRECTION)
        {
            shader.setVec3("dirLights[" + std::to_string(NrDirLights) + "].direction", lights[i].direction);
            shader.setVec3("dirLights[" + std::to_string(NrDirLights) + "].ambient", lights[i].ambient);
            shader.setVec3("dirLights[" + std::to_string(NrDirLights) + "].diffuse", lights[i].diffuse);
            shader.setVec3("dirLights[" + std::to_string(NrDirLights) + "].specular", lights[i].specular);
            NrDirLights++;
        }
        else if (lights[i].type == LIGHT_TYPE_POINT)
        {
            shader.setVec3("pointLights[" + std::to_string(NrPointLights) + "].position", lights[i].position);
            shader.setVec3("pointLights[" + std::to_string(NrPointLights) + "].ambient", lights[i].ambient);
            shader.setVec3("pointLights[" + std::to_string(NrPointLights) + "].diffuse", lights[i].diffuse);
            shader.setVec3("pointLights[" + std::to_string(NrPointLights) + "].specular", lights[i].specular);
            shader.setFloat("pointLights[" + std::to_string(NrPointLights) + "].constant", lights[i].constant);
            shader.setFloat("pointLights[" + std::to_string(NrPointLights) + "].linear", lights[i].linear);
            shader.setFloat("pointLights[" + std::to_string(NrPointLights) + "].quadratic", lights[i].quadratic);
            NrPointLights++;
        }
        else if (lights[i].type == LIGHT_TYPE_SPOT)
        {
            shader.setVec3("spotLights[" + std::to_string(NrSpotLights) + "].direction", lights[i].direction);
            shader.setVec3("spotLights[" + std::to_string(NrSpotLights) + "].position", lights[i].position);
            shader.setVec3("spotLights[" + std::to_string(NrSpotLights) + "].ambient", lights[i].ambient);
            shader.setVec3("spotLights[" + std::to_string(NrSpotLights) + "].diffuse", lights[i].diffuse);
            shader.setVec3("spotLights[" + std::to_string(NrSpotLights) + "].specular", lights[i].specular);
            shader.setFloat("spotLights[" + std::to_string(NrSpotLights) + "].constant", lights[i].constant);
            shader.setFloat("spotLights[" + std::to_string(NrSpotLights) + "].linear", lights[i].linear);
            shader.setFloat("spotLights[" + std::to_string(NrSpotLights) + "].quadratic", lights[i].quadratic);
            shader.setFloat("spotLights[" + std::to_string(NrSpotLights) + "].cutOff", lights[i].cutOff);
            shader.setFloat("spotLights[" + std::to_string(NrSpotLights) + "].outerCutOff", lights[i].outerCutOff);
            NrSpotLights++;
        }
        else
        {
            continue;
        }
    }
    shader.setInt("NrDirLights", NrDirLights);
    shader.setInt("NrPointLights", NrPointLights);
    shader.setInt("NrSpotLights", NrSpotLights);
}

void writeMVP(Shader &shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection, Camera &camera)
{
    updateMVP(model, view, projection, camera);
    shader.use();
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setVec3("viewPos", camera.Position);
}

Light creatMovableSpotLight(glm::vec3 position, glm::vec3 direction)
{

    Light spotLight;
    spotLight.type = LIGHT_TYPE_SPOT;
    spotLight.ambient = glm::vec3(0.8f, 0.8f, 0.8f);
    spotLight.diffuse = glm::vec3(0.4f, 0.64f, 0.73f);
    spotLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    spotLight.constant = 1.0f;
    spotLight.linear = 0.09;
    spotLight.quadratic = 0.032;

    spotLight.cutOff = glm::cos(glm::radians(7.5f));
    spotLight.outerCutOff = glm::cos(glm::radians(9.0f));

    spotLight.direction = direction;
    spotLight.position = position;
    return spotLight;
}
void initMVP(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection, Camera &camera)
{
    model = glm::mat4(1.0f);
    // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.f), glm::vec3(0.f, 1.f, 0.f));
    model = glm::translate(model, center); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(0.8));
    projection = glm::mat4(1.0);
    projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
}
void updateMVP(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection, Camera &camera)
{
    view = camera.GetViewMatrix();
    projection = glm::mat4(1.0);
    projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
}