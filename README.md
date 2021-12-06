Too run 
```
Clone repo into folder
Open folder in Visal Studio
Generate CMake cache
Select Pacman.exe from startup items
Build and Run Pacman.exe
```


[Known bugs](https://git.gvk.idi.ntnu.no/course/prog2002/autumn_2021/assignment_1/Group_23/-/wikis/Known-bugs) for CMake and building the project.

Pacman is moved by the arrow keys, and moves in this direction until a new arrow key has been pressed to change the direction. 

Instead of doing linear interpolation through the points of the map, Pacman moves freely inside the open spaces of the tunnel. If Pacman collides with the tunnel his speed will be slowed down to ensure that he can move as close to the edge of the wall as possible. (So if you go into the edge of another tunnel, and you cant instantly traverse through the next tunnel, you should stay in the directon towards the tunnel to get closer to the wall)

The game has a point system where you get 1 point per pellet collected. At the end of the game, the score is printed in the terminal.

To close window, press the X in the corner or the ESC-key

**Ending Conditions for the game:**

1. You get caught by the ghost
2. You collect all the pellets

**FEATURES**

