file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers} "src/source_processor/design_extractor/CallsManager.h" "src/source_processor/design_extractor/CallsManager.cpp" "src/source_processor/design_extractor/CallsGraphProcNode.cpp" "src/source_processor/design_extractor/UsesModifiesTypeExtractor.h" "src/source_processor/design_extractor/UsesModifiesTypeExtractor.cpp" "src/source_processor/design_extractor/DesignExtractor.cpp" "src/source_processor/design_extractor/DesignExtractor.h" "src/source_processor/design_extractor/CallsGraphStmtNode.cpp")
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
