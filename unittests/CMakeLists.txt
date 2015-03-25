cmake_minimum_required (VERSION 2.6)
project (AdjacentElementIteratorTest)

find_package(Boost COMPONENTS unit_test_framework)

if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	add_executable(unittests main.cpp
							 random_access.cpp)
	target_link_libraries(unittests ${Boost_LIBRARIES})
	add_definitions(-DBOOST_TEST_DYN_LINK)
	set_property(TARGET unittests PROPERTY CXX_STANDARD 11)
else()
	message(FATAL_ERROR "Could not find boost test libraries")
endif()