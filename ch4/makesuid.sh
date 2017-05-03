#!/bin/bash


sudo gcc suid.c  -o suid-exp
sudo chmod 4755 ./suid-exp # 这里设置了SUID位
ls -l
