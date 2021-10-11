1. How to use your program: 

o WS: move the camera forward/backward along its forward vector.
o AD: move the camera left/right along its side vector.
o QE: move the camera up/down along the global up vector (0,1,0).
o Click + Drag: the mouse should tilt the camera around its yaw and pitch.
o Up/Down: rotate the camera around the pitch.
o Left/right: rotate the camera around the yaw.

2. Important parts of the code: 
    RenderManager.h/cpp
    GBuffer.h/cpp
    GeometryStage shaders

3. Known issues and problems: when pressing f5 error from openg due to shader handles