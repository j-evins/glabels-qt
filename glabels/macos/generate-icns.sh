#!/bin/bash
# Generate macOS .icns file from SVG
# Uses available tools with fallbacks: rsvg-convert → inkscape → swift (native macOS)

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SVG_SOURCE="${SCRIPT_DIR}/../icons/glabels-flat/128x128/apps/glabels.svg"
ICONSET_DIR="${SCRIPT_DIR}/glabels.iconset"
ICNS_OUTPUT="${SCRIPT_DIR}/glabels.icns"

# Check for available SVG converter
if command -v rsvg-convert &> /dev/null; then
    SVG_CONVERTER="rsvg"
elif command -v inkscape &> /dev/null; then
    SVG_CONVERTER="inkscape"
elif command -v swift &> /dev/null; then
    SVG_CONVERTER="swift"
else
    echo "Error: No SVG converter found. Install librsvg (brew install librsvg) or Xcode."
    exit 1
fi

echo "Using $SVG_CONVERTER to convert SVG to PNG..."

# Create iconset directory
rm -rf "$ICONSET_DIR"
mkdir -p "$ICONSET_DIR"

# Swift helper for native macOS SVG→PNG conversion (no dependencies)
swift_convert() {
    local svg_path="$1"
    local png_path="$2"
    local size="$3"

    swift - "$svg_path" "$png_path" "$size" << 'SWIFT_EOF'
import AppKit

let args = CommandLine.arguments
let svgPath = args[1]
let pngPath = args[2]
let size = Int(args[3])!

guard let image = NSImage(contentsOfFile: svgPath) else {
    fputs("Failed to load SVG: \(svgPath)\n", stderr)
    exit(1)
}

let rect = NSRect(x: 0, y: 0, width: size, height: size)
guard let rep = NSBitmapImageRep(bitmapDataPlanes: nil, pixelsWide: size, pixelsHigh: size,
                                  bitsPerSample: 8, samplesPerPixel: 4, hasAlpha: true,
                                  isPlanar: false, colorSpaceName: .deviceRGB,
                                  bytesPerRow: 0, bitsPerPixel: 0) else {
    fputs("Failed to create bitmap\n", stderr)
    exit(1)
}

rep.size = NSSize(width: size, height: size)
NSGraphicsContext.saveGraphicsState()
NSGraphicsContext.current = NSGraphicsContext(bitmapImageRep: rep)
image.draw(in: rect, from: .zero, operation: .copy, fraction: 1.0)
NSGraphicsContext.restoreGraphicsState()

guard let data = rep.representation(using: .png, properties: [:]) else {
    fputs("Failed to create PNG data\n", stderr)
    exit(1)
}

do {
    try data.write(to: URL(fileURLWithPath: pngPath))
} catch {
    fputs("Failed to write PNG: \(error)\n", stderr)
    exit(1)
}
SWIFT_EOF
}

convert_svg() {
    local size=$1
    local output=$2

    case $SVG_CONVERTER in
        rsvg)
            rsvg-convert -w "$size" -h "$size" "$SVG_SOURCE" -o "$output"
            ;;
        inkscape)
            inkscape --export-type=png --export-width="$size" --export-height="$size" \
                     --export-filename="$output" "$SVG_SOURCE" 2>/dev/null
            ;;
        swift)
            swift_convert "$SVG_SOURCE" "$output" "$size"
            ;;
    esac
}

# Required sizes for macOS iconset
SIZES="16 32 128 256 512"

# Generate all required sizes
for size in $SIZES; do
    echo "Generating ${size}x${size}..."
    convert_svg "$size" "${ICONSET_DIR}/icon_${size}x${size}.png"

    # Also generate @2x versions (Retina)
    size2x=$((size * 2))
    if [ "$size2x" -le 1024 ]; then
        echo "Generating ${size}x${size}@2x (${size2x}x${size2x})..."
        convert_svg "$size2x" "${ICONSET_DIR}/icon_${size}x${size}@2x.png"
    fi
done

# Generate the icns file using iconutil (always available on macOS)
echo "Creating .icns file..."
iconutil -c icns "$ICONSET_DIR" -o "$ICNS_OUTPUT"

# Cleanup
rm -rf "$ICONSET_DIR"

echo "Done! Created: $ICNS_OUTPUT"
