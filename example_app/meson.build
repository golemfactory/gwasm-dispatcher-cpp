project(
  'gwasm_dispatcher_test_app', 'cpp',
  default_options: [
    'cpp_std=c++17',
    'b_ndebug=if-release',
  ],
  version: '0.1.0',
)

add_project_arguments('-Wall', '-Wextra', language: 'cpp')

executable(
  'gwasm_dispatcher_test_app',
  '../test/my_functions.cpp',
  '../test/utils.cpp',
  'main.cpp',
  dependencies: [
    dependency('gwasm_dispatcher'),
  ],
  install: true,
)
