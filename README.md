# DoOR

DoOR is a tool that help working with ramdisks under *nix systems.

### Usage
door [command]
- door install
- door init
- door start
- door sync
- door stop

### Version
1.1.0

### Installation

##### You can download here:
https://www.dropbox.com/sh/jt3jevspxyujsd1/AAAeLwRl4p5sIyF6P0kWwQMBa?dl=0

##### If you want to compile it from sources:

You need cmake and g++ installed on your computer:

```sh
$ git clone https://github.com/zbalint/DoOR.git
$ cd DoOR
$ cmake .
$ make
$ ./door install
$ sudo ln -s ~/.door/bin/door /usr/bin/door
```

License
----

GPLv3
