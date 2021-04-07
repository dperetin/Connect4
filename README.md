# Connect4

Connect 4 is a simple game object of which is to i
drop some coins into a grid until you or your oponent 
create a pattern of 4 adjacent coins in a row, column or doagonal.

This is a simple student project made in C with a dash of OpenGl.

![alt text](https://github.com/dperetin/Connect4/blob/master/screenshot.png)

## How to build

### Ubuntu

Install dependencies

```
sudo add-apt-repository universe
sudo apt update

sudo apt install build-essentials
sude apt install freeglut3-dev
```

Build

```
make
```

### Mac

Build

```
make
```
  
## How to use

To launch a basic game where users starts first and default level of search depth (5):

```
./CONNECT_4
```

To launch a game with different setups:

```
./CONNECT_4 game_mode search_depth
```

Game modes:
1. `0` - Computer plays first.
2. `1` - Default - User plays first.
3. `2` - Two players mode.
4. `3` - Not sure what that one does 🤖

Search depth - integer defining how deep will the state graph be evaluated.

While in game, pressing these keys will:
- `j` - Switch to 2 player mode.
- `ESC` - Exit the game.





