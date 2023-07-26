# MyGameEngine
Studying Hazel Engine coded by The Cherno

学习笔记：（从第三期开始，前两期没啥记的）
https://zhuanlan.zhihu.com/p/640303210?


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
I studyed for SIMD today and realized that the best way to use SIMD is to use mutithread first. I barely have any vector calculation in the current scene and it just seems not worth it to implement SIMD. I will see if I can further improve the performance of this scene tomorrow. If not, I will proceed to the next scene: Spring