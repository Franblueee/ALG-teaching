#!/usr/bin/env bash
set -euo pipefail

shopt -s nullglob

dot_files=(""""$PWD""""/*.dot)

if [[ ${#dot_files[@]} -eq 0 ]]; then
  echo "No .dot files found in $PWD"
  exit 0
fi

for f in "${dot_files[@]}"; do
  out="${f%.dot}.png"
  dot -Tpng "$f" -o "$out"
  echo "Generated $out"
done
