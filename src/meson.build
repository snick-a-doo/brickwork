brickwork_sources = ['brickwork.cc', 'draw.cc', 'wall.cc', 'main.cc']
brickwork_app = executable('brickwork',
                           brickwork_sources,
                           include_directories: brickwork_include)

test_sources = ['brickwork.cc', 'wall.cc', 'test.cc']
test_app = executable('test_app',
                      test_sources,
                      include_directories: brickwork_include)

test('brick test', test_app)
