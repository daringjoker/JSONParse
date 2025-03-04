project(jsonParse)

set(CMAKE_CXX_STANDARD 17)
set(CC "zig c++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_executable("parse"
"src/main.cpp"
"src/JSON/Json.cpp"
"src/JSON/Json.h"
"src/JSON/JsonValue.h"
"src/JSON/JsonValue.cpp")
