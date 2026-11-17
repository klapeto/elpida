i9-10900K, Ubuntu 26.04. Same binaries as the packages (x86_64-linux-gnu).
  interleaved.txt                 find / FindIndex / LUT, unpinned (performance governor)
  perf-stat-*.csv                 perf-stat.sh, pinned to CPU 2 (performance governor): score,event=count,...
                                  find 24 runs, FindIndex 15, LUT 15, Base64 encode 20. Analyse with correlate.py.
  layout-web.txt                  layout test, 10 rounds x 5 web benchmarks: "<index> normal|norand|fixed <score>"
                                  (norand = mimalloc-fixed-start patch, fixed = that + setarch -R), --affinity=2,
                                  powersave governor
  layout-base64-decode-aslr.txt   15 rounds: normal binary with ASLR (aslr) and with setarch -R (noaslr), --affinity=2
  layout-base64-decode-fixed.txt  20 rounds: normal binary (normal) and mimalloc-fixed-start + setarch -R (fixed)
