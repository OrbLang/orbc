test_exe = executable('loading_file_test',
  'test.cpp',
  files(
    src_dir / 'fileio/file_loading.cpp',
    src_dir / 'log/log.cpp',
  ),
  dependencies: [
    icu_uc,
    icu_io,
    icu_i18n
  ],
  include_directories : include,
)

testFiles = files('normaltest.txt', 'emojitest.txt', 'multilingualtest.txt')
testInput = [
  'This is the contents of the orb file !!!',
  'Hello i am a smiley face: "😊", time for emoji spam?? 💀🎺😭😍❤️😘🤩😶🍕🍳🥨🍞🔏',
  '我想知道這個測試是否會通過? ¡Ah, sí, algunos personajes raros españoles! ¿papá? Tid til dansk! ÆØÅ æøå ëÿüïöä êûîôâ ñ ËÜÏÖÄ ÊÛÎÔÂ おまえはもう、死んでいる!何? སུས་ཀྱང་ཨིསི་པེ་ནིཤ་ཞིབ་བཤེར་གྱི་རེ་བ་མེད།',
]

testArgs = []
foreach idx : range(testFiles.length())
  testArgs += [[testFiles[idx], testInput[idx]]]
endforeach

foreach testArg : testArgs
  test('loading_files', test_exe, args: testArg)
endforeach
