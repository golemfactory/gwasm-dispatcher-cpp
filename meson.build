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
  'src/detail/json.cpp',
  'src/detail/step_args.cpp',
  'src/detail/task_def.cpp',
  'src/split.cpp',
  install: true,
)

install_headers(
  'include/gwasm.hpp',
)
install_headers(
  'include/gwasm/blob.hpp',
  'include/gwasm/dispatcher.hpp',
  'include/gwasm/error.hpp',
  'include/gwasm/split.hpp',
  subdir: 'gwasm',
)
install_headers(
  'include/gwasm/detail/execute.hpp',
  'include/gwasm/detail/json.hpp',
  'include/gwasm/detail/merge.hpp',
  'include/gwasm/detail/split.hpp',
  'include/gwasm/detail/step_args.hpp',
  'include/gwasm/detail/task_def.hpp',
  'include/gwasm/detail/utils.hpp',
  subdir: 'gwasm/detail',
)

pkg = import('pkgconfig')
pkg.generate(
  libgwasm_dispatcher,
  description: 'Implements gwasm runner interface',
)


if host_machine.system() != 'emscripten'
  the_test = executable(
    'the_test',
    'test/args.cpp',
    'test/detail/step_args.cpp',
    'test/dispatcher.cpp',
    'test/execute.cpp',
    'test/main.cpp',
    'test/merge.cpp',
    'test/my_functions.cpp',
    'test/split.cpp',
    'test/temp_dir_fixture.cpp',
    'test/utils.cpp',
    dependencies: [
      dependency('boost', modules: ['unit_test_framework']),
    ],
    link_with: libgwasm_dispatcher,
  )

  test('the_test', the_test)
endif
