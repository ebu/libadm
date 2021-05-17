if(DEFINED ADM_SHARED_LIB) # Do shared lib copy
	message(STATUS "Shared lib copy: \n${ADM_SHARED_LIB} \n${TEST_EXE_DIR}")
	execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${ADM_SHARED_LIB} ${TEST_EXE_DIR} RESULT_VARIABLE CMD_RESULT)
	if(CMD_RESULT)
		message(FATAL_ERROR "Error performing shared lib copy: \n${CMD_RESULT} \n${RESULT_VARIABLE}")
	endif()
endif()

execute_process(COMMAND ${TEST_EXE} RESULT_VARIABLE CMD_RESULT)
if(CMD_RESULT)
	message(FATAL_ERROR "Error running ${TEST_EXE}: \n${CMD_RESULT} \n${RESULT_VARIABLE}")
endif()
