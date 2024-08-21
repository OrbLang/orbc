
test = executable('loading_file',
           'test.cpp',
           sources,
           dependencies: [icu_uc, icu_io, icu_i18n],
           include_directories : [include])

testfiles = files('normaltest.txt', 'emojitest.txt', 'multilingualtest.txt')



test('loading_files', test, args: [testfiles[0].full_path(), 'This is the contents of the orb file !!!'])
test('loading_files', test, args: [testfiles[1].full_path(), 'Hello i am a smiley face: "😊", time for emoji spam?? 💀🎺😭😍❤️😘🤩😶🍕🍳🥨🍞🔏'])
test('loading_files', test, args: [testfiles[2].full_path(), '我想知道這個測試是否會通過? ¡Ah, sí, algunos personajes raros españoles! ¿papá? Tid til dansk! ÆØÅ æøå ëÿüïöä êûîôâ ñ ËÜÏÖÄ ÊÛÎÔÂ おまえはもう、死んでいる!何? སུས་ཀྱང་ཨིསི་པེ་ནིཤ་ཞིབ་བཤེར་གྱི་རེ་བ་མེད།'])

