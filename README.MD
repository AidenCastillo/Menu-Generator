# Menu Generator

a C based menu generator for terminal applications and scripts. Only thing required is the compiled program and a menu-template.cfg file in the same directory.


# Compiling
You are required to have 'make' install on your linux box(`sudo apt install make
`).
1. Compile the program using `make`
   1. This will also compile any programs in the `src/exp/` folder as seperate executables.
2. Create a `menu-template.cfg` file in the same directory as the compiled program.
3. Run the `menu-generator` executable
   1. You can run `make run` and the MakeFile will automatically run the program after it has finished compiling.

# Example Programs
While it is not required to place programs you want the menu generator, if you want the programs to be automatically compiled, place the `.c` program in the `src/exp/` folder.
