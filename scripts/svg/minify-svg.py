#!/usr/bin/env python3
"""minify-svg.py IN.svg OUT.svg

Minifies an Inkscape SVG for embedding in a benchmark: drops the XML declaration, comments, <metadata>, <title>,
the Inkscape/Sodipodi editor elements and attributes, unused namespaces, ids nothing references, and style
properties that only affect text, filters or rendering hints. Geometry, transforms and paint are kept as written
(numbers are not rewritten), so the minified document renders identically.
"""
import re
import sys

KEEP_STYLE = {
    "display", "visibility", "opacity", "mix-blend-mode", "isolation",
    "fill", "fill-opacity", "fill-rule",
    "stroke", "stroke-width", "stroke-opacity", "stroke-linejoin", "stroke-linecap", "stroke-miterlimit",
    "stroke-dasharray", "stroke-dashoffset",
    "stop-color", "stop-opacity",
}


def clean_style(m):
    props = []
    for p in m.group(1).split(";"):
        if ":" not in p:
            continue
        k, v = p.split(":", 1)
        k, v = k.strip(), v.strip()
        if k in KEEP_STYLE:
            props.append(f"{k}:{v}")
    return f' style="{";".join(props)}"' if props else ""


def main():
    text = open(sys.argv[1], encoding="utf-8").read()

    text = re.sub(r"<\?xml[^>]*\?>", "", text)
    text = re.sub(r"<!--.*?-->", "", text, flags=re.S)
    for tag in ("metadata", "title", "sodipodi:namedview"):
        text = re.sub(rf"<{tag}\b[^>]*/>", "", text, flags=re.S)
        text = re.sub(rf"<{tag}\b.*?</{tag}>", "", text, flags=re.S)
    text = re.sub(r'\s(?:inkscape|sodipodi):[\w-]+="[^"]*"', "", text)
    text = re.sub(r'\sxmlns:(?:inkscape|sodipodi|rdf|cc|dc|svg)="[^"]*"', "", text)

    referenced = set(re.findall(r"url\(#([^)]+)\)", text)) | set(re.findall(r'href="#([^"]+)"', text))
    text = re.sub(r'\sid="([^"]*)"', lambda m: m.group(0) if m.group(1) in referenced else "", text)

    text = re.sub(r'\sstyle="([^"]*)"', clean_style, text)

    text = re.sub(r"\s+", " ", text)
    text = re.sub(r">\s+<", "><", text)
    text = re.sub(r"\s+(/?>)", r"\1", text)
    open(sys.argv[2], "w", encoding="utf-8", newline="").write(text.strip())


if __name__ == "__main__":
    main()
