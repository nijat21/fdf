# Fil de fer


https://github.com/user-attachments/assets/52f3eac1-96c0-46ed-9d51-2faa10d0913f


Compact 3D wireframe map renderer in C. Reads a map (`.fdf`) and draws a 3D wireframe using an isometric projection.

Main features

- Reads plain-text `.fdf` height maps and stores vertex heights.
- Renders a wireframe mesh using an isometric projection for a 3D look.
- Draws straight lines between points using Bresenham's line algorithm for efficient rasterization.
- Uses MiniLibX for windowing and drawing and a bundled `Libft` implementation.

Build

The project provides a Makefile that builds `Libft` and the platform MiniLibX variant for you.

```bash
make
```

This produces the `fdf` binary in the repository root.

Run

```bash
./fdf <map.fdf>
```

Example maps are available in `test_maps/` (for example `test_maps/42.fdf`). The program expects exactly one argument; running without one exits.

Controls

- Press `ESC` to close the window and exit.
- Closing the window with the window manager also exits the program.

Notes on platform

- The Makefile detects macOS vs Linux and builds the matching `minilibx` (`minilibx_macos_metal` or `minilibx-linux`).
- On Linux you may need X11 development packages installed. On macOS the Makefile runs `install_name_tool` to set the `libmlx.dylib` id.

Where to look in the code

- `fdf.c` — program entry, window/image initialization, and main loop.
- `reader.c`, `parser.c` — map input and parsing.
- `projection.c` — isometric projection math.
- `drawer.c` — rasterization and drawing (Bresenham's algorithm usage).
- `hooks.c` — input handlers (ESC and window close).

That's it — build with `make`, then run `./fdf <map.fdf>` to view a map in an isometric wireframe rendered with Bresenham's lines.
