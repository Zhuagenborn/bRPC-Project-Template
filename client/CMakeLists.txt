set(MODULE_NAME client)

add_executable(${MODULE_NAME})

target_sources(${MODULE_NAME}
    PRIVATE
        ${MODULE_NAME}.cpp
)

target_link_libraries(${MODULE_NAME}
    PRIVATE
        echo_proto
)