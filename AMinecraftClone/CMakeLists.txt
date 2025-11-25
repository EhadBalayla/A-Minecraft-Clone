#find SDL2... IDK what the hell it is but the flags i have here make SDL not work when trying to cross compile, so i commented em for now, i need to learn how to use find_package ASAP
#set(SDL_STATIC ON CACHE BOOL "")
#set(SDL_SHARED OFF CACHE BOOL "")
#set(SDL_TEST OFF CACHE BOOL "")
#set(SDL_AUDIO OFF CACHE BOOL "")
#set(SDL_JOYSTICK OFF CACHE BOOL "")
#set(SDL_HAPTIC OFF CACHE BOOL "")
#set(SDL_GAMECONTROLLER OFF CACHE BOOL "")
#set(SDL_SENSOR OFF CACHE BOOL "")
#set(SDL_THREADS ON CACHE BOOL "")
#set(SDL_TIMERS ON CACHE BOOL "")
#set(SDL_EVENTS ON CACHE BOOL "")
#set(SDL_VIDEO ON CACHE BOOL "")
#set(SDL_RENDER OFF CACHE BOOL "")
#set(SDL_LOADSO OFF CACHE BOOL "")
#set(SDL_FILESYSTEM OFF CACHE BOOL "")
#set(SDL_POWER OFF CACHE BOOL "")
#set(SDL_SVG OFF CACHE BOOL "")
#set(SDL_IMAGE OFF CACHE BOOL "")
#set(SDL_TTF OFF CACHE BOOL "")
#set(SDL_MIXER OFF CACHE BOOL "")
FetchContent_Declare(
	SDL2
	GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
	GIT_TAG 	    release-2.32.6
)
FetchContent_MakeAvailable(SDL2)
set_target_properties(SDL2 PROPERTIES FOLDER "Dependencies/SDL2")

file(GLOB_RECURSE MC_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Headers/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/Src/*.c"
	
	"${CMAKE_CURRENT_SOURCE_DIR}/ImGui/imgui.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/ImGui/imgui_demo.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/ImGui/imgui_draw.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/ImGui/backends/imgui_impl_opengl3.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/ImGui/backends/imgui_impl_sdl2.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/ImGui/imgui_tables.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/ImGui/imgui_widgets.cpp"
)
add_executable(AMinecraftClone ${MC_SOURCES})

target_include_directories(AMinecraftClone PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Headers
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty
	
	${SDL2_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/ImGui
)

target_link_libraries(AMinecraftClone PRIVATE
    SDL2
    OpenGL::GL
)

set_target_properties(AMinecraftClone PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Compiled
	RUNTIME_OUTPUT_NAME "AMinecraftClone"
)