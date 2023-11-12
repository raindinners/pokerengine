cmake_minimum_required(VERSION 3.12)

project(pokerengine VERSION 0.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(POKERENGINE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/pokerengine)
set(POKERENGINE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

include_directories(${PROJECT_NAME} ${POKERENGINE_INCLUDE_DIR})

set(SOURCE_FILES ${POKERENGINE_SOURCE_DIR}/pokerengine.cpp)
set(HEADER_FILES
        ${POKERENGINE_INCLUDE_DIR}/bits.hpp
        ${POKERENGINE_INCLUDE_DIR}/card/card.hpp
        ${POKERENGINE_INCLUDE_DIR}/card/cards.hpp
        ${POKERENGINE_INCLUDE_DIR}/card/hand.hpp
        ${POKERENGINE_INCLUDE_DIR}/constants.hpp
        ${POKERENGINE_INCLUDE_DIR}/engine/action.hpp
        ${POKERENGINE_INCLUDE_DIR}/engine/engine.hpp
        ${POKERENGINE_INCLUDE_DIR}/engine/player.hpp
        ${POKERENGINE_INCLUDE_DIR}/engine/positions.hpp
        ${POKERENGINE_INCLUDE_DIR}/engine/pot.hpp
        ${POKERENGINE_INCLUDE_DIR}/engine/round.hpp
        ${POKERENGINE_INCLUDE_DIR}/enums.hpp
        ${POKERENGINE_INCLUDE_DIR}/evaluator/evaluation.hpp
        ${POKERENGINE_INCLUDE_DIR}/evaluator/evaluation_result.hpp
        ${POKERENGINE_INCLUDE_DIR}/evaluator/lookup_tables.hpp
        ${POKERENGINE_INCLUDE_DIR}/evaluator/result.hpp
        ${POKERENGINE_INCLUDE_DIR}/pokerengine.hpp
        ${POKERENGINE_INCLUDE_DIR}/pycard/pycard.hpp
        ${POKERENGINE_INCLUDE_DIR}/pycard/pycard__init__.hpp
        ${POKERENGINE_INCLUDE_DIR}/pycard/pycards.hpp
        ${POKERENGINE_INCLUDE_DIR}/pycard/pyhand.hpp
        ${POKERENGINE_INCLUDE_DIR}/pyconstants.hpp
        ${POKERENGINE_INCLUDE_DIR}/pyengine/pyaction.hpp
        ${POKERENGINE_INCLUDE_DIR}/pyengine/pyengine.hpp
        ${POKERENGINE_INCLUDE_DIR}/pyengine/pyengine__init__.hpp
        ${POKERENGINE_INCLUDE_DIR}/pyengine/pyplayer.hpp
        ${POKERENGINE_INCLUDE_DIR}/pyengine/pypositions.hpp
        ${POKERENGINE_INCLUDE_DIR}/pyengine/pypot.hpp
        ${POKERENGINE_INCLUDE_DIR}/pyengine/pyround.hpp
        ${POKERENGINE_INCLUDE_DIR}/pyenums.hpp
        ${POKERENGINE_INCLUDE_DIR}/pyevaluation/pyevaluation__init__.hpp
        ${POKERENGINE_INCLUDE_DIR}/pyevaluation/pyevaluation_result.hpp
        ${POKERENGINE_INCLUDE_DIR}/pyevaluation/pyresult.hpp
        ${POKERENGINE_INCLUDE_DIR}/python.hpp
        ${POKERENGINE_INCLUDE_DIR}/pyutils.hpp
        ${POKERENGINE_INCLUDE_DIR}/string.hpp
        ${POKERENGINE_INCLUDE_DIR}/types.hpp
        ${POKERENGINE_INCLUDE_DIR}/utils.hpp
        ${POKERENGINE_INCLUDE_DIR}/vector.hpp)

find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
find_package(pybind11 CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)

python_add_library(pokerengine_core MODULE ${SOURCE_FILES} ${HEADER_FILES} WITH_SOABI)
target_link_libraries(pokerengine_core PRIVATE pybind11::headers magic_enum::magic_enum)
target_compile_definitions(pokerengine_core PRIVATE VERSION_INFO=${PROJECT_VERSION})

install(TARGETS pokerengine_core DESTINATION pokerengine)
