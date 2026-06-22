#!/usr/bin/env python3
"""
Genera un patch BPS (target vs source) — encoder minimale ma VALIDO (lo
applicano flips/beat e il RomPatcherService del progetto). Niente dipendenze.

Uso:  python3 make_bps.py source.gba target.gba out.bps
"""
import sys
import zlib


def encnum(n):
    out = bytearray()
    while True:
        x = n & 0x7F
        n >>= 7
        if n == 0:
            out.append(0x80 | x)
            break
        out.append(x)
        n -= 1
    return bytes(out)


def make_bps(src, tgt):
    out = bytearray(b"BPS1")
    out += encnum(len(src))
    out += encnum(len(tgt))
    out += encnum(0)  # metadata vuota

    o = 0
    n = len(tgt)
    ls = len(src)
    while o < n:
        same = o < ls and tgt[o] == src[o]
        run = 1
        if same:
            while o + run < n and o + run < ls and tgt[o + run] == src[o + run]:
                run += 1
            out += encnum(((run - 1) << 2) | 0)  # SourceRead
        else:
            while o + run < n and not (o + run < ls and tgt[o + run] == src[o + run]):
                run += 1
            out += encnum(((run - 1) << 2) | 1)  # TargetRead
            out += tgt[o:o + run]
        o += run

    out += (zlib.crc32(src) & 0xFFFFFFFF).to_bytes(4, "little")
    out += (zlib.crc32(tgt) & 0xFFFFFFFF).to_bytes(4, "little")
    out += (zlib.crc32(bytes(out)) & 0xFFFFFFFF).to_bytes(4, "little")
    return bytes(out)


if __name__ == "__main__":
    src = open(sys.argv[1], "rb").read()
    tgt = open(sys.argv[2], "rb").read()
    bps = make_bps(src, tgt)
    open(sys.argv[3], "wb").write(bps)
    print(f"BPS scritto: {sys.argv[3]} ({len(bps)} byte) src={len(src)} tgt={len(tgt)}")
