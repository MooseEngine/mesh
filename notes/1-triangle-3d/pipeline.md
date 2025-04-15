## Basic Pipeline for 3-d rendering

1. **Starting in Model Space:**  
   You begin with a vertex expressed in model space—for example, $(x, y, z, 1)$—which represents a point relative to the object’s own coordinate system (such as a corner of a cube).

2. **Model Matrix (M):**  
   The model matrix transforms vertices from model space into world space. It applies transformations like translation, rotation, and scaling to position and orient the object correctly in your scene.

3. **View Matrix (V):**  
   Once the vertex is in world space, the view matrix converts it into camera (or eye) space. This matrix is defined by your camera’s parameters (its position, the point or direction it’s looking at, and an up vector).  
   - **Key Point:** The view matrix effectively “moves” the world so that the camera is at the origin with a standardized orientation (typically looking down the negative $z$-axis). This transformation is conceptually the inverse of the camera's own transformation.

4. **Projection Matrix (P):**  
   The projection matrix takes vertices from camera space and transforms them into clip space. Using parameters like field of view, aspect ratio, and near/far clipping planes, it defines a view frustum (a pyramidal shape that represents the volume visible to the camera) and maps this frustum into a canonical cuboid (clip volume).  
   - **What It Does:** Although the projection matrix sets up the non-linear perspective effect (so that farther objects are “prepared” to appear smaller), it produces coordinates in homogeneous (4D) form. In this stage, the $w$ component is usually set based on the depth ($z$ value), which encodes how much scaling due to distance will be applied.

5. **Perspective Division:**  
   The final step is perspective division. Here, you divide the $x$, $y$, and $z$ components of your clip-space vertex by its $w$ component. This step is essential because:
   - It converts the homogeneous coordinates into normalized device coordinates (NDC), which typicall$y$ range from –1 to 1.
   - The division applies the perspective foreshortening: vertices that are farther away (and thus have larger $|w|$ values) get divided more, making them appear smaller on the screen.
   - Without dividing by $w$, the nonlinear scaling required to mimic how we perceive depth would not be correctly applied.

6. **Mapping to Screen Space:**  
   Once you have the vertex in normalized device coordinates (after perspective division), these coordinates are mapped to screen (or viewport) coordinates. This final step converts the NDC values to actual pixel positions on your display.

---

**In summary:**  
- **Model Matrix (M):** Moves and orients vertices from an object’s local space into world space.  
- **View Matrix (V):** Repositions and rotates the world so the camera appears at the origin, looking in a standard direction.  
- **Projection Matrix (P):** Transforms camera space into clip space using a perspective (or orthographic) transformation that encodes depth in the w component.  
- **Perspective Division:** Divides by the $w$ component to convert homogeneous coordinates to normalized device coordinates, finalizing the perspective effect that makes distant objects appear smaller.  
- **Viewport Mapping:** Converts NDC to actual screen coordinates.
