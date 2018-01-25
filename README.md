PIXMAKER
========

A pixel-art focused editor.

Goals
=====

- Ease to use;
- Multiple tools;
- Multi-layer drawing;
- Animation (spritesheet) support;
- Extensible:
    - May add new input/output formats;
    - May add custom tools;
    - May add custom effects;
- **Free Software** (See LICENSE.md).


Build steps
===========

Ensure you have the following dependencies:

- CMake (Version 3.3 or later);
- SDL2 (2.0.4 or later);
- cairo (1.14 or later).

On ubuntu (or most debian based), you can just execute the following as root:

    apt-get install cmake libsdl2-dev libsdl2-ttf-dev libcairo2-dev 


Then choose an apropriated directory (we recoment `build/`, but it's your call) and hit:

    cmake {YOUR directory}
    cmake --build 