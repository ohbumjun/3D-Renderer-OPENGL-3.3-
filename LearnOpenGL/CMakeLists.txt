# CONFIGURE_DEPENDS : when file is added or removed, cmake will be reconfigured

file(GLOB LEARNOPENGL_SOURCES 
"./*.cpp"
"./*.h"
"./*.glsl"
${CMAKE_SOURCE_DIR}/external/stb_image/stb_image.h
${CMAKE_SOURCE_DIR}/external/stb_image/stb_image.cpp
)
# Third Party
SET(THIRDPARTY_SPDLOG)
SOURCE_GROUP("${CMAKE_SOURCE_DIR}/external/spdlog/include" FILES ${THIRDPARTY_SPDLOG})

SET(THIRDPARTY_GLFW)
SOURCE_GROUP("${CMAKE_SOURCE_DIR}/external/GLFW/include" FILES ${THIRDPARTY_GLFW})

SET(THIRDPARTY_GLAD)
SOURCE_GROUP("${CMAKE_SOURCE_DIR}/external/Glad/include" FILES ${THIRDPARTY_GLAD})

SET(THIRDPARTY_IMGUI)
SOURCE_GROUP("${CMAKE_SOURCE_DIR}/external/imgui" FILES ${THIRDPARTY_IMGUI})

SET(THIRDPARTY_GLM)
SOURCE_GROUP("${CMAKE_SOURCE_DIR}/external/glm" FILES ${THIRDPARTY_GLM})

SET(THIRDPARTY_STBIMAGE)
SOURCE_GROUP("${CMAKE_SOURCE_DIR}/external/stb_image" FILES ${THIRDPARTY_STBIMAGE})

SET(THIRDPARTY_ENTT)
SOURCE_GROUP("${CMAKE_SOURCE_DIR}/external/entt" FILES ${THIRDPARTY_ENTT})

SET(THIRDPARTY_RAPIDJSON)
SOURCE_GROUP("${CMAKE_SOURCE_DIR}/external/RapidJson" FILES ${THIRDPARTY_RAPIDJSON})

SET(THIRDPARTY_IMGUIZMO)
SOURCE_GROUP("${CMAKE_SOURCE_DIR}/external/ImGuizmo" FILES ${THIRDPARTY_IMGUIZMO})

SET(THIRDPARTY_BOX2D)
SOURCE_GROUP("${CMAKE_SOURCE_DIR}/external/Box2D/include" FILES ${THIRDPARTY_BOX2D})

SET(THIRDPARTY_ASSIMP)
SOURCE_GROUP("${CMAKE_SOURCE_DIR}/external/assimp" FILES ${THIRDPARTY_ASSIMP})

set(LEARN_OPENGL_INCLUDES
  # "${CMAKE_SOURCE_DIR}/BJGameEngine/src"
   "${CMAKE_SOURCE_DIR}/external/spdlog/include"
   "${CMAKE_SOURCE_DIR}/external/GLFW/include"
   "${CMAKE_SOURCE_DIR}/external/Glad/include"
   "${CMAKE_SOURCE_DIR}/external/imgui"
   "${CMAKE_SOURCE_DIR}/external/glm"
   "${CMAKE_SOURCE_DIR}/external/stb_image"
   "${CMAKE_SOURCE_DIR}/external/entt/include"
   "${CMAKE_SOURCE_DIR}/external/RapidJson"
   "${CMAKE_SOURCE_DIR}/external/assimp"
  # "${CMAKE_SOURCE_DIR}/external/ImGuizmo"
  # "${CMAKE_SOURCE_DIR}/external/Box2D/include"
  # "${CMAKE_CURRENT_SOURCE_DIR}/public" 
   )


add_executable(
    ${LEARN_OPENGL_NAME} 

    ${LEARNOPENGL_SOURCES}
    ${THIRDPARTY_SPDLOG}
    ${THIRDPARTY_GLFW}
    ${THIRDPARTY_GLAD}
    ${THIRDPARTY_IMGUI}
    ${THIRDPARTY_GLM}
    ${THIRDPARTY_STBIMAGE}
    ${THIRDPARTY_ENTT}
    ${THIRDPARTY_RAPIDJSON}
    ${THIRDPARTY_IMGUIZMO}
    ${THIRDPARTY_BOX2D}
    ${THIRDPARTY_ASSIMP}
)

target_compile_definitions(
    ${LEARN_OPENGL_NAME} 
    PUBLIC 
    _CRT_SECURE_NO_WARNINGS 
    GLFW_INCLUDE_NONE
    _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
    RESOURCE_ROOT="${CMAKE_SOURCE_DIR}/BJResource/"
    PROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}"
    # DLL
    HZ_DYNAMIC_LINK
    )

# �ش� ������Ʈ�� include ��θ� add ���ִ� ����̴�. 
# - �ش� ������Ʈ���� ����ϴ� header ��� �߰�
# - PUBLIC       : ���� target �Ӹ� �ƴ϶�, �ش� ��ο� dependent �� target ���� �߰�
# - PRIVATE     : ���� target only (���Ӽ� ���� X)
# - INTERFACE : ���� target X, �ش� ��ο� dependent �� target ���� �߰�
target_include_directories(
    ${LEARN_OPENGL_NAME} 
    PUBLIC 
    ${LEARN_OPENGL_INCLUDES})

# link library to own library
target_link_libraries(
    ${LEARN_OPENGL_NAME}
    PUBLIC
    Ws2_32.lib
    # find project name in actual cmakefile
    # box2d  
    # imgui
    # ImGuizmo
    # stb_image
    Glad
    glm
    opengl32.lib
    glfw
    spdlog
    # project name 이 아니라면, 바로 lib 을 link 한다.
   "${CMAKE_SOURCE_DIR}/external/assimp/assimpd.lib"
    # -fsanitize=address
    )

# �Ʒ� �� ���� ���ؼ� �ش� ��ο� �ִ� ��� cpp ���ϵ��� pch �� ������� ���� ���̶�� �ϴ� ���̴�
# �׷��� hazel engine �� ���� hzpch.cpp �� ������ ��� cpp ���Ͽ��� include �ϰ� �س��µ�
# �̰��� imgui �ڵ���� ���� �ʵ��� �ϴ� ���̴�.
set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/external/ImGuizmo/**.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb_image/**.cpp
    PROPERTIES COMPILE_FLAGS "/Yc;pch.h"
)

# Set the output directory of the dll, exe
set_target_properties(${LEARN_OPENGL_NAME} 
    PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY
    ${RUNTIME_OUTPUT_DIR}
)

add_custom_command(
    TARGET ${LEARN_OPENGL_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy 
   "${CMAKE_SOURCE_DIR}/external/assimp/assimpd.dll"
    "${RUNTIME_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}"
    COMMENT "Copying data file: ${CMAKE_SOURCE_DIR}/external/assimp/assimpd.dll to ${RUNTIME_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}"
)

# apply clang-tidy
#if(${ENABLE_CLANG_TIDY})
#    add_clang_tidy_to_target(${ENGINE_NAME})
#endif()


if(MSVC)
    message(STATUS "CMAKE_BUILD_TYPE : ${CMAKE_BUILD_TYPE}")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")

    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")

    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        
    endif()
endif()
