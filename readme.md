# Gadget Controller

### A Simple Tool To Control Usb Gadget

## Dependency

* Cmake
* libusbgx

## Compile

``` shell
git clone https://github.com/HandsomeMod/gc.git
cd gc
mkdir build
cd build
cmake ..
make
```

## Usage

``` shell
Usage : gc [options....] 
-h          Show this help. 
-l          Show active gadget functions. 
-c          Clean all active gadget.                                           -e          Enable all active gadget.
-d          Disable all active gadget.
-a <function> [configs ...]   Add a gadget function.                           
-r <name>      Delete a gadget function by name in list (-l).  
```

## Examples

``` shell
# Add a rndis function to gadget
gc -a rndis
# Show Gadget Status
gc -l
# Add a mass storage function to gadget
gc -a mass /home/user
```

## License

Gadget Controller is licensed under GPL-2.0.