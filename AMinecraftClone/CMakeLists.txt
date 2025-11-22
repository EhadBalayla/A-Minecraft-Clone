#find SDL2
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
)
add_executable(AMinecraftClone ${MC_SOURCES})

target_include_directories(AMinecraftClone PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Headers
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty
	
	${SDL2_SOURCE_DIR}/include
)

target_link_libraries(AMinecraftClone PRIVATE
    SDL2
    OpenGL::GL
)

set_target_properties(AMinecraftClone PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Compiled
	RUNTIME_OUTPUT_NAME "AMinecraftClone"
)