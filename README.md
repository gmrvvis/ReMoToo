# ReMoToo - Remote Movie Tool
(c) 2015-2016. CCS / UPM
(c) 2015-2016. GMRV / URJC

www.ccs.upm.es
www.gmrv.es
gmrv@gmrv.es

## Introduction

ReMoToo is a system service that is able to stream the desktop as well as 
specific visual software to remote clients.

## Dependencies

* ReMo and its dependencies (see ReMo dependencies).

## Building

This software has been succesfully built and used on Ubuntu 18.04

The following steps should be enough to build it:

```bash
git clone https://gitlab.gmrv.es/nsviz/remotoo.git remotoo
mkdir remotoo/build && cd remotoo/build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```
