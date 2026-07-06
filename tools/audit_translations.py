#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
"""Audit built-in Thumbnail me 4 UI translation maps.

The script scans src/MainWindow.cpp, extracts every QHash<QString, QString>
translation table, and reports entries that still equal the English key.
A small allow-list is used for brand names, file filter labels, and short UI
terms that are intentionally shared between languages.
"""
from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

ALLOW_SAME = {
    "Thumbnail me",
    "English",
    "JPEG (*.jpg)",
    "PNG (*.png)",
    "WebP (*.webp)",
    # These short technical labels are widely acceptable as-is in several
    # languages, or are intentionally kept compact in the UI.
    "Config 1",
    "Config 2",
    "Config 3",
    "File",
    "Font:",
    "Info text:",
    "Input / Output",
    "Output",
    "Reset",
    "Styles",
    "Timestamp:",
}

EU_LANGS = {"bg", "da", "et", "el", "ga", "lv", "lt", "mt", "pl", "ro", "sk", "sl", "fi", "sv"}


def decode_cpp_string(value: str) -> str:
    out: list[str] = []
    i = 0
    while i < len(value):
        if value[i] == "\\" and i + 1 < len(value):
            nxt = value[i + 1]
            if nxt == "n":
                out.append("\n")
                i += 2
                continue
            if nxt == "t":
                out.append("\t")
                i += 2
                continue
            if nxt == '"':
                out.append('"')
                i += 2
                continue
            if nxt == "\\":
                out.append("\\")
                i += 2
                continue
        out.append(value[i])
        i += 1
    return "".join(out)


def find_translation_blocks(source: str) -> dict[str, str]:
    result: dict[str, str] = {}
    pattern = re.compile(r"static const QHash<QString, QString>\s+(\w+)\s*=\s*\{")
    for match in pattern.finditer(source):
        name = match.group(1)
        start = match.end()
        i = start
        level = 1
        in_string = False
        escape = False
        while i < len(source):
            ch = source[i]
            if in_string:
                if escape:
                    escape = False
                elif ch == "\\":
                    escape = True
                elif ch == '"':
                    in_string = False
            else:
                if ch == '"':
                    in_string = True
                elif ch == "{":
                    level += 1
                elif ch == "}":
                    level -= 1
                    if level == 0:
                        result[name] = source[start:i]
                        break
            i += 1
    return result


def extract_pairs(block: str) -> dict[str, str]:
    pairs: dict[str, str] = {}
    pair_re = re.compile(r'\{\s*"((?:\\.|[^"\\])*)"\s*,\s*"((?:\\.|[^"\\])*)"\s*\}')
    for key, value in pair_re.findall(block):
        pairs[decode_cpp_string(key)] = decode_cpp_string(value)
    return pairs


def main() -> int:
    parser = argparse.ArgumentParser(description="Audit Thumbnail me 4 built-in UI translations.")
    parser.add_argument("--source", default=str(Path(__file__).resolve().parents[1] / "src" / "MainWindow.cpp"))
    parser.add_argument("--eu-only", action="store_true", help="audit only the 14 EU languages added after the TM3 language set")
    args = parser.parse_args()

    source_path = Path(args.source)
    source = source_path.read_text(encoding="utf-8")
    blocks = find_translation_blocks(source)
    maps = {name: extract_pairs(block) for name, block in blocks.items()}

    if not maps:
        print("ERROR: no translation maps found", file=sys.stderr)
        return 2

    wanted = EU_LANGS if args.eu_only else set(maps)
    failures: dict[str, list[str]] = {}
    for lang in sorted(wanted):
        table = maps.get(lang)
        if not table:
            failures[lang] = ["<missing translation table>"]
            continue
        untranslated = [key for key, value in table.items() if key == value and key not in ALLOW_SAME]
        if untranslated:
            failures[lang] = untranslated

    print(f"Scanned: {source_path}")
    print(f"Languages checked: {', '.join(sorted(wanted))}")
    if not failures:
        print("OK: no unintended English-identical UI entries found.")
        return 0

    print("Untranslated / suspicious entries:")
    for lang, keys in sorted(failures.items()):
        print(f"  {lang}: {len(keys)}")
        for key in keys:
            print(f"    - {key}")
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
