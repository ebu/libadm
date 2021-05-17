if(${BUILD_SHARED_LIBS})
	file(COPY ${ADMLIB} DESTINATION ${TESTEXEDIR})
endif()

execute_process(COMMAND ${TESTEXE} RESULT_VARIABLE CMD_RESULT)
if(CMD_RESULT)
	message(FATAL_ERROR "Error running ${CMD}")
endif()
