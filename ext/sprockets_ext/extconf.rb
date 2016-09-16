require 'mkmf'

$CFLAGS = '-std=c99 -Wall -g'

dir_config('sprockets_ext')
create_makefile('sprockets_ext')
