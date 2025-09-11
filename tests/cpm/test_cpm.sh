#!/bin/bash

SCRIPT_DIR="$(dirname "$0")"
cd "$SCRIPT_DIR" || exit

if [ -n "$GITHUB_HEAD_REF" ]; then
    BRANCH_NAME="$GITHUB_HEAD_REF"
elif [ -n "$GITHUB_REF_NAME" ]; then
    BRANCH_NAME="$GITHUB_REF_NAME"
else
    BRANCH_NAME=$(git branch --show-current)
fi

echo "$BRANCH_NAME"

sed "s/__COMMIT_PLACEHOLDER__/$BRANCH_NAME/g" CMakeLists.txt.template > CMakeLists.txt

mkdir -p cmake
wget -O cmake/CPM.cmake https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake
cmake -S . -B build
cmake --build build
./build/cpmtest
