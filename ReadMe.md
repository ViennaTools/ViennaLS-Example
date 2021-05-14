# ViennaLS Example

This repo contains a minimal example to show how to install ViennaLS on a bare system and then include it as dependency in an external project.


```
mkdir ViennaTools && cd ViennaTools
export VIENNATOOLS_DIR=`pwd`

# Installing ViennaLS
cd $VIENNATOOLS_DIR
git clone https://github.com/ViennaTools/ViennaLS.git
cd ViennaLS
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install/
make # this will install all dependencies for ViennaLS and might take while
make install


# Building the example
cd $VIENNATOOLS_DIR
git clone https://github.com/ViennaTools/ViennaLS-Example.git
cd ViennaLS-Example
mkdir build && cd build
cmake .. -DViennaLS_DIR=../ViennaLS/install
make
```

After the example has been built, you can run the program by:

```
./Frosty
```

This will create a level set snowman and advect it to simulate it melting.
