#!/usr/bin/env bash
set -euo pipefail

# -------------------------------------------------
# Colors
# -------------------------------------------------
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
NC='\033[0m'

msg()  { echo -e "${CYAN}$*${NC}"; }
ok()   { echo -e "${GREEN}$*${NC}"; }
warn() { echo -e "${YELLOW}$*${NC}"; }
err()  { echo -e "${RED}$*${NC}"; exit 1; }

msg "🎨 Compiling shaders..."

ROOT_DIR="."
TARGETS=("$@")

OS_TYPE=$(uname -s)
MACHINE_TYPE=$(uname -m)

# -------------------------------------------------
# Detect sokol-shdc binary
# -------------------------------------------------
case "$OS_TYPE" in
    Darwin)
        [[ "$MACHINE_TYPE" == "arm64" ]] && SOKOL_OS="osx_arm64" || SOKOL_OS="osx"
        ;;
    Linux)
        SOKOL_OS="linux"
        ;;
    *)
        warn "Unknown OS ($OS_TYPE). Using linux tools."
        SOKOL_OS="linux"
        ;;
esac

# -------------------------------------------------
# If no targets provided → auto detect
# -------------------------------------------------
if [[ ${#TARGETS[@]} -eq 0 ]]; then
    case "$OS_TYPE" in
        Darwin)
            TARGETS=("mac")
            ;;
        Linux)
            TARGETS=("desktop")
            ;;
        *)
            TARGETS=("android")
            ;;
    esac
fi

# -------------------------------------------------
# Map targets → shader backends
# -------------------------------------------------
BACKENDS=()

for target in "${TARGETS[@]}"; do
    case "$target" in
        mac|macos)
            BACKENDS+=("metal_macos")
            ;;
        ios)
            BACKENDS+=("metal_ios")
            ;;
        linux|windows|desktop)
            BACKENDS+=("glsl410")
            ;;
        android)
            BACKENDS+=("glsl300es")
            ;;
        web)
            BACKENDS+=("glsl300es")
            ;;
        *)
            err "Unknown target: $target"
            ;;
    esac
done

# -------------------------------------------------
# Deduplicate backends
# -------------------------------------------------
BACKENDS=($(printf "%s\n" "${BACKENDS[@]}" | sort -u))

SHADER_LANG=$(IFS=:; echo "${BACKENDS[*]}")

msg "Targets: ${TARGETS[*]}"
msg "Shader backends: $SHADER_LANG"

# -------------------------------------------------
# Setup sokol tools
# -------------------------------------------------
SOKOL_TOOLS_DIR="$ROOT_DIR/tools/sokol_tools"
SOKOL_SHDC="$SOKOL_TOOLS_DIR/bin/$SOKOL_OS/sokol-shdc"

if [[ ! -d "$SOKOL_TOOLS_DIR" ]]; then
    msg "📥 Downloading sokol-tools..."
    mkdir -p "$ROOT_DIR/tools"
    git clone --depth 1 https://github.com/floooh/sokol-tools-bin.git "$SOKOL_TOOLS_DIR"
fi

[[ -f "$SOKOL_SHDC" ]] || err "sokol-shdc not found for $SOKOL_OS"

chmod +x "$SOKOL_SHDC"

msg "Using sokol-shdc: $SOKOL_SHDC"

# -------------------------------------------------
# Compile shaders
# -------------------------------------------------
count=0

while IFS= read -r -d '' shader; do
    out="${shader%.glsl}.glsl.h"

    msg "  ▶ $(basename "$shader")"

    "$SOKOL_SHDC" \
        -i "$shader" \
        -o "$out" \
        -l "$SHADER_LANG"

    ((count++))

done < <(find "$ROOT_DIR/app/src/main/cpp/shaders" -name "*.glsl" -type f -print0)

ok "✔ Compiled $count shaders"