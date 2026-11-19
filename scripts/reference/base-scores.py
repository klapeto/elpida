#!/usr/bin/env python3
# Computes the base scores from reference-run results (elpida CLI --format=json files) and optionally writes them into
# apps/Application/BenchmarkBaseScores.hpp. See docs/development/reference-run.md.
#
#   python3 scripts/reference/base-scores.py RESULT.json [RESULT.json ...]            table: median and spread
#   python3 scripts/reference/base-scores.py --write RESULT.json [RESULT.json ...]    also update the header
#
# Each file may hold several runs (--iterations=N). The base score of a benchmark is the median of its results over
# all runs. All files must come from the same CPU and the same Elpida version.
import argparse
import json
import re
import statistics
import sys
from pathlib import Path

HEADER = Path(__file__).resolve().parents[2] / "apps/Application/BenchmarkBaseScores.hpp"
# "Name", { "single-thread uuid", score, "multi-thread uuid", score,
ENTRY = re.compile(r'"([^"]+)",\s*\{\s*"([0-9a-f-]{36})",\s*([0-9.eE+-]+),\s*"([0-9a-f-]{36})",\s*([0-9.eE+-]+),')


def load(path):
    text = Path(path).read_text(encoding="utf-8")
    data, _ = json.JSONDecoder().raw_decode(text)
    return data


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--write", action="store_true", help="write the medians into BenchmarkBaseScores.hpp")
    parser.add_argument("results", nargs="+")
    args = parser.parse_args()

    header = HEADER.read_text(encoding="utf-8")
    entries = ENTRY.findall(header)
    names = {}
    for name, st, _, mt, _ in entries:
        names[st] = (name, "single")
        names[mt] = (name, "multi")

    values, cpus, versions, runs = {}, set(), set(), 0
    for path in args.results:
        data = load(path)
        cpus.add(data["cpu"]["modelName"])
        versions.add(data["elpidaVersion"]["version"])
        for run in data["results"]:
            runs += 1
            for result in run["benchmarkResults"]:
                values.setdefault(result["uuid"], []).append(result["result"])

    if len(cpus) != 1 or len(versions) != 1:
        sys.exit(f"error: results must come from one CPU and one Elpida version, got {cpus} {versions}")
    unknown = set(values) - set(names)
    missing = set(names) - set(values)
    if unknown or missing:
        sys.exit(f"error: results do not match the header: unknown {sorted(unknown)} missing "
                 f"{sorted(names[u] for u in missing)}")

    print(f"{cpus.pop()}, Elpida {versions.pop()}, {runs} runs")
    print(f"{'Benchmark':32} {'single-thread':>16} {'spread':>7} {'multi-thread':>16} {'spread':>7}")
    medians = {uuid: statistics.median(v) for uuid, v in values.items()}
    worst = 0.0
    for name, st, _, mt, _ in entries:
        cells = []
        for uuid in (st, mt):
            v = values[uuid]
            spread = (max(v) - min(v)) / medians[uuid] * 100
            worst = max(worst, spread)
            cells.append(f"{medians[uuid]:16.6f} {spread:6.2f}%")
        print(f"{name:32} {' '.join(cells)}")
    print(f"largest spread (max-min)/median: {worst:.2f}%")

    if args.write:
        def replace(match):
            name, st, _, mt, _ = match.groups()
            return f'"{name}",\n                    {{\n                        "{st}",\n                        ' \
                   f'{medians[st]:.6f},\n                        "{mt}",\n                        {medians[mt]:.6f},'
        HEADER.write_text(ENTRY.sub(replace, header), encoding="utf-8")
        print(f"wrote {HEADER}")


if __name__ == "__main__":
    main()
