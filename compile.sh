#!/bin/bash
gcc main.c ./jsmn/jsmn.c ./can_lib/can_custom_lib.c -I/usr/include/libbson-1.0 -I/usr/include/libmongoc-1.0 -lmongoc-1.0 -lbson-1.0 -lm -lmosquitto -lc -o pub