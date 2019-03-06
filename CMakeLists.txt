cmake_minimum_required(VERSION 3.10)
project(Queen_Problem VERSION 0.1.0)

add_executable(queen_problem
               Queen_Problem.cpp
               Color.cpp
               Color.hpp
               Board.hpp)

target_compile_features(queen_problem PRIVATE
                        cxx_std_17)
