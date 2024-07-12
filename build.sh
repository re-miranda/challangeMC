#!/bin/sh
apk add gcc libc-dev
gcc -Wall -Wextra -Werror -o unitTest.out unitTest.c libcsv.c
