#!/usr/bin/env python3

import pymsb
from sys import argv
import ipdb
import yaml
import libyaz0
import sys

import SarcLib

DEFAULT_ATTRS = b'\x12\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

class SillyAdapter(pymsb.LMSAdapter):
    @property
    def supports_attributes(self) -> bool:
        return True

    def parse_attributes(self, stream, root_offset, root_size):
        stream.seek(root_offset)
        out = { "attrs": stream.read(root_size) }
        return out

    def write_attributes(self, stream, attributes):
        stream.write(DEFAULT_ATTRS)

    def create_default_attributes(self):
        return {}

    def use_fixed_buckets(self): return True

    def read_tag(self, stream):
        tag_group = stream.read_u16()
        tag_index = stream.read_u16()
        tag_size = stream.read_u16()
        params = stream.read(tag_size)
        params_repr = ",".join([str(c) for c in params])
        return f'[{tag_group},{tag_index},{params_repr}{icon}]'

def dump(argv):
    fname = argv[0]

    msbt = pymsb.msbt_from_file(SillyAdapter, fname)

    for m in sorted(msbt.messages, key=lambda x: x.label):
        print(f"{m.label}: {m.text.strip()}")

def parse_yml(infile, default={}):
    with open(infile) as f:
        text = f.read()

    doc = yaml.load(text, Loader=yaml.Loader)
    msbt = pymsb.LMSDocument(SillyAdapter)

    msbt.set_little_endian()

    for k, v in doc.items():
        m = msbt.new_message(k)
        m.text = v

    for k, v in default.items():
        if k in doc: continue
        m = msbt.new_message(k)
        m.text = v

    return msbt

def pack_szs(outfile, elements={}, default={}):
    arc = SarcLib.SARC_Archive(endianness='<')

    for path, source in elements.items():
        msbt = parse_yml(source, default)
        binary = msbt.makebin()
        file = SarcLib.File(path, binary)
        arc.addFile(file)

    data, maxAlignment = arc.save()
    outData = libyaz0.compress(data, maxAlignment, 1)
    del data

    with open(outfile, "wb+") as out:
        out.write(outData)

def convert(argv):
    outfile = 'out.msbt'
    infile = 'in.yml'

    while len(argv):
        arg = argv.pop(0)

        if arg == '-o' or arg == '--output':
            outfile = argv.pop(0)
        elif arg == '-i' or arg == '--input':
            infile = argv.pop(0)
        elif arg[0] != '-':
            infile = arg

    msbt = parse_yml(infile)

    pymsb.msbt_write_file(msbt, outfile)

def pack(argv):
    outfile = 'out.szs'
    elements = {}
    default = {}

    while len(argv):
        arg = argv.pop(0)

        if arg == '-o' or arg == '--output':
            outfile = argv.pop(0)
        if arg == '-d' or arg == '--default':
            with open(argv.pop(0)) as f:
                default = yaml.load(f.read(), Loader=yaml.Loader)
        else:
            try:
                (path, infile) = arg.split('=')
                elements[path] = infile
            except ValueError:
                infile = arg

    pack_szs(outfile, elements, default)


def main(argv):
    argv.pop(0)
    command = argv.pop(0)

    if command == 'dump':
        return dump(argv)
    elif command == 'convert':
        return convert(argv)
    elif command == 'pack':
        return pack(argv)

if __name__ == '__main__':
    sys.exit(main(sys.argv[:]))
