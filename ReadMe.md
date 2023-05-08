# ViennaLS Example

This repo contains a minimal example to show how to install ViennaLS on a bare system and then include it as dependency in an external project.

## Working in C++

You need a working installation of [ViennaLS](https://github.com/ViennaTools/ViennaLS#installing) to run this example. Then run the following steps to build the C++ project.

```
git clone https://github.com/ViennaTools/ViennaLS-Example.git
cd ViennaLS-Example
mkdir build && cd build
cmake .. -DViennaLS_DIR=/path/to/your/ViennaLS/install
make
```

After the example has been built, you can run the program by:

```
./Frosty
```

This will create a level set snowman and advect it to simulate it melting.

## Wokring in Pyhton

If you a Python version of ViennaLS installed on your system, you can simply call the Python script
```
python3 Frosty.py
```
to run the example.