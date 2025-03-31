add_library(TableGenerator
        Utils/Utils.h
        ../Common/Table.h
        Data/Grammar.h
        Parser/GrammarParser.h
        Parser/GrammarParser.cpp
        Processor/GrammarProcessor.h
        Processor/GrammarProcessor.cpp
        Utils/TableExporter.h
        Generator/TableGenerator.h
        Generator/TableGenerator.cpp
)

target_include_directories(TableGenerator PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Data
        ${CMAKE_CURRENT_SOURCE_DIR}/Generator
        ${CMAKE_CURRENT_SOURCE_DIR}/Parser
        ${CMAKE_CURRENT_SOURCE_DIR}/Processor
        ${CMAKE_CURRENT_SOURCE_DIR}/Utils
        ${PROJECT_ROOT}/modules/Common
)
