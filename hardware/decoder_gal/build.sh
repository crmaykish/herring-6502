#!/bin/sh
echo "Building Herring Decoder for GAL..."

galasm herring_decoder.pld

minipro -p ATF16V8B -w herring_decoder.jed

echo "Done!"