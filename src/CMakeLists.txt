find_package(SDL2 REQUIRED)

link_directories(~/usr/local/lib)

add_executable(mario
  main.cpp
  game_values.cpp
  input_game.cpp
  input_world.cpp
  input_entity.cpp
  step_game.cpp
  step_world.cpp
  step_entity.cpp
  load_game.cpp
  load_tileset.cpp
  load_sound.cpp
  load_level.cpp
  load_level_object.cpp
  run_game.cpp
  run_world.cpp
  run_entity.cpp
  texture_manager.cpp
  sprite_renderer.cpp
  text_renderer.cpp
  world_state.cpp
  process_game.cpp
  process_world.cpp
  make_entity_game.cpp
  make_entity_world.cpp
  make_entity_entity.cpp
  destroy_entity_game.cpp
  destroy_entity_world.cpp
  destroy_entity_entity.cpp
  flush_sound_queue.cpp)

set_target_properties(mario PROPERTIES COMPILE_FLAGS "-Wall -std=c++14 -DEASTL_DLL -DEASTL_ASSERT_ENABLED -DEASTL_MOVE_SEMANTICS_ENABLED -DGLM_PRECISION_HIGHP_INT")

target_include_directories(mario PRIVATE ${SDL2_INCLUDE_DIRS} /usr/local/include ~/usr/local/include /usr/include/freetype2)
target_link_libraries(mario SDL2 SDL2main SDL2_mixer GL EASTL tegl GLEW IL ILU freetype)
