include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)

CPMAddPackage(
        NAME magic_enum
        GITHUB_REPOSITORY Neargye/magic_enum
        GIT_TAG master
)
