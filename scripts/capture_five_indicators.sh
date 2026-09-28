#!/usr/bin/env bash
# Capture the complete configuration containing all five indicators.
set -euo pipefail

project_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
platform="${1:?Usage: $0 <platform>}"
output_dir="${OUTPUT_DIR:-$project_dir/screenshots/${platform}-five-indicators}"
app_uuid="1d23d5b6-ee9c-49a6-8f6e-efc6a7335c77"
capture_timestamp=1893578400
time_settle_seconds="${TIME_SETTLE_SECONDS:-1}"
emulator_logs_pid=""
emulator_logs_file=""

if ! type nvm >/dev/null 2>&1; then
  source "$HOME/.nvm/nvm.sh"
fi
nvm use v24.14.0 >/dev/null

cd "$project_dir"
mkdir -p "$output_dir"

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
  sleep 2
  emulator_logs_file="$(mktemp)"
  pebble logs -vv --emulator "$platform" >"$emulator_logs_file" 2>&1 &
  emulator_logs_pid=$!
  for attempt in {1..30}; do
    if rg -q 'Firmware booted\.' "$emulator_logs_file" &&
       rg -q 'pypkjs:Ready\.' "$emulator_logs_file"; then
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

if [[ "${SKIP_BUILD:-0}" != "1" ]]; then
  pebble build
fi

pbw_file="$project_dir/build/pebble-BinaryWatchFace.pbw"
if [[ ! -f "$pbw_file" ]]; then
  echo "The Pebble build did not produce $pbw_file." >&2
  exit 1
fi

start_emulator
pebble install --emulator "$platform" --force "$pbw_file"
pebble emu-battery --emulator "$platform" --percent 72
pebble emu-bt-connection --emulator "$platform" --connected yes

# Set all fixed watchface settings before the indicator configuration.
pebble send-app-message --emulator "$platform" --app-uuid "$app_uuid" --uint \
  "0=0" "1=0" "2=0" "5=0" "6=23" "7=1" "8=0"

name="bluetooth-battery-weather-bpm-steps"
frame="$output_dir/${name}.png"

# Explicitly enable every indicator so emulator persistence cannot affect the
# result if this script is run after another capture script.
pebble send-app-message --emulator "$platform" --app-uuid "$app_uuid" --uint \
  "3=2" "4=2" "9=1" "10=1" "11=1" \
  "12=22" "13=1" "14=$capture_timestamp"
pebble emu-heart-rate --emulator "$platform" 112
pebble emu-steps --emulator "$platform" 13938
pebble emu-set-time --emulator "$platform" "$capture_timestamp"
sleep "$time_settle_seconds"
pebble screenshot --emulator "$platform" --no-open "$frame"
echo "Captured $frame"
