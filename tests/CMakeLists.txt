add_executable(test1 test1.cpp)
add_executable(test2 test2.cpp)
add_executable(test3 test3.cpp)
add_executable(test4 test4.cpp)
add_executable(test5 test5.cpp)
add_executable(test6 test6.cpp)

target_link_libraries(test1 PRIVATE parambin)
target_link_libraries(test2 PRIVATE parambin)
target_link_libraries(test3 PRIVATE parambin)
target_link_libraries(test4 PRIVATE parambin)
target_link_libraries(test5 PRIVATE parambin)
target_link_libraries(test6 PRIVATE parambin)

if(WIN32)
	foreach(ARG test1 test2 test3 test4 test5 test6)
		add_custom_command(TARGET ${ARG} 
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:parambin> ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Copy parambin dll file to ${CMAKE_CURRENT_BINARY_DIR} directory" VERBATIM
		)

		add_custom_command(TARGET ${ARG}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:pwutils> ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Copy pwutils dll file to ${CMAKE_CURRENT_BINARY_DIR} directory" VERBATIM
		)
	endforeach()
endif()

add_custom_command(TARGET test1 POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
					${CMAKE_CURRENT_SOURCE_DIR}/input1.txt
					${CMAKE_CURRENT_BINARY_DIR}/input1.txt
)

add_custom_command(TARGET test1 POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
					${CMAKE_CURRENT_SOURCE_DIR}/input2.txt
					${CMAKE_CURRENT_BINARY_DIR}/input2.txt
)

add_custom_command(TARGET test1 POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
					${CMAKE_CURRENT_SOURCE_DIR}/argon.txt
					${CMAKE_CURRENT_BINARY_DIR}/argon.txt
)

add_custom_command(TARGET test2 POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
					${CMAKE_CURRENT_SOURCE_DIR}/input1.txt
					${CMAKE_CURRENT_BINARY_DIR}/input1.txt
)

add_custom_command(TARGET test2 POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
					${CMAKE_CURRENT_SOURCE_DIR}/input2.txt
					${CMAKE_CURRENT_BINARY_DIR}/input2.txt
)


