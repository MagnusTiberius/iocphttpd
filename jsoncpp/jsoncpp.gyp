{
  'variables': {
  },

  'target_defaults': {
    'defines': [
      '_GNU_SOURCE',
    ],
  },

  'targets': [
  {
    'target_name': 'jsoncpp',
    'type': '<(library)',
    'include_dirs': [
      'include',
    ],
    'direct_dependent_settings': {
      'include_dirs': [ 'include' ],
    },

    'sources': [
      'src/lib_json/json_reader.cpp',
      'src/lib_json/json_writer.cpp',
      'src/lib_json/json_value.cpp',
    ],

    'conditions':
    [
      [ 'OS!="win"',
      {
        'xcode_settings': 
        {
          'OTHER_CFLAGS': 
          [
            '-std=c++0x',
            '-stdlib=libc++',
          ],
        },
        'cflags':
        [
          '-std=c++0x',
        ],
      },
      ],
      [ 'OS=="freebsd"',
      {
#        'cflags+': [ '-stdlib=libc++' ],
      }],
    ],
  },
  ],
}
