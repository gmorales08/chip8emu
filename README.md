# chip8emu
This is an interpreter for the CHIP-8 programming language. It aims to simulate the operation of a physical machine while abstracting some low-level concepts.

> For a CHIP-8 overview check [this document](./doc/chip8.md)

The project is organized into the following concepts: 

![machineDiagram](doc/img/machineStructure.svg)

* **Machine**: Simulates a computer where the interpreter runs. It has a chip with the fontset, which is loaded into main memory during startup via the bus. The fontset used is the same as the one in the COSMAC VIP.

* **CHIP-8/CPU**: The core of the interpreter. It contains the registers, timers and the stack.

* **Main memory**: 4KB of memory that stores the font and the program data.

* **Auxiliary memory**: Memory that stores the machine status flag, the status of the keys in two 8-bit registers, and the status of the display.

* **Bus**: The bus is accessed directly by the CPU, the fontset chip, the keyboard controller, the display controller, and the sound controller. It handles reading and writing data to memory.

* **Display**: The display controller checks the [machine status register](#machine-status-register) at 60 Hz. If the display flag is set, the display controller reads the display memory through the bus and output its bytes to the screen.

* **Keyboard**: When needed, the CPU requests a keyboard scan with the control signal. Then, the keyboard controller reads the status of the keys and updates the [IO registers](#io-registers) through the bus.

* **Sound**: The CPU has a sound timer that when is equal to 0, the sound controller must emit a fixed-frecuency tone. The sound controller checks the [machine status register](#machine-status-register) to know if it must emit sound. The sound timer is decremented at 60Hz, at the sound controller reads the [machine status register](#machine-status-register) at 60 Hz.

The **memory addresses** are organized as follows:
* `0x0000` to `0x004F` are free
* `0x0050` to `0x01FF` is where the fontset is loaded
* `0x0200` to `0x0FFF` is where the programs are loaded
* `0x1000` is the [machine status register](#machine-status-register)
* `0x1001` and `0x1002` are the [IO registers](#io-registers)
* `0x1003` to `0x1102` is where the CPU writes the display memory

The **machine status register** <a id="machine-status-register"></a> is a byte that holds information about the current status of the machine:

`
Machine status register (0x1000): XXXXXX10
`
* display flag (bit 1): is set if the display needs to be updated.
* sound flag (bit 0): is set if the speaker must emit sound.
* The other bits are unused.

The **IO registers** <a id="io-registers"></a> are two bytes that hold the status of the 16 keyboard keys. Each bit is set when its respective key is pressed.
The first register stores the status of the keys 0 to 7 and the second register stores the status of the keys 8 to F.

`IO register 1 (0x1001): 01234567`

`IO register 2 (0x1002): 89ABCDEF`

<br>

> **Note**: This is my own implementation and may differ from other CHIP-8 interpreters.

## How to build chip8emu

Requirements:
* Cmake 3.10+
* SDL2 

In the root of the project, run:
```bash
cmake -B ./build -S .
cmake --build ./build
sudo cmake --install build
```

## How to run games

```bash 
chip8emu gameFile
```
