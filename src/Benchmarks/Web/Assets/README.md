# Web benchmark inputs

Real-world documents embedded into the Json Parsing, XML Parsing and Regex benchmarks (see `EmbedFile.cmake`), so
every platform parses or searches the same bytes. They are kept small (8-12 KB) so that the input plus the parsed
result stay inside the per-core cache of small CPUs (see `docs/development/web-benchmark-inputs.md`).

Do not edit these files: they are byte-exact copies (or, for the text, a plain excerpt) of the sources below. Any change
changes the benchmark results.

| File | Source | License |
|---|---|---|
| `JsonBenchmarkDocument.json` | Wikidata entity [Q2034599](https://www.wikidata.org/wiki/Q2034599) (*Mediavagina macropteri*), revision 2534270247, as served by `https://www.wikidata.org/wiki/Special:EntityData/Q2034599.json` | [CC0 1.0](https://creativecommons.org/publicdomain/zero/1.0/) (Wikidata structured data) |
| `XmlBenchmarkDocument.xml` | United States bill H.R. 2225 (119th Congress), reported in House (RH), "Access to Small Business Investor Capital Act", `https://www.govinfo.gov/bulkdata/BILLS/119/1/hr/BILLS-119hr2225rh.xml` | Public domain (work of the U.S. Government, 17 U.S.C. 105; stated in the file's `dc:rights`) |
| `RegexBenchmarkText.txt` | Jane Austen, *Pride and Prejudice* (1813), the first 11.7 KB (chapter 1 to part of chapter 3), from Project Gutenberg eBook #1342; illustration captions and the Project Gutenberg header and license removed | Public domain |
