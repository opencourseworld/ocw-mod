#!/usr/bin/env python3

# vim: ts=4 sts=4 sw=4 et

import re
import yaml
import ipdb
import json

def esc(s):
    return json.dumps(str(s))

ui_map = {}

def gen_lines():
    yield "// DO NOT EDIT THIS FILE"
    yield f"// this file was automatically generated by misc/scripts/codegen.py"
    yield "// and is configured by misc/resources/ui_map.yml"
    yield ""

    for szs, msbts in ui_map.items():
        for msbt in msbts.keys():
            yield f"static eui::MessageSet* source__{szs}__{msbt} = nullptr;"

    yield f"static eui::MessageSet* messageOverride = nullptr;"
    yield ""

    yield "UILookup TryOverride(UILookup orig) {"
    yield "  UILookup ocwOverride;"
    yield "  ocwOverride.msbt = messageOverride;"
    _else1 = ''
    for szs, msbts in ui_map.items():
        for msbt, labels in msbts.items():
            yield f"  {_else1}if (orig.msbt == source__{szs}__{msbt}) {'{'}"
            _else1 = "else "
            _else2 = ''
            for label, ocw_label in labels.items():
                # yaml moment
                label = str(label)
                yield f"    {_else2}if (strncmp(orig.label, {esc(label)}, {1+len(label)}) == 0) {'{'}"
                _else2 = "else "
                yield f"      ocwOverride.label = {esc(ocw_label)};"
                yield f"      return ocwOverride;"
                yield "    }"
            yield "  }"



    yield "  return orig;"
    yield "}"
    yield ""


    yield "inline void TryDetectMessageSet(sead::FixedSafeString<256> name, eui::MessageSet* set) {"
    yield f"  if (name == \"OpenCourseWorldMsg.msbt\") messageOverride = set;"
    for szs, msbts in ui_map.items():
        for msbt in msbts.keys():
            yield f"  else if (name == {esc(msbt + '.msbt')}) source__{szs}__{msbt} = set;"

    yield '}'
    yield ""

if __name__ == '__main__':
    import sys
    argv = sys.argv[1:]
    outfile = sys.stdout
    infile = 'misc/resources/ui_map.yml'

    while len(argv):
        arg = argv.pop(0)
        if arg == '-o' or arg == '--output':
            outfile = open(argv.pop(0), 'w')
        else:
            infile = arg

    with open(infile) as f:
        ui_map = yaml.load(f.read(), Loader=yaml.Loader)

    for line in gen_lines():
        outfile.write(line)
        outfile.write("\n")
