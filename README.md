**Affirm**
Author: polarstingray

A C program that randomly selects a line from a chosen file and displays it.  
This program caches recently queried results and, upon hits, re rolls for a new line.  


**Usage**  
run:  
    _cd /path/to/affirm  
    ./setup.sh  
    affirm Q  
    affirm A_  

**Adding More and Making Changes**   
To add more content, either directly edit the $HOME/.local/share/affirm/*.txt files directly or add your own with its own unique id <id>.txt
  
or   
  
Do the same to the files ./data/*.txt in this current directory.  
and run:  
_make install  
affirm <id>.txt_  
  
  
To make changes to the code, alter the ./src/main.c  
and run:   
_make clean  
make install_  


