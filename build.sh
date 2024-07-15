#!/bin/sh
apk add gcc libc-dev
gcc -Wall -Wextra -Werror -shared -o libcsv.so libcsv.c helper.c
