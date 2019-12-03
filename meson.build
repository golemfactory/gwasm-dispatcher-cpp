project(
  'gwasm_dispatcher', 'cpp',
  default_options: [
    'cpp_std=c++17',
    'b_ndebug=if-release',
  ],
  version: '0.1.0',
)

add_project_arguments('-Wall', '-Wextra', language: 'cpp')
add_project_arguments('-DBOOST_ALL_DYN_LINK', language: 'cpp')

libgwasm_dispatcher = library(
  'gwasm_dispatcher',
  'src/blob.cpp',
  'src/detail/args.cpp',
  'src/detail/json.cpp',
  'src/detail/task_def.cpp',
  'src/split.cpp',
)

the_test = executable(
  'the_test',
  'test/execute.cpp',
  'test/main.cpp',
  'test/merge.cpp',
  'test/split.cpp',
  'test/temp_dir_fixture.cpp',
  'test/utils.cpp',
  dependencies: [
    dependency('boost', modules: ['unit_test_framework']),
  ],
  link_with: libgwasm_dispatcher,
)

test('the_test', the_test)
