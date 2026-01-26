**Affirm**
  - polarstingray

A C program that randomly selects a line from a chosen file and displays it. 
This program caches recently queried results and, upon hits, re rolls for a new line.


**Usage**
run:
  *make
  make install
  affirm A*

**Adding More and Making Changes
To add more content, either directly edit the $HOME/.local/share/affirm/*.txt files directly or add your own
with its own unique id <id>.txt

or 

Do the same to the files ./data/*.txt in this current directory.
and run:
  *make install
  affirm <id>.txt*


To make changes to the code, simply alter the ./src/main.c
and run:
  *make clean
  make install*


