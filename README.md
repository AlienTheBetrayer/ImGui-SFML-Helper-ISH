# ISH
ImGui SFML Helper or ISH is a simple header-only helper library that allows you to easily make your own UI on a regular blank SFML window. You can use this library like an UI framework.

## Setup
To use this method you have to have MSVC compiler.
1. Build ImGui-SFML and SFML as dynamic link libraries(preferably using **vcpkg**);
2. In **ish/settings.hpp** change the **LIB_PATH** to the path where you have all the **.lib** files;
3. Put all the DLLs in the same path where your executable is;
4. Set include path to a folder that contains all the header files;
5. Now you can just do #include "ish/ish.hpp";
6. Done!
## Example
```cpp
#include "ish/ish.hpp"

int main() {
	// Construct and initialize everything
	auto window = ish::construct_window_frame({ 300, 150 });
	ish::init_imgui(window);

	// Hide the console
	ish::hide_console();

	// Logic loop
	for(; ish::is_sfml_open(window) ;) {
		// Handling events and updating
		ish::handle_events(window, [](sf::Event&  event) { });
		ish::update_imgui(window);

		// The code will run if a window is closed
		if(!ish::begin_imgui("ISH Test", ish::default_flags)) {
			ImGui::End();
			return  0;
		}

		// All your ImGui UI code is here...

		ImGui::End();

		// Rendering UI
		ish::render(window);
	}

	// Shutdown
	ish::shutdown_imgui();
}
```
How it looks:
![enter image description here](https://raw.githubusercontent.com/AlienTheBetrayer/ImGui-SFML-Helper-ISH-/main/example.png)