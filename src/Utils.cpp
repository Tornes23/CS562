#include <iostream>
#include <fstream>
#include <sdl2/SDL.h>
#include <gl/glew.h>
#include <tinyglft/stb_image_write.h>
#include <imgui/imgui.h>
#include "Utils.h"
#include "Window.h"
#include "GameObjectManager.h"
#include "Camera.h"
#include "RenderManager.h"
#include "ResourceManager.h"

namespace Utils
{

    void InitSDL()
    {
        //initializing the SDL library to video only
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "Could not initialize SDL: " << SDL_GetError() << "\n";
            exit(1);
        }
    }

    void InitGL()
    {
        //initializing the glew library
        if (glewInit() != GLEW_OK)
        {
            SDL_GL_DeleteContext(Window.GetSDLContext());
            SDL_DestroyWindow(Window.GetSDLWindow());
            std::cerr << "GLEW Error: Failed to init" << "\n";
            SDL_Quit();
            exit(1);
        }

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(Utils::MessageCallback, 0);

        //depth buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        //back face removal
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
    }

    void ParseShader(std::string& filename, std::string& code)
    {
        //stream to read from the file
        std::ifstream inFile(filename, std::ios::in);

        //checking if it was opened
        if (!inFile)
            std::cerr << "ERROR WHILE TRYING TO OPEN " << filename << " FILE\n";

        //reading the code from the file and adding it to the string
        while (inFile.good())
        {
            int c = inFile.get();
            if (!inFile.eof())
                code.push_back((char)c);
        }
        //closing the file
        inFile.close();
    }

    void LoadScene(const std::string& filename)
    {
        std::ifstream inFile(filename);

        nlohmann::json j;
        if (inFile.good() && inFile.is_open())
        {
            inFile >> j;
            nlohmann::json& lights = *j.find("lights");
            if (j.find("camera") != j.end())
                Camera.LoadCamera(*j.find("camera"));
            if (j.find("objects") != j.end())
                GOManager.LoadObjects(*j.find("objects"));
            if (j.find("lights") != j.end())
                RenderManager.LoadLights(*j.find("lights"));
        }

    }
    void LoadResources(const std::string& filename)
    {
        //loading the model
        ResourceManager.Load();
        //loading the level
        LoadScene(filename);
    }


    void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
        const GLchar* message, const void* userParam)
    {
        const char* _source;
        const char* _type;
        const char* _severity;

        switch (source)
        {
        case GL_DEBUG_SOURCE_API:
            _source = "API";
            break;

        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            _source = "WINDOW SYSTEM";
            break;

        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            _source = "SHADER COMPILER";
            break;

        case GL_DEBUG_SOURCE_THIRD_PARTY:
            _source = "THIRD PARTY";
            break;

        case GL_DEBUG_SOURCE_APPLICATION:
            _source = "APPLICATION";
            break;

        case GL_DEBUG_SOURCE_OTHER:
            _source = "UNKNOWN";
            break;

        default:
            _source = "UNKNOWN";
            break;
        }

        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:
            _type = "ERROR";
            break;

        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            _type = "DEPRECATED BEHAVIOR";
            break;

        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            _type = "UNDEFINED BEHAVIOR";
            break;

        case GL_DEBUG_TYPE_PORTABILITY:
            _type = "PORTABILITY";
            break;

        case GL_DEBUG_TYPE_PERFORMANCE:
            _type = "PERFORMANCE";
            break;

        case GL_DEBUG_TYPE_OTHER:
            _type = "OTHER";
            break;

        case GL_DEBUG_TYPE_MARKER:
            _type = "MARKER";
            break;

        default:
            _type = "UNKNOWN";
            break;
        }

        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            _severity = "HIGH";
            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            _severity = "MEDIUM";
            break;

        case GL_DEBUG_SEVERITY_LOW:
            _severity = "LOW";
            break;

        case GL_DEBUG_SEVERITY_NOTIFICATION:
            _severity = "NOTIFICATION";
            break;

        default:
            _severity = "UNKNOWN";
            break;
        }

        if(severity != GL_DEBUG_SEVERITY_NOTIFICATION)
            std::cerr << id << ":" << _type << " of " << _severity << " severity, raised from " << _source << ": " << message << "\n";
    }
    
    // Saves the front framebuffer to an image with the specified filename
    // Call it after swapping buffers to make sure you save the last frame rendered
    void SaveScreenshot(const std::string& output)
    {
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        int x = viewport[0];
        int y = viewport[1];
        int width = viewport[2];
        int height = viewport[3];

        std::vector<unsigned char> imageData(width * height * 4, 0);

        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadBuffer(GL_FRONT);
        glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imageData.data());
        stbi_flip_vertically_on_write(true);
        stbi_write_png(output.data(), width, height, 4, imageData.data(), 0);
    }
    void PerformanceGUI()
    {
        //creating a window
        if (!ImGui::Begin("Performance"))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }
        glm::ivec2 viewport = Window.GetViewport();
        std::string res = "Resolution: ";
        res += std::to_string(viewport.x);
        res += "x";
        res += std::to_string(viewport.y);
        ImGui::Text(res.data());

        auto frames = Window.GetFrames();
        std::string framerate = "Frame Rate: ";
        if (!frames.empty())
        {
            framerate += std::to_string(frames.back());
            ImGui::Text(framerate.data());
            ImGui::PlotLines("", &frames[0], static_cast<int>(frames.size()), 0, NULL, 0.0f, 100.0f, ImVec2(400, 100));
        }
        
        ImGui::End();
    }
}