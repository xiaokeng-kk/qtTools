add_library(dataGenLib
    dataGen.cpp
    dataGen.h
)

target_include_directories(dataGenLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(dataGenLib
    PRIVATE
        Qt6::Core
)