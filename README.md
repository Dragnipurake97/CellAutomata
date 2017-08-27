# Cell Automata
This is a simulation of prey and predator cells using OpenGL. [It is inspired by this video](https://www.youtube.com/watch?v=v7Z2euv7kwY)
and uses the same rules and colours, albeit with a different implemention as it uses oon OpenGL.  

# Rules
**Prey:**
1. Can move to any adjacent empty space.   
2. Increases health by 1 each iteration.
3. After reaching a health threshold reset health to 1 and genertate a new prey on an adjacent tile if there is room. 
   
**Predator:**
1. Can move to any adjacent empty space.  
2. Decreases health by 1 each iteration.
3. If prey is adjacent, convert it to prey and increase health by 1.   

# Current Status
![](https://github.com/LibLib97/CellAutomata/raw/master/GIF/CellAutomota_v1.gif)

## Possible Improvements
1. Add cell count.
2. Add better controls (left click to spawn prey, right for predators).
3. Optimisations.

