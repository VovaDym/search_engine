# 'Google_test' is the subproject name
project(Google_tests)

include_directories(../include
                   ../src
)

# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests
add_executable(Google_Tests_All_Test
        invertedindextests.cpp
        ../src/invertedindex.cpp

        searchservertests.cpp
        ../src/searchserver.cpp)


target_link_libraries(Google_Tests_All_Test gtest gtest_main)





