#
# cppan
#

################################################################################

########################################
# bazel_parser
########################################

BISON_TARGET(grammar
    "${CMAKE_CURRENT_SOURCE_DIR}/grammar.yy"
    "${CMAKE_CURRENT_BINARY_DIR}/grammar.cpp"
)
FLEX_TARGET(lexer
    "${CMAKE_CURRENT_SOURCE_DIR}/lexer.ll"
    "${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp"
    COMPILE_FLAGS --header-file=${CMAKE_CURRENT_BINARY_DIR}/lexer.h
)
ADD_FLEX_BISON_DEPENDENCY(lexer grammar)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(parser_src
    ${BISON_grammar_OUTPUTS}
    ${FLEX_lexer_OUTPUTS}
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.h
)
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/lexer.h PROPERTIES GENERATED TRUE)

set(bazel_parser_src
    bazel.cpp
    bazel.h
    driver.cpp
    driver.h
	grammar.yy
	lexer.ll
)
add_library(bazel_parser ${parser_src} ${bazel_parser_src})

########################################
# bazel_parser_test
########################################

add_executable(bazel_parser_test test.cpp)
target_link_libraries(bazel_parser_test bazel_parser)

################################################################################