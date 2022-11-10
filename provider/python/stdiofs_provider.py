#!/usr/bin/env python3

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

import os
import subprocess
import argparse
import errno
import signal

shutdown_requested = False
def on_shutdown_requested(sig, frame):
        shutdown_requested = True

class Message:
    def __init__(self, id):
        self.buffer = []
        self.add_u32(0)
        self.add_u32(id)
    
    def add_u32(self, value):
        a = (value >> 24) & 0xff
        b = (value >> 16) & 0xff
        c = (value >>  8) & 0xff
        d = value         & 0xff
        self.buffer.extend([a, b, c, d])
    
    def add_i32(self, value):
        self.add_u32(value & 0xffffffff)

    def add_u64(self, value):
        upper = (value >> 32) & 0xffffffff
        lower =  value        & 0xffffffff
        self.add_u32(upper)
        self.add_u32(lower)

    def add_str(self, value):
        data = value.encode('utf-8')
        size = len(data) + 1
        self.add_u32(size)
        self.buffer.extend(data)
        self.buffer.append(0)

    def add_strings(self, values):
        count = len(values)
        self.add_u32(count)
        for value in values:
            self.add_str(value)

    def add_bytes(self, value):
        size = len(value)
        self.add_u32(size)
        self.buffer.extend(value)

    def get_bytes(self):
        size = len(self.buffer)
        self.buffer[0] = (size >> 24) & 0xff
        self.buffer[1] = (size >> 16) & 0xff
        self.buffer[2] = (size >>  8) & 0xff
        self.buffer[3] =  size        & 0xff
        return bytearray(self.buffer)

class MessageReader:
    def __init__(self, buffer):
        self.buffer = buffer
        self.offset = 0
    
    def read_u32(self):
        value = (self.buffer[self.offset] << 24) + (self.buffer[self.offset + 1] << 16) + (self.buffer[self.offset + 2] << 8) + self.buffer[self.offset + 3]
        self.offset += 4
        return value
    
    def read_i32(self):
        value = self.read_u32()
        if value >= 0x80000000:
            value -= 0x100000000
        return value
    
    def read_u64(self):
        high = self.read_u32()
        low  = self.read_u32()
        return (high << 32) + low

    def read_str(self):
        size = self.read_u32()
        value = self.buffer[self.offset : self.offset + size - 1]
        self.offset += size
        return value.decode()
    
    def read_bytes(self):
        size = self.read_u32()
        value = self.buffer[self.offset : self.offset + size]
        self.offset += size
        return value
    
class EmptyStatVfsBuffer:
    def __init__(self):
        self.f_bsize = 0
        self.f_frsize = 0
        self.f_blocks = 0
        self.f_bfree = 0
        self.f_bavail = 0
        self.f_files = 0
        self.f_ffree = 0
        self.f_namemax = 0

class EmptyStatResult:
    def __init__(self):
        self.st_ino = 0
        self.st_mode = 0
        self.st_nlink = 0
        self.st_uid = 0
        self.st_gid = 0
        self.st_dev = 0
        self.st_size = 0
        self.st_blocks = 0
        self.st_atime = 0
        self.st_atime_ns = 0
        self.st_mtime = 0
        self.st_mtime_ns = 0
        self.st_ctime = 0
        self.st_ctime_ns = 0        

