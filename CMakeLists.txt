# Engine impl (OGRE) project
cmake_minimum_required(VERSION 3.17)
project(HPMSEngineOgreImpl)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_SHARED_LIBRARY_PREFIX)
set(CMAKE_STATIC_LIBRARY_PREFIX)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(OGRE_VERSION v1.12.9)
set(PODS_VERSION master)
set(HPMSAPI_VERSION main)

if (NOT DEFINED DEP_PATH)
    set(DEP_PATH ${CMAKE_CURRENT_BINARY_DIR}/_deps)
endif ()

string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
if (BUILD_TYPE STREQUAL "debug")
    set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Debug Postfix for lib, samples and tools")
else ()
    set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "Debug Postfix for lib, samples and tools")
endif ()


include(cmake/Shared.cmake)


# Other libraries setup.
install_from_repo(https://github.com/mtrempoltsev/pods.git PODS ${PODS_VERSION} FALSE /include/)

install_from_repo(https://github.com/Ray1184/HPMSApi.git HPMSAPI ${HPMSAPI_VERSION} TRUE /include/)

# Ogre setup.
set(OGRE_BUILD_PATH ${PROJECT_BINARY_DIR}/_deps/ogre-build)
set(CORE_INCLUDES "/OgreMain/include/")
set(COMPONENTS_INCLUDES "/Components/Overlay/include/;/Components/Property/include/")
set(PLUGINS_INCLUDES "/PlugIns/STBICodec/include/;/PlugIns/ParticleFX/include/")
set(DEP_INCLUDES "/RenderSystems/GL/include/")
set(INCLUDES ${CORE_INCLUDES};${COMPONENTS_INCLUDES};${PLUGINS_INCLUDES};${RENDERER_INCLUDES};${DEP_INCLUDES})
install_from_repo(https://github.com/OGRECave/ogre.git OGRE ${OGRE_VERSION} TRUE "${INCLUDES}")
include_directories(${OGRE_BUILD_PATH}/include)
include_directories(${OGRE_BUILD_PATH}/Dependencies/include)

# SDL2 setup.
find_package(SDL2 REQUIRED)

include_directories(include)

include_directories(${DEP_PATH}/dynalo-src/include/dynalo)
include_directories(${DEP_PATH}/glm-src)

set(SOURCES
        src/input/HPMSInputHandler.cpp
        src/common/HPMSUtils.cpp
        src/core/HPMSOgreContext.cpp
        src/thirdparty/Vector2D.cpp
        src/thirdparty/InputHelper.cpp
        src/resource/HPMSLuaScript.cpp
        src/resource/HPMSLuaScriptManager.cpp
        src/resource/HPMSWalkmap.cpp
        src/resource/HPMSWalkmapManager.cpp
        src/core/HPMSRenderToTexture.cpp
        src/core/HPMSEntityAdaptee.cpp
        src/core/HPMSAnimationAdaptee.cpp
        src/core/HPMSSceneNodeAdaptee.cpp
        src/core/HPMSCameraAdaptee.cpp
        src/core/HPMSBackgroundImageAdaptee.cpp
        src/core/HPMSOverlayImageAdaptee.cpp
        src/core/HPMSLightAdaptee.cpp
        src/core/HPMSSupplierAdaptee.cpp
        src/core/HPMSSimulatorAdaptee.cpp)

add_definitions(-DBACKEND_IMPL_NAME="Ogre 3D ${OGRE_VERSION}")

add_library(HPMSEngineOgreImpl SHARED ${SOURCES})


#add_executable(HPMSEngineOgreImpl src/Main.cpp ${SOURCES})
target_link_libraries(HPMSEngineOgreImpl OgreMain OgreOverlay RenderSystem_GL Codec_STBI ${SDL2_LIBRARY})


add_custom_command(TARGET HPMSEngineOgreImpl
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/ext/ ${CMAKE_CURRENT_BINARY_DIR}/engine/
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/data/ ${CMAKE_CURRENT_BINARY_DIR}/data/
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${OGRE_BUILD_PATH}/bin/ ${CMAKE_CURRENT_BINARY_DIR}/engine/
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/${BUILD_TYPE}/ ${CMAKE_CURRENT_BINARY_DIR}/engine/
        COMMAND ${CMAKE_COMMAND} -E rm -Rf ${CMAKE_CURRENT_BINARY_DIR}/debug
        COMMAND ${CMAKE_COMMAND} -E rm -Rf ${CMAKE_CURRENT_BINARY_DIR}/release
        COMMAND ${CMAKE_COMMAND} -E rm -Rf ${CMAKE_CURRENT_BINARY_DIR}/minsizerel
        COMMAND ${CMAKE_COMMAND} -E rm -Rf ${CMAKE_CURRENT_BINARY_DIR}/relwithdebinfo
        COMMAND ${CMAKE_COMMAND} -E rm ${CMAKE_CURRENT_BINARY_DIR}/plugins.cfg
        COMMAND ${CMAKE_COMMAND} -E rm ${CMAKE_CURRENT_BINARY_DIR}/resources.cfg
        COMMAND ${CMAKE_COMMAND} -E rm ${CMAKE_CURRENT_BINARY_DIR}/samples.cfg
        COMMAND ${CMAKE_COMMAND} -E rm ${CMAKE_CURRENT_BINARY_DIR}/tests.cfg
        )
