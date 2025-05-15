# GLFW: 3.4 - 3.28
CMAKE_MINIMUM_REQUIRED(VERSION 3.4...3.28 FATAL_ERROR)
SET(BUILD_SHARED_LIBS OFF) # We expect external libraries to be linked statically.
SET(CMAKE_CXX_STANDARD 17) # Compile as C++17.
SET(CMAKE_CXX_STANDARD_REQUIRED ON) # Require C++17 support.

PROJECT(OpenGL
        VERSION 0.0.1
        LANGUAGES C CXX
)

# #########################################################
# GLFW
# #########################################################
ADD_SUBDIRECTORY(third-party/glfw-3.4)
INCLUDE_DIRECTORIES(third-party/glfw-3.4/include)


# #########################################################
# GLEW
# #########################################################
ADD_SUBDIRECTORY(third-party/glew-2.1.0/build/cmake)
INCLUDE_DIRECTORIES(third-party/glew-2.1.0/include)

# #########################################################
# GLM (header-only)
# #########################################################
INCLUDE_DIRECTORIES(third-party/glm)

# #########################################################
# Include
# #########################################################
INCLUDE_DIRECTORIES(src/include)

# #########################################################
# Source
# #########################################################
AUX_SOURCE_DIRECTORY(src SRC_LIST)

# #########################################################
# TARGET: APP
# #########################################################
ADD_EXECUTABLE(app ${SRC_LIST})
TARGET_LINK_LIBRARIES(app glfw glew)

