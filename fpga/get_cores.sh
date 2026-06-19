#!/usr/bin/env bash
# Clone the external soft cores used by Herring-F.

set -euo pipefail

# Clone into the cores/ directory next to this script, regardless of CWD.
CORES_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/cores"

clone_core() {
    local name="$1" url="$2" dest="$CORES_DIR/$1"
    if [ -d "$dest" ]; then
        echo "Skipping $name (already present at $dest)"
        return
    fi
    echo "Cloning $name into $dest..."
    git clone "$url" "$dest"
}

# Arlet Ottens' 6502 core
clone_core verilog-6502 https://github.com/Arlet/verilog-6502.git

# 65C51 ACIA core
clone_core verilog-65c51 https://github.com/crmaykish/verilog-65c51.git

echo "Done."
