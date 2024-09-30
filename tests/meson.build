src_dir = meson.source_root() / 'src'

add_test_setup('Basic',
                env: ['__TEST__=1'],
                is_default: true)


subdir('loading_files')

cursor_test = executable(
  'cursor_test',
  'data_structures/cursor.cpp',
  include_directories : include,
)

test('cursor_test', cursor_test)
