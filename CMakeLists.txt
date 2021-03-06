cmake_minimum_required(VERSION 3.0)

project(296Zvonarev)

# Поиск зависимостей
set(DEPENDENCIES_ROOT CACHE PATH "Dependencies install path")
if (DEPENDENCIES_ROOT)
	include(${DEPENDENCIES_ROOT}/lib/cmake/glew/glew-targets.cmake)
	include(${DEPENDENCIES_ROOT}/lib/cmake/glfw/glfwTargets.cmake)
	include(${DEPENDENCIES_ROOT}/lib/cmake/glm/glmTargets.cmake OPTIONAL)
	include(${DEPENDENCIES_ROOT}/lib64/cmake/glm/glmTargets.cmake OPTIONAL)
	include(${DEPENDENCIES_ROOT}/lib/cmake/assimp-3.2/assimpTargets.cmake)
	include(${DEPENDENCIES_ROOT}/lib/cmake/soil/soilTargets.cmake)
else ()
	message("Set DEPENDENCIES_ROOT variable")
endif ()


# Исходные файлы
set(SRC_FILES
	Main.cpp
        Application.cpp
        KleinBottleApplication.cpp
        Mesh.cpp
        ShaderProgram.cpp
        Texture.cpp
)

set(HEADER_FILES
        Application.hpp
        Common.h
        KleinBottleApplication.h
        Mesh.hpp
        ShaderProgram.hpp
        Texture.hpp
)

set(SHADER_FILES
	296ZvonarevData/LightsAndTexture.vert
	296ZvonarevData/LightsAndTexture.frag
        296ZvonarevData/Marker.frag
        296ZvonarevData/Marker.vert
)

# Создаем исполняемый файл
add_executable(296Zvonarev ${SRC_FILES} ${HEADER_FILES})
	
# Линкуем зависимости
target_link_libraries(296Zvonarev GLEW::glew_s glm glfw ASSIMP::assimp SOIL::soil)

# Копируем готовый исполняемый файл в папку ${CMAKE_INSTALL_PREFIX}
install(TARGETS 296Zvonarev RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})

# Копируем папку с ресурсами в папку ${CMAKE_INSTALL_PREFIX}
install(DIRECTORY 296ZvonarevData DESTINATION ${CMAKE_INSTALL_PREFIX})