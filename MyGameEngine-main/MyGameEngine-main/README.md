# MyGameEngine
Studying Hazel Engine coded by The Cherno and Physics simulation study by myself

****
## So Far I Got and TODO:
1, collision detection with circle and high performance simulation
![alt text](https://github.com/voidiii/MyGameEngine/blob/main/CodeStructNotebook/Circle_Collision_detection.png?raw=true)

2, GJK and EPA implemented but not stable

3, Soft body with spring and mass model/Pressure vloume model

****
## How to Set the VS Solution Up with Premake
Right now only support Windows, havent tried with Mac and Linux
Make sure you have Visual Studio newer than 2019, or you can change the version of VS in GenrateProjects.bat

pull or clone the project
```
git clone
```
pull the submodules
```
git pull --recurse-submodules
```

Then click GenrateProjects.bat to generate the solution
****
## Engine's Dev Log

Event system anslysis at 7/10
![alt text](https://github.com/voidiii/MyGameEngine/blob/main/CodeStructNotebook/Event_system_note.jpg?raw=true)

Render Abstraction at 7/10
![alt text](https://github.com/voidiii/MyGameEngine/blob/main/CodeStructNotebook/Render_Abstraction_note.jpg?raw=true)

7/23/2023
Starts my own creation, will keep going with Hazel video but mainly will foucs on Physics now.

Today's work: elastic collision with 10 balls
Todo: 
1, need to create better API for the scene, like how many balls/width and height etc
2, improve the collision detection function, now is O(n^2), which is stupid
3, Add debug tools and Frame rate 

7/24/2023
Done with basic API. Right now with 500 balls the fps is only around 60
A known bug: moving the window will cause timestep to grow into a unusual number which can lead to unknown behavior. 

7/25/2023
Fix the bug with timestep but it feels like the simulation now is depenedent on the frame rate. Great, another bug.
Roughly done with the grid optimazition, it is a Space Division optimazition. With 500 balls, the fps can now be over 120 now. However the frame rate can be influeneced by how tight balls are to each other. 
Next step I will see if I can further optimaze the grid, or I will see if I can implement Sort and Sweep or AABB

7/26/2026
Fixed a bug on the edge
Roughly done with mutithread optimazition. With 2000 balls, single thread can't hold to do anything. With 20 threads, it can be around 50fps. I think I should try another way to see the improvement of the optimazition. 
I studyed for SIMD today and realized that the best way to use SIMD is to use mutithread first. I barely have any vector calculation in the current scene and it just seems not worth it to implement SIMD. I will see if I can further improve the performance of this scene tomorrow. If not, I will proceed to the next scene: Spring or Pendulum

7/27/2023
I realized that, neither of Spring or Pendulum shoud be my piority right now so I decided to try to implement Minkowski sum and GJK for collision detection for complex 2D polygon

7/30/2023
Done with GJK for collision detection, now in theroy any convex will be able to detect collision. I just need to find a better to resolve the collision, which I think I will have to implement rotation first

8/2/2023
Added the rotation for square and realized the performance of the circle collision detection has a huge flaw so decided to add sub-steps to smooth things a bit. But it couldnt hold much more balls in the scene. On youtube there is this one guy who can do the collision scene for 3000 balls with 10 sub-steps. Damn. 

8/6/2023
Feels like I have done everything I can with this scene and I just cant figure out a way to further improve this. 

8/8/2023
Basic implementation of the balls collision scene
Optimaztion methods:
    1, Space division, use m_Grid in the scene to store info of postion of balls to reduce the number of collision check
    2, Use Thread pool to assgin tasks to different thread, go through m_Grid colume by colume for task-balancing
    3, Use std::vector for cache friendly, it is necessary because for every frame, grid's info needs to update with the new position of the balls and therefore need to go through balls one by one in order. In this case, cache firendly is highly in need.
    4, Substep for soomther perfermance 
    5, Use Verlet intergation, faster performance than classic Newton

Optimaztion Abstraction:
![alt text](https://github.com/voidiii/MyGameEngine/blob/main/CodeStructNotebook/Grid.jpg?raw=true)

8/15/2023
Trying to implement EPA, doesnt look like it can be stable, need more work. 