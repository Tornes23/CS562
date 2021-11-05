1. How to use your program: 

o LShift: moves the camera faster.
o LControl: moves the camera slower.
o WS: move the camera forward/backward along its forward vector.
o AD: move the camera left/right along its side vector.
o QE: move the camera up/down along the global up vector (0,1,0).
o Click + Drag: the mouse should tilt the camera around its yaw and pitch.
o Up/Down: rotate the camera around the pitch.
o Left/right: rotate the camera around the yaw.

o Num1: Displays the resulting texture.
o Num2: Displays the Diffuse texture.
o Num3: Displays the Normal texture.
o Num4: Displays the Specular texture.
o Num5: Displays the Depth texture.

o LContol + Num1: Displays the Decal Volumes.
o LContol + Num2: Displays the Decal Projected Areas.
o LContol + Num3: Displays the Decal Result.

2. Important parts of the code: 
    RenderManager.h/cpp (DecalStage Function)
    DecalBuffer.h/cpp
    Decal shaders

3. Known issues and problems: 
    - I think i fixed the bloom problem 
      if possible let me know that is corrected