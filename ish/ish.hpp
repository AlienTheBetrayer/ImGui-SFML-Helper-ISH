// helper.hpp
//
// ImGui SFML Helper
//
// A simple header-only library.
// This library is supposed to be used to create UI only windows.
//
// Note: Make sure to have all the DLLs in the same place where your executable is.

#pragma once

// Including all the libraries

// ISH's dependencies
#include "./settings.hpp"
#include "./macros.hpp"

// ImGui
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui-SFML.h>

// SMFL
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

// Linking the DLLs
// SFML
#pragma comment(lib, LIB_PATH"sfml-system.lib")
#pragma comment(lib, LIB_PATH"sfml-window.lib")
#pragma comment(lib, LIB_PATH"sfml-graphics.lib")
#pragma comment(lib, LIB_PATH"sfml-audio.lib")
#pragma comment(lib, LIB_PATH"sfml-network.lib")

// ImGui
#pragma comment(lib, LIB_PATH"imgui.lib")
#pragma comment(lib, LIB_PATH"ImGui-SFML.lib")
#pragma comment(lib, LIB_PATH"OpenGL32.Lib")

// Other libraries
#include <string>
#include <functional>
#include <version>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace ish {
    // --- === Vector2 Struct === --- \\ 
#ifdef __cpp_lib_concepts
    template<typename T>
    concept integral = std::is_integral_v<T>;
#define vector_type integral
#else // __cpp_lib_concepts
#define vector_type typename
#endif // __cpp_lib_concepts

    template<vector_type T>
    struct Vector2 {
        T x = 0;
        T y = 0;
    };
    
    // --- ===== --- --- ===== --- Other --- ===== --- --- ===== --- \\ 

#ifdef _WIN32
    /**
     * @brief Hides the Windows console.
     * @retval None
     */
    void hide_console(void) noexcept { 
        ShowWindow(GetConsoleWindow(), SW_HIDE);
    }
#endif

    // --- ===== --- --- ===== --- SFML + ImGui Related --- ===== --- --- ===== --- \\ 

    /**
     * @brief Handles the dragging of the window. Shouldn't be called as this gets called in the handle_events function. 
     * @param  window: SFML window
     * @param  event: SFML event
     * @retval None
     */
    void handle_move(sf::RenderWindow& window, const sf::Event& event) noexcept {
        static const sf::Rect<int32_t> pos = { 0, 0, static_cast<int32_t>(window.getSize().x), 20 };
        static sf::Vector2i grabbedOffset;
        static bool grabbedWindow = false;

        switch (event.type) {
        case sf::Event::MouseButtonPressed:
            if (pos.contains({event.mouseButton.x, event.mouseButton.y}) && event.mouseButton.button == sf::Mouse::Left) {
                grabbedOffset = window.getPosition() - sf::Mouse::getPosition();
                grabbedWindow = true;
            }    
            break;
        case sf::Event::MouseButtonReleased:
            if (event.mouseButton.button == sf::Mouse::Left)
                grabbedWindow = false;
            break;
        case sf::Event::MouseMoved:
            if (grabbedWindow)
                window.setPosition(sf::Mouse::getPosition() + grabbedOffset);
            break;
        }
    }

    /**
     * @brief Handles the events.
     * @param  window: SFML window
     * @param  callback_func: Callback function that will be called when an event is polled
     * @retval None
     */
    void handle_events(sf::RenderWindow& window, const std::function<void(sf::Event&)>& callback_func) noexcept {
        for(static sf::Event event; window.pollEvent(event) ;) {
            ImGui::SFML::ProcessEvent(event);

            if(event.type == sf::Event::Closed)
                window.close();

            handle_move(window, event);
            callback_func(event);
        }
    }

    /**
     * @brief Constructs a new window frame that's gonna be used for the main ImGui UI.
     * @param  window_size: Window's size
     * @param  window_name: Window's title string
     * @retval A constructed window frame
     */
    [[nodiscard]] sf::RenderWindow construct_window_frame(const ish::Vector2<uint32_t>& window_size = { 900, 400 }, const std::string& window_name = "SFML Window") noexcept {
        return sf::RenderWindow(sf::VideoMode(window_size.x, window_size.y), window_name, sf::Style::None);
    }

    /**
     * @brief Renders the SFML window and ImGui UI.
     * @param  window: SFML window
     * @retval None
     */
    void render(sf::RenderWindow& window) noexcept {
        window.clear();
        ImGui::SFML::Render();
        window.display();
    }

    // --- ===== --- --- ===== --- ImGui Related --- ===== --- --- ===== --- \\ 
    /**
     * @brief Initializes the ImGui window.
     * @param  window: 
     * @retval None
     */
    void init_imgui(sf::RenderWindow& window) noexcept {
        ImGui::SFML::Init(window, { static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y) });
    }

    /**
     * @brief Updates the ImGui window and snaps it to match the SFML window.
     * @param  window: SFML window
     * @retval None
     */
    void update_imgui(sf::RenderWindow& window) noexcept {
        static sf::Clock delta_clock;
        ImGui::SFML::Update(window, delta_clock.restart());;
        ImGui::SetNextWindowSize({ static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y) });
        ImGui::SetNextWindowPos({ 0, 0 });
    }
    
    /**
     * @brief Default flags for an ImGui window.
     */
    constexpr inline const int32_t default_flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

    /**
     * @brief Begins an ImGui window
     * @param  window_title: ImGui window's title string
     * @param  flags: The flags for the ImGui window
     * @retval Whether the window is closed or not. (false means closed)
     */
    bool begin_imgui(const std::string& window_title, const int32_t flags = default_flags) noexcept {
        static bool show = true;
        ImGui::Begin(window_title.c_str(), &show, flags);
        return show;
    }

    /**
     * @brief Shutdowns the ImGui UI.
     * @retval None
     */
    void shutdown_imgui(void) noexcept {
        ImGui::SFML::Shutdown();
    }

    // --- ===== --- --- ===== --- SFML Related --- ===== --- --- ===== --- \\ 
    /**
     * @brief Returns a boolean that indicates whether the window is open or not. 
     * @param  window: SFML window
     * @retval Whether the window is open or not
     */
    [[nodiscard]] bool is_sfml_open(sf::RenderWindow& window) noexcept {
        return window.isOpen();
    }
}   