# 2048 
A TUI 2048 game with vim-like controls implemented in c++ using ncurses library.

# Compiling & installation
Make sure you have the necessary dependencies:
- make
- g++
- ncurses

then in your terminal, navigate to source code directory, then run following commands:
```bash
$ make
$ sudo make install
```
after that you can delete whole source code directory if you don't need the source code.
To run the game:
```bash
$ 2048
```

# Controls
| Key | Action |
|-----|--------|
| k/w | slide blocks up |
| j/s | slide blocks down |
| h/a | slide blocks left |
| l/d | slide blocks right |
| r | refresh TUI |
| R | restart game |
| Q | exit |
