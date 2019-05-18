#!/bin/sh
export MallocStackLogging=1
export SHELL_LEAKS=1
make LEAKS=FULL
./21sh 2> tmp_stderr.log
cat tmp_stderr.log | less
