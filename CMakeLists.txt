#---------------------------------------------------------
#   I copied most of this CMakeLists.txt from: https://cliutils.gitlab.io/modern-cmake/examples/root-dict/README.html 
cmake_minimum_required(VERSION 3.11)

#---------------------------------------------------------
#   Define the project 
project(Interactive3dHist VERSION 0.1
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
    Interactive3dHist.cxx
)

set(headers 
    Interactive3dHist.hxx
)

#---------------------------------------------------------
#   Generate the ROOT dictionary
root_generate_dictionary(G__Interactive3dHist ${headers} LINKDEF LinkDef.hxx)

#---------------------------------------------------------
#   Add Libraries
add_library(Interactive3dHist SHARED ${headers} ${sources} G__Interactive3dHist.cxx)
target_include_directories(Interactive3dHist PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(Interactive3dHist PUBLIC ROOT::Core)
