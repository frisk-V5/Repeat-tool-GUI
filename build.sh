#!/bin/bash

# エラーが発生したら処理を中断する
set -e

# コンパイラとオプションの設定
CC=gcc
CFLAGS="-Wall -Wextra -O2"

# 実行ファイル名とソースファイル名の指定
TARGET="myapp"
SOURCE="main.c"

echo "Building $SOURCE with $CC..."

# コンパイルの実行
$CC $CFLAGS -o $TARGET $SOURCE

echo "Build successful! Running $TARGET:"
echo "----------------------------------------"

# 成果物の実行
./$TARGET
