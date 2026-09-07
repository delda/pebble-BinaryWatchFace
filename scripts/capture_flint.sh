#!/usr/bin/env bash
# Capture the documented Flint watchface configurations into a GIF.
set -euo pipefail

project_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
platform="${1:-flint}"
output_dir="${2:-$project_dir/screenshots/configurations-flint}"
frames_dir="$output_dir/frames"
app_uuid="1d23d5b6-ee9c-49a6-8f6e-efc6a7335c77"
time_settle_seconds="${TIME_SETTLE_SECONDS:-2}"
# 1 January 2030, 10:00:00 UTC (also CET in January).
capture_timestamp=1893492000
emulator_logs_pid=""
emulator_logs_file=""

if [[ "$platform" != "flint" ]]; then
  echo "This capture set targets the Flint emulator." >&2
  exit 2
fi

if ! command -v ffmpeg >/dev/null; then
  echo "ffmpeg is required to create the GIF." >&2
  exit 1
fi

if ! type nvm >/dev/null 2>&1; then
  source "$HOME/.nvm/nvm.sh"
fi
nvm use v24.14.0

cd "$project_dir"
mkdir -p "$frames_dir"
rm -f "$frames_dir"/*.png "$output_dir/binary-watchface-configurations.gif"

stop_emulator() {
  if [[ -n "$emulator_logs_pid" ]]; then
    kill "$emulator_logs_pid" 2>/dev/null || true
    wait "$emulator_logs_pid" 2>/dev/null || true
  fi
  [[ -z "$emulator_logs_file" ]] || rm -f "$emulator_logs_file"
}

start_emulator() {
  local attempt
  pebble kill --force >/dev/null 2>&1 || true
  emulator_logs_file="$(mktemp)"
  pebble logs -vv --emulator "$platform" >"$emulator_logs_file" 2>&1 &
  emulator_logs_pid=$!
  for attempt in {1..30}; do
    if rg -q 'Firmware booted\.' "$emulator_logs_file" && rg -q 'pypkjs:Ready\.' "$emulator_logs_file"; then
      return 0
    fi
    if ! kill -0 "$emulator_logs_pid" 2>/dev/null; then
      break
    fi
    sleep 1
  done
  cat "$emulator_logs_file" >&2
  echo "Unable to start a ready $platform emulator." >&2
  return 1
}

trap stop_emulator EXIT

if [[ ! -d node_modules/@rebble/clay ]]; then
  npm install --package-lock=false --ignore-scripts
fi

pebble build
pbw_file="$(find build -maxdepth 1 -type f -name '*.pbw' -print -quit)"
if [[ -z "$pbw_file" ]]; then
  echo "The Pebble build completed but did not produce a .pbw file." >&2
  exit 1
fi

start_emulator
pebble install --emulator "$platform" --force "$pbw_file"
pebble emu-battery --emulator "$platform" --percent 72
pebble emu-bt-connection --emulator "$platform" --connected yes

capture() {
  local index="$1" name="$2" shape="$3" color="$4" numbers="$5"
  local bit_values="$6" bluetooth="$7" battery="$8" date_format="$9"
  local frame="$frames_dir/${index}-${name}.png"

  pebble send-app-message --emulator "$platform" --app-uuid "$app_uuid" --uint \
    "0=$shape" "1=$color" "2=$numbers" "3=$bluetooth" "4=$battery" \
    "5=0" "6=$date_format" "7=$bit_values" "8=0"
  pebble emu-set-time --emulator "$platform" "$capture_timestamp"
  sleep "$time_settle_seconds"
  pebble screenshot --emulator "$platform" --no-open "$frame"
  echo "Captured $frame"
}

# Flint is monochrome; the colour values deliberately resolve to its B/W palette.
capture 01 circle-light-minimal       0 2 0 0 0 0 0
capture 02 circle-light-bit-values    0 2 0 1 0 0 0
capture 03 circle-light-bluetooth     0 2 0 1 2 0 0
capture 04 circle-light-battery       0 2 0 1 2 2 0
capture 05 circle-light-date          0 2 0 1 2 2 23
capture 06 circle-light-digits        0 2 1 1 2 2 23
capture 07 square-light-digits        1 2 1 1 2 2 23
capture 08 square-dark                1 1 1 1 2 2 23

ffmpeg -y -framerate 1 -pattern_type glob -i "$frames_dir/*.png" \
  -filter_complex '[0:v]split[frames][palette];[palette]palettegen=stats_mode=single[pal];[frames][pal]paletteuse=new=1' \
  -loop 0 "$output_dir/binary-watchface-configurations.gif"

echo "GIF created: $output_dir/binary-watchface-configurations.gif"
