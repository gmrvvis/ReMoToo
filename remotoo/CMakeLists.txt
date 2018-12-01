
include_directories( ${WEBSTREAMER_INCLUDE_DIRS} )
include_directories( ${PROJECT_SOURCE_DIR}
                     ${PROJECT_SOURCE_DIR}/ReMo/ReMo
                     ${PROJECT_SOURCE_DIR}/ReMo/ReMo/flow
                     ${PROJECT_SOURCE_DIR}/ReMo/ReMo/media
                     ${PROJECT_SOURCE_DIR}/ReMo/ReMo/stream
                     ${PROJECT_SOURCE_DIR}/ReMo/ReMo/pipeline
                     ${PROJECT_SOURCE_DIR}/ReMo/ReMo/util
                     ${PROJECT_SOURCE_DIR}/ReMo/ReMo/util/IO
                     ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}/src
                     ${PROJECT_BINARY_DIR} 
)

file(GLOB  DESKTOP_STREAM_SOURCES src/*.cpp)
file(GLOB  DESKTOP_STREAM_HEADERS src/*.hpp)

add_executable(ReMoToo
  ${DESKTOP_STREAM_SOURCES}
  ${DESKTOP_STREAM_HEADERS}
)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# --- dependencies --
# webstreamer
target_include_directories(ReMoToo PUBLIC ${PROJECT_SOURCE_DIR}/ReMo/ReMo ${WEBSTREAMER_INCLUDE_DIRS} webstreamer)
target_link_libraries(ReMoToo   ReMo
                                ${WEBSTREAMER_LIBRARIES}
                                )

set_target_properties( ReMoToo
                       PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
