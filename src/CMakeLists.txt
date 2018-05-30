

add_library(syrup-fs fs.c)
target_include_directories(syrup-fs 
    PUBLIC ${PROJECT_SOURCE_DIR}/include)