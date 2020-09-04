# Atari Jaguar examples by Seb/The Removers

This repository contains some examples of use of the rmvlib.

Do not forget to define the following environment variables:

export JAGPATH=/path/to/Jaguar-tree

It is assumed that Jaguar-tree has a 'bin' subdirectory which contains
- the Atari tools (MadMac [or rmac], ALN [or jlinker], rdbjag, wdb) 
- the BJL loader (lo)
- the Skunk loader (jcp)
- the Removers Jaguar image converter

If not, please modify Makefile.config according to your configuration.

You may also need ocaml to build some examples from source code.

Common target rules for Makefile (defined in Makefile.example)

- reset: reset SkunkBoard
- skunk: build COF file and send it to SkunkBoard
- interactive: build COF file, send it to SkunkBoard and start an interactive session with jcp (useful for jcp supporting Removers extensions)
- flash: build ABS file (located in cartridge area) and flash it to SkunkBoard
- bjl: build BIN file and send it to BJL
