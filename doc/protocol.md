# stdiofs protocol

Stdiofs uses a binary protocol which is communicated between _stdiofs provider_ and the _stdiofs_ process.
The protocol uses a strict request-reply scheme, where all requests are send from _stdiofs_ process and are responded by _stdiofs provider_.

## Endianess

All numeric data types are transferred in [Big Endian](https://en.wikipedia.org/wiki/Endianness).  
For instance, the numeric value 1 will be tranferred as

    00 00 00 01

## Data Types

### Basic Data Types

| Data Type | Width  | Desciption                                          |
|-----------|--------|-----------------------------------------------------|
| u8        |  8 Bit | Represents a byte.                                  |
| int       | 32 Bit | Signed integer. Mainly used as return type.         |
| u32       | 32 Bit | Unigned 32-bit integer.                             |
| size      | 32 Bit | Unsigned integer. Used to specify buffer sizes.     |
| mode      | 32 Bit | Represents access rights of file system entries.    |
| uid       | 32 Bit | Represents a user ID.                               |
| gid       | 32 Bit | Represents a group ID.                              |
| u64       | 64 Bit | Unsigned 64-bit integer. Used to file handled.      |
| offset    | 64 Bit | Used to specify offsets within files or file sizes. |
| dev       | 64 Bit | Represents a device ID.                             |

### Complex Data Types

#### String

Strings are mainly used to represent file system paths.

| Member | Data type | Description                                       |
|--------|-----------|---------------------------------------------------|
| size   | size      | Size of the string data in bytes, including '\0'. |
| data   | u8[]      | Zero-terminated string data.                      |

__Note__: Strings are encoded as zero-terminated strings and the 
trailing '\0' is included in the size.

Therefore, an empty string is encoded as follows:

    [ size    ] '\0'
    00 00 00 01  00

#### Bytes

Bytes are used to transfer file contents.

| Member | Data Type | Description          |
|--------|-----------|----------------------|
| size   | size      | Count of data bytes. |
| data   | u8[]      | Data bytes.          |

#### Stringlist

Used to represent directory contents and supported operations.

| Member | Data Type | Description               |
|--------|-----------|---------------------------|
| count  | size      | Count of strings in list. |
| items  | string[]  | List items.               |

#### Stat

Represents file statistics.

| Member     | Data Type | Description                               |
|------------|-----------|-------------------------------------------|
| inode      | u64       | inode number                              |
| mode       | mode      | access mode                               |
| nlink      | u64       | number of hard links                      |
| uid        | uid       | user ID of owner                          |
| gid        | gid       | group ID of owner                         |
| rdev       | dev       | device ID (is special file)               |
| size       | offset    | total size in bytes                       |
| blocks     | u64       | number of 512 byte blocks allocated       |
| atime_sec  | u32       | time of last access (seconds)             |
| atime_nsec | u32       | time of last access (nano seconds)        |
| mtime_sec  | u32       | time of last modification (seconds)       |
| mtime_nsec | u32       | time of last modification (nano seconds)  |
| ctime_sec  | u32       | time of last status change (seconds)      |
| ctime_nsec | u32       | time of last status change (nano seconds) |

__Note:__ The following fields are not included by intention conforming to 
[libfuse documentation](https://github.com/libfuse/libfuse/blob/master/include/fuse.h) of _getattr_:

- st_dev 
- st_blksize

#### StatVFS

Represents files system statistics.


| Member     | Data Type | Description                               |
|------------|-----------|-------------------------------------------|
| bsize      | u64       | file system block size                    |
| frsize     | u64       | fragement size                            |
| blocks     | u64       | size of file system in frsize units       |
| bfree      | u64       | free blocks                               |
| bavail     | u64       | free blocks for unprivileged users        |
| files      | u64       | count of inodes                           |
| ffree      | u64       | free inodes                               |
| namemax    | u64       | maximum filename length                   |

__Note:__ The following fields are not included by intention conforming to 
[libfuse documentation](https://github.com/libfuse/libfuse/blob/master/include/fuse.h) of _stdfs_:

- f_favail
- f_fsid
- f_flags

## Message format

Each request and each response is considerred a message.

| Member  | Data Type | Description                                         |
|---------|-----------|-----------------------------------------------------|
| size    | size      | total size of message, including size and id fields |
| id      | u32       | id of method to call or to reply, see below         |
| payload | u8[]      | payload data, size - 8 bytes                        |

### Method IDs

| ID | Method         |
|----|----------------|
|  1 | listoperations |
|  2 | getattr        |
|  3 | access         |
|  4 | readlink       |
|  5 | readdir        |
|  6 | mknod          |
|  7 | mkdir          |
|  8 | symlink        |
|  9 | unlink         |
| 10 | rmdir          |
| 11 | rename         |
| 12 | link           |
| 13 | chmod          |
| 14 | chown          |
| 15 | truncate       |
| 16 | create         |
| 17 | open           |
| 18 | read           |
| 19 | write          |
| 20 | statfs         |
| 21 | release        |
| 22 | fsync          |
| 23 | lseek          |

__Note:__ Both, requests and replies use always the same method ID.

## Constants

| Constant       | Data Type | Value | Description                  |
|----------------|-----------|-------|------------------------------|
| INVALID_HANDLE | u64       | -1    | Invalid or unset file handle |

## Methods

Note that all fields of each reply are always transmitted, even if the
operation failed.

A successful operation returns an operation result of 0. Some operations, 
e.g. _read_ and _write_ have postive results. A negative result is considerred
an error. Typically -errno is returned.

Detailed information of each operation can be found in 
[libfuse documentation](https://github.com/libfuse/libfuse/blob/master/include/fuse.h)
or in the according man page of the particular operation.

### listoperations

Query supported operations of the file system.

This will be the vary first request. A provider can safely assume that only
supported operations will be requested.

| Field      | Data Type  | Direction | Description          |
|------------|------------|-----------|----------------------|
| result     | int        | reply     | Operation result     |
| operations | stringlist | reply     | supported operations |


### getattr

Query file statistics.

| Field       | Data Type | Direction | Description                 |
|-------------|-----------|-----------|-----------------------------|
| path        | string    | request   | path of file system element |
| file_handle | u64       | request   | file handle                 |
| result      | int       | reply     | operation result            |
| statistics  | stat      | reply    | statistics of the file      |

__Note__: _file handle_ handle might be INVALID_HANDLE.

### access

Check access to file system element.

| Field  | Data Type | Direction | Description                 |
|--------|-----------|-----------|-----------------------------|
| path   | string    | request   | path of file system element |
| mask   | int       | request   | access rights to check      |
| result | int       | reply     | operation result            |

### readlink

Resolve a link.

| Field       | Data Type | Direction | Description             |
|-------------|-----------|-----------|-------------------------|
| path        | string    | request   | path of link to resolve |
| buffer_size | size      | request   | size of result buffer   |
| result      | int       | reply     | operation result        |
| buffer      | string    | reply     | resolved link           |


### readdir

Read contents of a directory.

| Field       | Data Type  | Direction | Description         |
|-------------|------------|-----------|---------------------|
| path        | string     | request   | path of directory   |
| offset      | offset     | request   | offset in directory |
| file_handle | u64        | request   | handle of directory |
| result      | int        | reply     | operation result    |
| entries     | stringlist | reply     | directory contents  |

__Note__: _file handle_ handle might be INVALID_HANDLE.

### mknod

Create a file system node.

| Field  | Data Type | Direction | Description                    |
|--------|-----------|-----------|--------------------------------|
| path   | string    | request   | path of node to create         |
| mode   | mode      | request   | type of node to create         |
| rdev   | dev       | request   | device in case of special file |
| result | int       | reply     | operation result

### mkdir

Create a directory.

| Field  | Data Type | Direction | Description                 |
|--------|-----------|-----------|-----------------------------|
| path   | string    | request   | path of directory to create |
| mode   | mode      | request   | access rights of directory  |
| result | int       | reply     | operation result            |

### symlink

Create a symbolic link.

| Field  | Data Type | Direction | Description      |
|--------|-----------|-----------|------------------|
| from   | string    | request   | source of link   |
| target | string    | request   | target of link   |
| result | int       | reply     | operation result |

### unlink

Remove a file system element.

| Field  | Data Type | Direction | Description               |
|--------|-----------|-----------|---------------------------|
| path   | string    | request   | path of element to remove |
| result | int       | reply     | operation result          |

### rmdir

Remove a directory.

| Field  | Data Type | Direction | Description                 |
|--------|-----------|-----------|-----------------------------|
| path   | string    | request   | path of directory to remove |
| result | int       | reply     | operation result            |

### rename

Rename a file system element.

| Field  | Data Type | Direction | Description                         |
|--------|-----------|-----------|-------------------------------------|
| from   | string    | request   | old path of the file system element |
| to     | string    | request   | new path of the file system element |
| flags  | int       | request   | flags                               |
| result | int       | reply     | operation result

### link

Create a hard link.

| Field  | Data Type | Direction | Description         |
|--------|-----------|-----------|---------------------|
| from   | string    | request   | source of hard link |
| to     | string    | request   | target of hard link |
| result | int       | reply     | operation result    |

### chmod

Change file mode.

| Field  | Data Type | Direction | Description                 |
|--------|-----------|-----------|-----------------------------|
| path   | string    | request   | path of file system element |
| mode   | mode      | request   | new file mode               |
| result | int       | reply     | operation result            |

### chown

Change ownership of file.

| Field  | Data Type | Direction | Description                 |
|--------|-----------|-----------|-----------------------------|
| path   | string    | request   | path of file system element |
| uid    | uid       | request   | ID of new user              |
| gid    | gid       | request   | ID of new group             |
| result | int       | reply     | operation result            |

### truncate

Set size of a file.

| Field       | Data Type | Direction | Description      |
|-------------|-----------|-----------|------------------|
| path        | string    | request   | path of file     |
| size        | offset    | request   | new size of file |
| file_handle | u64       | request   | handle of file   |
| result      | int       | reply     | operation result |

__Note__: _file handle_ handle might be INVALID_HANDLE.

### create

Create and opens a file.

| Field       | Data Type | Direction | Description            |
|-------------|-----------|-----------|------------------------|
| path        | string    | request   | path of file to create |
| mode        | mode      | request   | file mode              |
| flags       | int       | request   | creation flags         |
| result      | int       | reply     | operation result       |
| file_handle | u64       | reply     | handle of created file |

### open

Opens a file.

| Field       | Data Type | Direction | Description          |
|-------------|-----------|-----------|----------------------|
| path        | string    | request   | path of file to open |
| flags       | int       | request   | creation flags       |
| result      | int       | reply     | operation result     |
| file_handle | u64       | reply     | handle of file       |

### read

Reads data from file.

| Field       | Data Type | Direction | Description            |
|-------------|-----------|-----------|------------------------|
| path        | string    | request   | path of file to read   |
| buffer_size | size      | request   | amount of data to read |
| offset      | offset    | request   | offset in file to read |
| file_handle | u64       | request   | handle of file to read |
| result      | int       | reply     | operation result       |
| buffer      | bytes     | reply     | data read              |

__Note__: _file handle_ handle might be INVALID_HANDLE.

### write

Write data to file.

| Field       | Data Type | Direction | Description             |
|-------------|-----------|-----------|-------------------------|
| path        | string    | request   | path of file to write   |
| buffer      | bytes     | request   | data to write           |
| offset      | offset    | request   | offset to write         |
| file_handle | u64       | request   | handle of file to write |
| result      | int       | reply     | operation result        |

__Note__: _file handle_ handle might be INVALID_HANDLE.

### statfs

Query file system statistics.

| Field      | Data Type | Direction | Description            |
|------------|-----------|-----------|------------------------|
| path       | string    | request   | path in file system    |
| result     | int       | reply     | operation result       |
| statistics | statvfs   | reply     | file system statistics |

### release

Close a file.

| Field       | Data Type | Direction | Description      |
|-------------|-----------|-----------|------------------|
| path        | string    | request   | path to file     |
| file_handle | u64       | request   | handle to close  |
| result      | int       | reply     | operation result |

### fsync

Synchronize a file.

| Field       | Data Type | Direction | Description                                 |
|-------------|-----------|-----------|---------------------------------------------|
| path        | string    | request   | path of file to synchronize                 |
| isdatasync  | int       | request   | specify whether meta data should be flushed |
| file_handle | u64       | request   | handle of file to synchronze                |
| result      | int       | reply     | operation result                            |

__Note__: _file handle_ handle might be INVALID_HANDLE.

### lseek

Find next data or hole in file.

| Field       | Data Type | Direction | Description                 |
|-------------|-----------|-----------|-----------------------------|
| path        | string    | request   | path of file to search      |
| offset      | offset    | request   | offset to start search      |
| whence      | int       | request   | origin of offset            |
| file_handle | u64       | request   | handle of file to search    |
| result      | offset    | reply     | offset of next data or hole |

__Note__: _file handle_ handle might be INVALID_HANDLE.
