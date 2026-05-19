# shader-to-image
CPU renderer that takes a fragment shader from a hardcoded list and renders it
to a series of image files to be used as video frames in ffmpeg.

This is a passion project that will let me play with the math behind fragment
shaders in a very raw way in a custom environment.

Initial implementation will output only to ppm files due to the simplicity of
the format. My final image for this project is a user friendly program that can
even be released as an executable, where the user can choose a shader from a
list, some parameters like speed, length, resolution, and then have a video at
the end. A GUI front end is also intended.

More advanced users will be able to manually implement their own shaders.

For ffmpeg use this bash command:
`$ ffmpeg -framerate 30 -i name-%04d.ppm -c:v libx264 -pix_fmt yuv420p output.mp4`

## TODO list
- [ ] working prototype (simple ppm file output)
    - [x] create a valid ppm sample file at a fixed resolution
    - [x] create a simple pattern
    - [x] put file in a folder
    - [x] loop while moving the pattern to create a simple animation
    - [x] update logic so i don't print as many new lines as frames
    - [ ] print status bar
- [ ] working sample shader
    - [x] setup vec structs
    - [ ] rewrite shader in a separate file
    - [ ] refactor logic in main into functions
- [ ] handle custom options (use custom parser to avoid porting issues later on)
    - [ ] custom output file names
    - [ ] custom duration
    - [ ] custom speed / framerate
- [ ] use build scripts / make / cmake toolchain
- [ ] add a second shader and handle runtime choice (note to self: function pointers)
- [ ] add different colour palletes and handle runtime choice
- [ ] have a solid pool of shaders and palletes
- [ ] port executable from linux to windows
- [ ] add scripts for automatically creating the video (bash and ps)
- [ ] add more supported image file types
- [ ] create a GUI for linux
- [ ] port GUI to windows
- [ ] port everything to mac
- [ ] create an integrated text editor to write shaders inside the GUI (requires parser? compile from gui and use as library?)
