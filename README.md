# stdiofs

Inject a filesystem via stdio.

It's like [sshfs](https://github.com/libfuse/sshfs), but reverse (and eventually without ssh).

It's like [NFS](https://en.wikipedia.org/wiki/Network_File_System), but without a server.

## Motivation

May embedded device, such as smart home and [IoT](https://en.wikipedia.org/wiki/Internet_of_things) are very limited regarding to their (non-volatile) memory resources. Such devices are typically based on embedded linux system.

Some use cases, like updating the firmware, creating backups or restoring them require to handle (larger) files:

- during a firmware update, the firmware image is typically stored somewhere on device
- unless backups can be streamed, (larger) files are typically buffered somewhere on device
- unless restore can be streamed, (larger) files are typically buffered somewhere on device

For such use caes, it would be nice to _inject_ a remote file system to provide or store these files.

During development phase, there are also use cases that may require to provide some data to the device, e.g. providing debug information or additional tools.  
One approach is [NFS](https://en.wikipedia.org/wiki/Network_File_System), which comes really in handy during development. Unfortunately, NFS requires a server where the device needs to connect to. So the NFS approach is often not suitable in productive environments.

Many embedded linux based devices contain an [SSH](https://en.wikipedia.org/wiki/Secure_Shell_Protocol) for maintenance purposes. Therefore, it is preferrable to use this existing and secure protocol to inject a file system. [Sshfs](https://github.com/libfuse/sshfs) allows to mount a virtual files system using [fuse](https://github.com/libfuse/libfuse) on the host PC and delegates all file system requests to the target. This is pretty much the reverse direction for the most use cases decribed above.


## How does it work?

![stdiofs](doc/stdiofs.svg "stdiofs")

The idea of _stdiofs_ is to reverse the direction of [sshfs](https://github.com/libfuse/sshfs) in order to inject a file system on a host PC to the target device. Therefore, _stdiofs_ consists of two binaries:

- _stdiofs_provider_: a tool running on the host PC, which replies to file system requests
- _stdiofs_: a [fuse](https://github.com/libfuse/libfuse)-based tool on the target device, which delegates all filesystem requests to the calling process

_Stdiofs_ does not establish any SSH connection by itself. Instead _stdiofs_provider_ forks a process an commuicate via _stdin_ and _stdout_. The forked process can be _ssh_ which connects to to target device and executes _stdiofs_ there.

Typically, stdiofs will be used like this:

    stdiofs_provider -p /some/path -- sshpass -p secret ssh user@target stdiofs -f -s /path/to/mountpoint

Technically, stdiofs can use every tunnel that provides communication via _stdin_ and _stdout_. For testing purposes, it can even be used without any tunnel.

    stdiofs_provider -p /some/path -- stdiofs -f -s /path/to/mountpoint

## Build

    mkdir build
    cd build
    cmake ..
    make
    make install

### Build Options

| Option           | Type | Description                      |
|------------------|------|----------------------------------|
| WITHOUT_TESTS    | bool | Disable build of unit tests      |
| WITHOUT_STDIOFS  | bool | Disable build of stdiofs binary  |
| WITHOUT_PROVIDER | bool | Disable build of provider binary |


## Protocol 

Stdiofs uses a [binary protocol](doc/protocol.md) which is communicated between _stdiofs provider_ and the _stdiofs_ process.

## Quirks

Since stdiofs uses _stdin_ and _stdout_ for communication, it does not make much sence to run it in daemon mode.
Therefore, option _-f_ should be provided in most use cases.

Since stdiofs uses a strict request-reply scheme, it does not make much sence to run it multithreaded.
A mutex is used to enforce the communication scheme.
Therefore, running stdiofs in multithreded mode does not lead to any errors, but wastes resources.
Option _-s_ should be provided to run in single-threaded mode.
