cmake_minimum_required(VERSION 3.5)

project(RayTracing1Wknd LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(RayTracing1Wknd main.cpp vec3.h color.h ray.h hittable.h sphere.h hittable_list.h common.h)
