cmake_minimum_required(VERSION 3.10)
project(Queen_Problem VERSION 0.1.0)

find_package(Boost REQUIRED)

add_executable(queen_problem
               Queen_Problem.cpp
               Color.cpp
               Color.hpp
               Board.hpp)

target_compile_features(queen_problem PRIVATE
                        cxx_std_17)

target_link_libraries(queen_problem
                      PRIVATE Boost::boost)
message("boost lib: " ${Boost_LIBRARIES})
