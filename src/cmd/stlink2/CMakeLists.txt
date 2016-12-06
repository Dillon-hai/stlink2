set (STLINK2_CMD "stlink2")

set (STLINK2_CMD_SOURCE
	"main.c"
)

add_executable(${STLINK2_CMD} ${STLINK2_CMD_SOURCE})
add_dependencies(${STLINK2_CMD} ${STLINK2_LIBRARY_STATIC})
target_link_libraries(${STLINK2_CMD} ${STLINK2_LIBRARY_STATIC})

install (TARGETS
	${STLINK2_CMD}
	DESTINATION bin
)
