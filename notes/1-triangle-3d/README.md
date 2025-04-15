## See `notes/` for a more technical explanation of what this code does. In short:
We have taken our triangle-2d code and extended it to 3d, running it through Model, View, and Projection matrices, and displayed it relative to a camera. This required the creation of some helper files:

### `linalg.*`
This handles all the matrix and vector math, including basics like subtraction, dot and cross products, matrix and vector multiplication. It also includes handling getting the model, view, and projection matrices according to relevant parameters.

It also includes (and will include) the handling of edge functions and possibly barycentric coordinates.

### `model.*`
This handles anything specifically related to the shapes that we're modelling. At the moment, this means only a triangle, but it has code to determine whether a point is within a triangle, and holds the `Triangle` struct.

## Build and run
As with the last project, you can build and run this code with:
```sh
make; ./rasterizer
```

I separated out the building and linking processes in the makefile, so run `make clean` if you want to get rid of the object files. They are `.gitignore`d though.

# Camera Movement Branch
This branch is dedicated to getting camera movement working. This will include getting user input (I anticipate by using `SDL2`) and then moving the camera based on that input.

## `DeltaTime`
`DeltaTime` is a measure of the elapsed time between consecutive frames. It's used to ensure that object movement and other time-dependent computations remain consistent regardless of variations in frame rate. Use it like so:
```c
uint32_t lastTime = SDL_GetTicks(); // time in milliseconds before the main loop starts

while(true) {
    uint32_t currentTime = SDL_GetTickets();
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
    
    // ...

    camera.position.x += cameraSpeed * deltaTime;
}
```

## SDL Screen Coordinates
When using SDL screen coordinates, $(0, 0)$ is in the top left. Your NDC (Normalized Device Coordinates) to `SDL` coordinates conversion already handles this just fine, but in order to move the camera, you'll want to reverse what you'd intuitively think as increasing or decreasing `y`.

## Distortion when moving
Don't worry about the distortion when moving around, it's just a product of the fact that the screen is a 2d projection of a 3d triangle, so when you move the location of the camera, the perspective of the camera may cause it to seem like there is distortion. I believe this wouldn't be very brow-raising if there was some shading or something to indicate a difference in depth.

## Controls
| Key | Behavior |
|-----|----------|
| `w` | Move forward (increase `z`) |
| `s` | Move backward (decrease `z`) |
| `a` | Move left (decrease `x`) |
| `d` | Move right (increase `x`) |
| `e` | Move downward (increase `y`) |
| `q` | Move upward (decrease `y`) |

See how the controls for `e` and `q` are opposite what you'd think they'd be. This is because `SDL` has a coordinate system that starts wtih $(0, 0)$ in the top left.
