# Order matters because of how add_dependencies() works
add_subdirectory(controls)
add_subdirectory(base)

add_subdirectory(animation)
add_subdirectory(dialog)
add_subdirectory(model)
add_subdirectory(particle)
add_subdirectory(sound)

add_subdirectory(world)
