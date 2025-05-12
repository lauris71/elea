# elea
A 3D mathematics library in C

## Features

Basic 3D prmitive classes

- Vector2f
- Vector3f
- Vector4f
- Quaternionf
- Color4f
- Planef
- Rayf
- Matrx3x3f
- Matrix3x4f
- Matrx4x4f

Plus some collision and geometry generation functions

It integrates with [az](https://github.com/lauris71/az) library for run-time type system features (construction and destruction, properties and much more).


## Building

It needs arikkei and az libraries for type system. Just clone [arikkei](https://github.com/lauris71/arikkei) and [az](https://github.com/lauris71/az) to an accessible place (either to the root of the main project or elea).
Then, in the elea project directory execute:

    cmake -S . -B build
    cmake --build build

The provided cmake configuration builds static library.
