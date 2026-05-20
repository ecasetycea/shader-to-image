# shader-to-image
Render shader to a series of image files that can be turned into
video using ffmpeg. Currently only supports hardcoded shaders in
C using a single CPU thread and outputting to ppm. Plan to support more file
formats, automatically generating video, multithreading,
parsing GLSL/HLSL shaders and GPU rendering.

**Code assumes a linux environment** at the moment. It is highly probable it
**will not compile** in other environments.

This is a passion project that will let me play with the math behind fragment
shaders in a very raw way in a custom environment.

In the final implementation I want the user to be able to:
1. Choose a shader from a precompiled list
2. Load existing shader file
3. Load shadertoy shaders natively
4. Write shaders in an included GUI editor
5. Preview shaders in the GUI


For ffmpeg use this bash command:
`$ ffmpeg -framerate 30 -i name-%04d.ppm -c:v libx264 -pix_fmt yuv420p output.mp4`

## TODO list
- [x] working prototype (simple ppm file output)
    - [x] create a valid ppm sample file at a fixed resolution
    - [x] create a simple pattern
    - [x] put file in a folder
    - [x] loop while moving the pattern to create a simple animation
    - [x] update logic so i don't print as many new lines as frames
    - [x] print status bar
- [x] working sample shader
    - [x] setup vec structs
    - [x] rewrite shader in a separate file
    - [x] refactor logic in main into functions
    - [x] provide all shadertoy variables (time, dt, frame)
- [ ] handle custom options (use custom parser to avoid porting issues later on)
    - [ ] custom output file names
    - [ ] custom duration
    - [ ] custom speed
    - [ ] custom framerate
- [ ] setup build toolchain
- [ ] add a second shader and handle runtime choice (note to self: function pointers)
- [ ] add different colour palletes and handle runtime choice
- [ ] have a solid pool of shaders and palletes
- [ ] port executable from linux to windows
- [ ] automatically create video
- [ ] add more supported image file types
- [ ] create a GUI for linux
- [ ] port GUI to windows
- [ ] port everything to mac
- [ ] parse existing shaders
- [ ] create an integrated text editor to write shaders inside the GUI (requires parser? compile from gui and use as library?)
- [ ] preview shader
