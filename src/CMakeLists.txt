find_package(SDL2 REQUIRED)

link_directories(~/usr/local/lib)

add_executable(mario
  main.cpp
  input_game.cpp
  input_world.cpp
  input_entity.cpp
  step_game.cpp
  step_world.cpp
  step_entity.cpp
  load_game.cpp
  load_tileset.cpp
  load_level.cpp
  run_game.cpp
  run_world.cpp
  run_entity.cpp
  texture_manager.cpp
  sprite_renderer.cpp
  world_state.cpp
  process_game.cpp
  process_world.cpp)

set_target_properties(mario PROPERTIES COMPILE_FLAGS "-Wall -std=c++14 -DEASTL_DLL -DEASTL_MOVE_SEMANTICS_ENABLED")

target_include_directories(mario PRIVATE ${SDL2_INCLUDE_DIRS} /usr/local/include ~/usr/local/include)
target_link_libraries(mario SDL2 SDL2main GL EASTL tegl GLEW IL ILU)
