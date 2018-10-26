cmake_minimum_required(VERSION 3.6.2)
project (moneymaker,c++)


#  set(CMAKE_RC_COMPILER_INIT windres)
  ENABLE_LANGUAGE(RC)
#  SET(CMAKE_RC_COMPILE_OBJECT
#    "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")


#add_executable(moneymaker moneymakerapp.cpp moneymaker.cpp moneymaker.res)
add_executable(moneymaker moneymakerapp.cpp moneymaker.cpp moneymaker.rc)
target_link_libraries(moneymaker user32 gdi32)

add_executable(myjson myjson.cpp)
