if build_machine.system() == 'windows'
  glob = run_command('dir /s /b *.cpp', check: true)
else
  glob = run_command('find . -name "*.cpp"', check: true)
endif
sources = glob.stdout().strip().splitlines()

executable('orbc',
  sources,
  cpp_args: compile_options,
  cpp_pch: 'pch/pch.hpp',
  link_args: link_options,
  dependencies: [
    icu_uc,
    icu_io,
    icu_i18n,
  ],
  include_directories : include,
  install : true,
)