class FilesystemProvider:
    def __init__(self, path):
        self.root = os.path.abspath(path)
        self.commands = {
            1: FilesystemProvider.list_operations,
            2: FilesystemProvider.getattr,
            3: FilesystemProvider.access,
            4: FilesystemProvider.readlink,
            5: FilesystemProvider.readdir,
            6: FilesystemProvider.mknod,
            7: FilesystemProvider.mkdir,
            8: FilesystemProvider.symlink,
            9: FilesystemProvider.unlink,
           10: FilesystemProvider.rmdir,
           11: FilesystemProvider.rename,
           12: FilesystemProvider.link,
           13: FilesystemProvider.chmod,
           14: FilesystemProvider.chown,
           15: FilesystemProvider.truncate,
           16: FilesystemProvider.create,
           17: FilesystemProvider.open,
           18: FilesystemProvider.read,
           19: FilesystemProvider.write,
           20: FilesystemProvider.statfs,
           21: FilesystemProvider.release,
           22: FilesystemProvider.fsync,
           23: FilesystemProvider.lseek
        }

    def get_path(self, reader):
        local_path = reader.read_str().lstrip(os.path.sep)
        return os.path.join(self.root, local_path)

    def read_command(self, p):
        try:
            header = MessageReader(p.stdout.read(8))
            size = header.read_u32()
            id = header.read_u32()
            remainder = p.stdout.read(size - 8) if size > 8 else b''
        except:
            id = -1
            remainder = b''
        return (id, remainder)

    def run(self, commandline):
        p = subprocess.Popen(commandline, bufsize=0, stdin=subprocess.PIPE, stdout=subprocess.PIPE, close_fds=True)
        while not shutdown_requested and p.poll() == None:
            (command, payload) = self.read_command(p)
            if command in self.commands:
                fun = self.commands[command]
                reader = MessageReader(payload)
                message = Message(command)
                fun(self, reader, message)
                p.stdin.write(message.get_bytes())
            elif command == -1:
                break
            else:
                print('unknown command')
                break
        p.terminate()

    def list_operations(self, reader, message):
        message.add_u32(0)
        message.add_strings([
            "getattr",
            "access",
            "readlink",
            "readdir",
            "mknod",
            "mkdir",
            "unlink",
            "rmdir",
            "symlink",
            "rename",
            "link",
            "chmod",
            "chown",
            "truncate",
            "create",
            "open",
            "read",
            "write",
            "statfs",
            "release",
            "fsync",
            "lseek"
        ])

    def getattr(self, reader, message):
        path = self.get_path(reader)
        result = 0
        try:
            data = os.lstat(path)
        except OSError as ex:
            result = -ex.errno
            data = EmptyStatResult()
        message.add_i32(result)
        message.add_u64(data.st_ino)
        message.add_u32(data.st_mode)
        message.add_u64(data.st_nlink)
        message.add_i32(data.st_uid)
        message.add_i32(data.st_gid)
        message.add_u64(data.st_dev)
        message.add_u64(data.st_size)
        message.add_u64(data.st_blocks)
        message.add_u32(int(data.st_atime))
        message.add_u32(data.st_atime_ns)
        message.add_u32(int(data.st_mtime))
        message.add_u32(data.st_mtime_ns)
        message.add_u32(int(data.st_ctime))
        message.add_u32(data.st_ctime_ns)



    def access(self, reader, message):
        path = self.get_path(reader)
        mode = reader.read_u32()
        result = -errno.EACCES
        try:
            if os.access(path, mode) == True:
                result = 0
        except OSError as ex:
            result = -ex.errno
        message.add_i32(result)

    def readlink(self, reader, message):
        path = self.get_path(reader)
        result = 0
        link = ""
        try:
            link = os.readlink(path)
        except OSError as ex:
            result = -ex.errno
        message.add_i32(result)
        message.add_str(link)

    def readdir(self, reader, message):
        path = self.get_path(reader)
        result = 0
        names = []
        try:
            with os.scandir(path) as it:
                for entry in it:
                    names.append(entry.name)
        except OSError as ex:
            result = -ex.errno
        message.add_i32(result)
        message.add_strings(names)

    def mknod(self, reader, message):
        path = self.get_path(reader)
        mode = reader.read_u32()
        rdev = reader.read_u64()
        result = 0
        try:
            os.mknod(path, mode, rdev)
        except OSError as ex:
            result = -ex.errno
        message.add_i32(result)

    def mkdir(self, reader, message):
        path = self.get_path(reader)
        mode = reader.read_u32()
        result = 0
        try:
            os.mkdir(path, mode)
        except OSError as ex:
            result = -ex.errno
        message.add_i32(result)

    def symlink(self, reader, message):
        source = reader.read_str()
        target = self.get_path(reader)
        result = 0
        try:
            os.symlink(source, target)
        except OSError as ex:
            result = -ex.errno
        message.add_i32(result)

    def unlink(self, reader, message):
        path = self.get_path(reader)
        result = 0
        try:
            os.unlink(path)
        except OSError as ex:
            result = -ex.errno
        message.add_i32(result)

    def rmdir(self, reader, message):
        path = self.get_path(reader)
        result = 0
        try:
            os.rmdir(path)
        except OSError as ex:
            result = -ex.errno
        message.add_i32(result)

    def rename(self, reader, message):
        source = self.get_path(reader)
        target = self.get_path(reader)
        result = 0
        try:
            os.rename(source, target)
        except OSError as ex:
            result = -ex.errno
        message.add_i32(result)

    def link(self, reader, message):
        source = self.get_path(reader)
        target = self.get_path(reader)
        result = 0
        try:
            os.link(source, target)
        except OSError as ex:
            result = -ex.errno
        message.add_i32(result)

    def chmod(self, reader, message):
        path = self.get_path(reader)
        mode = reader.read_u32()
        result = 0
        try:
            os.chmod(path, mode)
        except OSError as ex:
            result = -ex.errno
        message.add_i32(result)

    def chown(self, reader, message):
        path = self.get_path(reader)
        uid = reader.read_i32()
        gid = reader.read_i32()
        result = 0
        try:
            os.chown(path, uid, gid)
        except OSError as ex:
            result = -ex.errno
        message.add_i32(result)

    def truncate(self, reader, message):
        path = self.get_path(reader)
        size = reader.read_u64()
        fd = reader.read_u64()
        result = 0
        try:
            if fd != 0xffffffffffffffff:
                os.ftruncate(fd, size)
            else:
                os.truncate(path, size)
        except OSError as ex:
            result = -ex.errno
        message.add_i32(result)

    def create(self, reader, message):
        path = self.get_path(reader)
        mode = reader.read_u32()
        flags = reader.read_i32()
        result = 0
        try:
            fd = os.open(path, flags, mode)
        except OSError as ex:
            result = -ex.errno
        message.add_i32(result)
        message.add_u64(fd)

    def open(self, reader, message):
        path = self.get_path(reader)
        flags = reader.read_i32()
        result = 0
        try:
            fd = os.open(path, flags)
        except OSError as ex:
            result = -ex.errno
        message.add_i32(result)
        message.add_u64(fd)

    def read(self, reader, message):
        path = self.get_path(reader)
        size = reader.read_u32()
        offset = reader.read_u64()
        fd = reader.read_u64()
        result = 0
        buffer = []
        try:
            if fd != 0xffffffffffffffff:
                buffer = os.pread(fd, size, offset)
                result = len(buffer)
            else:
                with os.open(path, os.O_RDONLY) as f:
                    buffer = os.pread(f, size, offset)
        except OSError as ex:
            result = -ex.errno
        message.add_i32(result)
        message.add_bytes(buffer)

    def write(self, reader, message):
        path = self.get_path(reader)
        data = reader.read_bytes()
        offset = reader.read_u64()
        fd = reader.read_u64()
        result = 0
        try:
            if fd != 0xffffffffffffffff:
                result = os.pwrite(fd, data, offset)
            else:
                with os.open(path, os.O_WRONLY) as f:
                    os.pread(f, data, offset)
        except OSError as ex:
            result = -ex.errno
        message.add_i32(result)

    def statfs(self, reader, message):
        path = self.get_path(reader)
        result = 0
        buffer = EmptyStatVfsBuffer()
        try:
            buffer = os.statvfs(path)
        except OSError as ex:
            result = -ex.errno
        message.add_i32(result)
        message.add_u64(buffer.f_bsize)
        message.add_u64(buffer.f_frsize)
        message.add_u64(buffer.f_blocks)
        message.add_u64(buffer.f_bfree)
        message.add_u64(buffer.f_bavail)
        message.add_u64(buffer.f_files)
        message.add_u64(buffer.f_ffree)
        message.add_u64(buffer.f_namemax)


    def release(self, reader, message):
        path = self.get_path(reader)
        fd = reader.read_u64()
        result = 0
        try:
            os.close(fd)
        except OSError as ex:
            result = -ex.errno
        message.add_i32(result)

    def fsync(self, reader, message):
        path = self.get_path(reader)
        _ = reader.read_i32()
        fd = reader.read_u64()
        result = 0
        try:
            os.fsync(fd)
        except OSError as ex:
            result = -ex.errno
        message.add_i32(result)

    def lseek(self, reader, message):
        path = self.get_path(reader)
        offset = reader.read_u64()
        whence = reader.read_i32()
        fd = reader.read_u64()
        result = 0
        try:
            if fd != 0xffffffffffffffff:
                result = os.lseek(fd, offset, whence)
            else:
                with os.open(path, os.O_RDONLY) as f:
                    buffer = os.lseek(f, offset, whence)
        except OSError as ex:
            result = 0
        message.add_u64(result)


def main():
    parser = argparse.ArgumentParser(
        prog = 'stdiofd_provider',
        description = 'stdiofs provider')
    parser.add_argument('-p', '--path', type=str, required=True)
    parser.add_argument('command', type=str, nargs=argparse.REMAINDER)
    args = parser.parse_args()
    signal.signal(signal.SIGINT, on_shutdown_requested)
    provider = FilesystemProvider(args.path)
    provider.run(args.command)

if __name__ == '__main__':
    main()
