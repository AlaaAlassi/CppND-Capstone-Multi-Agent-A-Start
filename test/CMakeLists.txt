# Add testing executable
enable_testing()

add_executable(
    collisionTest
    collisionTest.cpp
)
target_link_libraries(
    collisionTest
    gtest
)