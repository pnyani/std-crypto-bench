#!/usr/bin/env python3
"""
parse_ipb.py: extract Ir from a callgrind output file and compute IPB.

Usage:
    parse_ipb.py <callgrind_out> <algo> <data_bytes> <dir> <repeat>

    data_bytes: actual bytes processed per repeat (block-aligned)
    repeat:     number of measured iterations (must match bench REPEAT constant)

IPB = Ir / (data_bytes * repeat)
"""

import sys
import re


def parse_ir(filepath: str) -> int:
    with open(filepath, "r") as f:
        for line in f:
            m = re.match(r"^summary:\s+(\d+)", line)
            if m:
                return int(m.group(1))
    raise ValueError(f"'summary:' line not found in {filepath!r}")


def main() -> int:
    if len(sys.argv) != 6:
        print(
            f"Usage: {sys.argv[0]} "
            "<callgrind_out> <algo> <data_bytes> <dir> <repeat>",
            file=sys.stderr,
        )
        return 1

    out_file   = sys.argv[1]
    algo       = sys.argv[2]
    data_bytes = int(sys.argv[3])
    direction  = sys.argv[4]
    repeat     = int(sys.argv[5])

    try:
        ir = parse_ir(out_file)
    except (OSError, ValueError) as e:
        print(f"error: {e}", file=sys.stderr)
        return 1

    ipb = ir / (data_bytes * repeat)

    print(
        f"{algo:<12s}  {data_bytes:>10,d} B  {direction:3s}  "
        f"Ir={ir:>16,d}  IPB={ipb:8.2f}"
    )
    return 0


if __name__ == "__main__":
    sys.exit(main())
