cmake_minimum_required(VERSION 3.10)
project(advancedsw)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 소스 파일 목록
file(GLOB APP_SOURCES
    src/app/*.cpp
    src/app/domain/*.cpp
    src/app/application/*.cpp
    src/app/presentation/*.cpp
    src/app/exception/*.cpp
)

add_executable(app_program src/app/main.cpp ${APP_SOURCES})

# 필요시 테스트 타겟 추가
# file(GLOB TEST_SOURCES src/test/*.cpp)
# add_executable(test_program ${TEST_SOURCES}) 