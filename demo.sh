#!/bin/bash
make
./runServoSpeed << EOF
5
0
180
45
EOF
./runServoSpeed << EOF
5
180
0
180
EOF
