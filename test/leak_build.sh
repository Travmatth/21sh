#!/bin/sh
make LEAKS=FULL
./21sh 2> tmp_stderr.log
cat tmp_stderr.log | less
