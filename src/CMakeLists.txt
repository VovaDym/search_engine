cmake_minimum_required(VERSION 3.25)
project(searchengine VERSION 0.1)


include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)
include_directories(${json_SOURCE_DIR}/include)

set(JSON_BuildTests OFF CACHE INTERNAL "")

include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

FetchContent_Declare(
        thread-pool
        GIT_REPOSITORY https://github.com/bshoshany/thread-pool.git
        GIT_TAG        737c6036103ac034682ee11c31634ebe6769781f # tag: v2.0.0
)
FetchContent_MakeAvailable(thread-pool)

include_directories(${thread-pool_SOURCE_DIR})





add_executable(searchengine searchengine.cpp
                            converterJSON.cpp
                            invertedindex.cpp
                            searchserver.cpp
        )


target_include_directories (searchengine PUBLIC ${MY_INCLUDE_PATH})

target_link_libraries(searchengine nlohmann_json::nlohmann_json)




