# Tron-Like Game

>This college project is a very simplified implementation of the famous game in the tron movie where players makes team and ride a motorcycle that leaves a trail behind them. The first one to collide with a trail or a wall make the other one win a round. The first to 3 win the game!

## Features

- **Console mode (ncurses)** or **Graphics mode (SDL)** :  
  The user **have to** specify the interface when compiling.
- **Turbo mode** :  
  When the user press t, the player will go much faster for a short moment. Youy can only go turbo mode 3 times in a round.

## Compilation & Execution

### Prerequisites

- **ncurses** (console mode)  
  ```bash
  sudo apt-get install libncurses-dev
- **SDL2** and **SDL2_ttf** (graphics mode)
  ```bash
  sudo apt-get install libsdl2-dev libsdl2-ttf-dev

### Choose the interface
The user have to chose a interface when compiling.

- **ncurses** (console mode)  
  ```bash
  make UI=ncurses
- **SDL** (graphics mode)
  ```bash
  make UI=sdl

### Launch the game
After compilition, a bin folder will be created. Inside, you will find a game exec.
So you can just from the directory 
```bash
bin/game
