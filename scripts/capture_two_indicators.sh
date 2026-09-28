#!/usr/bin/env bash
set -euo pipefail

project_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
platform="${1:?Usage: $0 <platform>}"
output_dir="${OUTPUT_DIR:-$project_dir/screenshots/${platform}-two-indicators}"
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

# Every capture explicitly disables the other three indicators so emulator
# persistence cannot affect the result.
capture() {
  local name="$1" bluetooth="$2" battery="$3" weather="$4" bpm="$5" steps="$6"
  local bluetooth_option=0 battery_option=0
  local frame="$output_dir/${name}.png"

  (( bluetooth )) && bluetooth_option=2
  (( battery )) && battery_option=2

  pebble send-app-message --emulator "$platform" --app-uuid "$app_uuid" --uint \
    "3=$bluetooth_option" "4=$battery_option" "9=$bpm" "10=$steps" "11=$weather" \
    "12=22" "13=1" "14=$capture_timestamp"

  if (( bpm )); then
    pebble emu-heart-rate --emulator "$platform" 100
  fi
  if (( steps )); then
    pebble emu-steps --emulator "$platform" 13947
  fi

  pebble emu-set-time --emulator "$platform" "$capture_timestamp"
  sleep "$time_settle_seconds"
  pebble screenshot --emulator "$platform" --no-open "$frame"
  echo "Captured $frame"
}

capture bluetooth-battery 1 1 0 0 0
capture bluetooth-weather 1 0 1 0 0
capture bluetooth-bpm     1 0 0 1 0
capture bluetooth-steps   1 0 0 0 1
capture battery-weather   0 1 1 0 0
capture battery-bpm       0 1 0 1 0
capture battery-steps     0 1 0 0 1
capture weather-bpm       0 0 1 1 0
capture weather-steps     0 0 1 0 1
capture bpm-steps         0 0 0 1 1
