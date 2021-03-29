cmake_minimum_required(VERSION 3.10.0)
project(percipio_cam_demo)

enable_language(CSharp)
set(CMAKE_CSharp_FLAGS "/langversion:default")

if(CMAKE_CL_64)
    set(CMAKE_CSharp_FLAGS "/platform:x64")
else()
    set(CMAKE_CSharp_FLAGS "/platform:x86")
endif()

file(GLOB CS_SUPPORT_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cs)

add_library(percipio_cam_demo SHARED ${CS_SUPPORT_FILES})

# .NET reference libraries.
set (REF_LIST 
    "Microsoft.CSharp"
    "System"
    "System.Core"
    "System.Data"
    "System.Deployment"
    "System.Drawing"
    "System.Xml"
)
set_property(TARGET percipio_cam_demo PROPERTY VS_DOTNET_REFERENCES ${REF_LIST})
set_property(TARGET percipio_cam_demo PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.0")

add_executable(percipio_test ${CMAKE_CURRENT_SOURCE_DIR}/percipio_test.cs)
set_property(TARGET percipio_test PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.0")
set_property(TARGET percipio_test PROPERTY VS_DOTNET_REFERENCES ${REF_LIST} )

add_dependencies(percipio_test percipio_cam_demo)
target_link_libraries(percipio_test percipio_cam_demo)

install (TARGETS percipio_test DESTINATION  "./")
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/percipio_test.cs DESTINATION  "./")

