#---------------------------------------------------------
#   I copied most of this CMakeLists.txt from: https://cliutils.gitlab.io/modern-cmake/examples/root-dict/README.html 
cmake_minimum_required(VERSION 3.11)

#---------------------------------------------------------
#   Define the project 
project(3dInteractiveHist VERSION 0.1
        LANGUAGES CXX)  

#---------------------------------------------------------
set(CMAKE_CXX_STANDARD
    17
    CACHE STRING "C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

#---------------------------------------------------------
#   Find ROOT (necessary)
find_package(ROOT 6.20 REQUIRED COMPONENTS 
    Core 
    RIO 
    Physics
    ROOTDataFrame
    Gui
)

#---------------------------------------------------------
#   Define sources and headers
set(sources 
    InteractiveHistApp.cxx
)

set(headers 
    InteractiveHistApp.hxx
)

#---------------------------------------------------------
#   Generate the ROOT dictionary
root_generate_dictionary(G__3dInteractiveHist ${headers} LINKDEF LinkDef.hxx)

#---------------------------------------------------------
#   Add Libraries
add_library(3dInteractiveHist SHARED ${headers} ${sources} G__3dInteractiveHist.cxx)
target_include_directories(3dInteractiveHist PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(3dInteractiveHist PUBLIC ROOT::Core)
