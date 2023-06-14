# Minesweeper
A small Minesweeper game written in Qt. The style/ astethics is inspired by the original Windows Entertainment Pack version.

<sub>*(For large fields/ tile counts the game is a bit laggy when restarting or uncovering large chunks of the field. I might one day fix this.)*</sub>

## How to play
***Left-click*** on a tile to uncover it. The number on the tile says how many of the eight neighbouring tiles contain a ***mine***. If you ***right-click*** on a tile it will be ***flagged*** which prevents you from accidentally pressing it. Uncover the entire field without clicking on a mine. If you do click on one, the game is lost. The mines are randomly distributed at the start of each game. To restart the current difficulty simply press the smiley at the top of the window or choose a new difficulty via the menu and restart the game.

Convinience feature for fast solving: if an uncovered tile has the same amount of flags surrounding it as its displayed number, you can click on the uncovered tile to immediatley uncover all non-flagged covered tile surrounding it.

## Difficulties
- ***Beginner***: 8x8 field with 10 mines
- ***Intermediate***: 16x16 field with 40 mines
- ***Expert***: 30x16 field with 99 mines.
- ***Custom***: up to 40x40 with 700 mines.

## Highscores & stats
The game automatically keeps track of the 10 best times in each of the 3 default difficulties. The scores are saved in `saves\highscores.txt` with the date and time (UTC+1).
