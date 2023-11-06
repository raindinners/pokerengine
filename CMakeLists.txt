cmake_minimum_required(VERSION 3.12)

project(pokerengine VERSION 0.3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CPP_DIR cpp)
set(CPP_INCLUDE_DIR cpp/include)
set(CPP_SOURCE_DIR cpp/src)

set(PYTHON_DIR python)

include_directories(${PROJECT_NAME} ${CPP_INCLUDE_DIR}/pokerengine)
include_directories(${PROJECT_NAME} ${PYTHON_DIR})

set(SOURCE_FILES ${CPP_SOURCE_DIR}/bits.cpp)
set(HEADER_FILES
        ${CPP_INCLUDE_DIR}/pokerengine/bits.hpp
        ${CPP_INCLUDE_DIR}/pokerengine/card/card.hpp
        ${CPP_INCLUDE_DIR}/pokerengine/card/cards.hpp
        ${CPP_INCLUDE_DIR}/pokerengine/card/hand.hpp
        ${CPP_INCLUDE_DIR}/pokerengine/evaluator/evaluation.hpp
        ${CPP_INCLUDE_DIR}/pokerengine/evaluator/evaluation_result.hpp
        ${CPP_INCLUDE_DIR}/pokerengine/evaluator/lookup_tables.hpp
        ${CPP_INCLUDE_DIR}/pokerengine/evaluator/result.hpp
        ${CPP_INCLUDE_DIR}/pokerengine/engine/engine.hpp
        ${CPP_INCLUDE_DIR}/pokerengine/engine/player.hpp
        ${CPP_INCLUDE_DIR}/pokerengine/engine/positions.hpp
        ${CPP_INCLUDE_DIR}/pokerengine/engine/pot.hpp
        ${CPP_INCLUDE_DIR}/pokerengine/engine/round.hpp
        ${CPP_INCLUDE_DIR}/pokerengine/pokerengine.hpp
        ${CPP_INCLUDE_DIR}/pokerengine/types.hpp
        ${CPP_INCLUDE_DIR}/pokerengine/utils.hpp
        ${CPP_INCLUDE_DIR}/pokerengine/vector.hpp)

set(PYTHON_HEADERS
        ${PYTHON_DIR}/pycard/pycard.hpp
        ${PYTHON_DIR}/pycard/pycard__init__.hpp
        ${PYTHON_DIR}/pycard/pycards.hpp
        ${PYTHON_DIR}/pycard/pyhand.hpp
        ${PYTHON_DIR}/pyengine/pyengine.hpp
        ${PYTHON_DIR}/pyengine/pyengine__init__.hpp
        ${PYTHON_DIR}/pyengine/pyplayer.hpp
        ${PYTHON_DIR}/pyengine/pypositions.hpp
        ${PYTHON_DIR}/pyengine/pypot.hpp
        ${PYTHON_DIR}/pyengine/pyround.hpp
        ${PYTHON_DIR}/pyenums/pyenums.hpp
        ${PYTHON_DIR}/pyenums/pyenums__init__.hpp
        ${PYTHON_DIR}/pyevaluation/pyevaluation__init__.hpp
        ${PYTHON_DIR}/pyevaluation/pyevaluation_result.hpp
        ${PYTHON_DIR}/pyevaluation/pyresult.hpp
        ${PYTHON_DIR}/python.hpp
        ${PYTHON_DIR}/pyutils.hpp)
set(PYTHON_SOURCE
        ${PYTHON_DIR}/pokerengine.cpp)
set(PYTHON_FILES ${PYTHON_HEADERS} ${PYTHON_SOURCE})

find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
find_package(pybind11 CONFIG REQUIRED)

python_add_library(pokerengine_core MODULE ${SOURCE_FILES} ${HEADER_FILES} ${PYTHON_FILES} WITH_SOABI)
target_link_libraries(pokerengine_core PRIVATE pybind11::headers)
target_compile_definitions(pokerengine_core PRIVATE VERSION_INFO=${PROJECT_VERSION})

install(TARGETS pokerengine_core DESTINATION pokerengine)
