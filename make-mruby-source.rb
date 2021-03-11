require 'fileutils'

SRC_ROOT = File.join('.', 'mruby')
DST_ROOT = File.join('source', 'mruby')
SRC_BUILD = File.join(SRC_ROOT, 'build', 'microbit')

# source/mruby
FileUtils.mkdir_p DST_ROOT

# copy src/
FileUtils.cp_r File.join(SRC_ROOT, 'src'), DST_ROOT

# copy include/
FileUtils.cp_r File.join(SRC_ROOT, 'include'), DST_ROOT
# presym/id.h, presym/table.h
FileUtils.cp_r File.join(SRC_BUILD, 'include'), DST_ROOT

# copy mrblib/mrblib.c
mrblib_dir = File.join(DST_ROOT, 'mrblib')
FileUtils.mkdir_p mrblib_dir
FileUtils.cp File.join(SRC_BUILD, 'mrblib', 'mrblib.c'), mrblib_dir

# rename string.h to _string.h
FileUtils.mv File.join(DST_ROOT, 'include', 'mruby', 'string.h'), File.join(DST_ROOT, 'include', 'mruby', '_string.h')

bak = case RUBY_PLATFORM.downcase
when /darwin/ # macOS
  "''"
else  # Linux and other
  ''
end

# modify #inlucde string.h
`find source/mruby -type f | grep '\.[ch]$'`.each_line {|f|
  if f =~ /\.[ch]$/
    `sed -i #{bak} 's!mruby/string.h!mruby/_string.h!g' #{f}`
  end
}

# patch mrbconf.h
MRBCONF_DEFS = [
  "/* Defines for Micro:bit */\n",
  "#define MRB_32BIT\n",
  "#define MRB_USE_FLOAT\n",
  "#define MRB_WORD_BOXING\n",
  "#define POOL_PAGE_SIZE 1024\n",
  "#define MRB_CONSTRAINED_BASELINE_PROFILE\n",
  "\n"
]
MRBCONF_TARGET_LINE = 12  # target line for insert definitions

mrbconf = File.join(DST_ROOT, 'include', 'mrbconf.h')
mrbconf_bak = mrbconf + '.bak'
FileUtils.mv mrbconf, mrbconf_bak
File.open(mrbconf_bak, 'r') {|fb|
  conf = fb.readlines
  conf.insert(MRBCONF_TARGET_LINE, MRBCONF_DEFS)
  File.open(mrbconf, 'w') {|fn|
    fn.write conf.flatten.join('')
  }
}

# patch boxing_word.h
boxing_word = File.join(DST_ROOT, 'include', 'mruby', 'boxing_word.h')
boxing_word_bak = boxing_word + '.bak'
FileUtils.cp boxing_word, boxing_word_bak
`sed -i #{bak} 's/^\s\s\(\!mrb_immediate_p\(o\)\s*\&\&\s/\s\s(/g' #{boxing_word}`
