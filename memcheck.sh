#! /bin/bash

#`valgrind --tool=memcheck --leak-check=full -v ./ray 10 10 <spotlight.txt \
#            >k-check=full --show-leak-kinds=all
#            spotlight.ppm`--track-origins=yes

`valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./ray 25 25 <tileplane.txt \
            >tileplane.ppm`
