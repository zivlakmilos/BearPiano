#!/bin/bash

make
avr-objcopy -j .text -j .data -O ihex bearpiano.elf bearpiano.hex
