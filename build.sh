#!/bin/bash

# エラーが発生したら即座にスクリプトを終了させる設定
set -e

# ユーザー名と対象ファイルの設定
USER_NAME="asahi"
SOURCE_FILE="main.c"
OUTPUT_EXE="program"

# 1. デスクトップディレクトリへ移動
TARGET_DIR="/c/Users/${USER_NAME}/Desktop"
cd "${TARGET_DIR}"

echo "========================================"
echo "[INFO] ビルドを開始します..."
echo "[Target] ${TARGET_DIR}/${SOURCE_FILE}"
echo "========================================"

# 2. ソースファイルの存在確認
if [ ! -f "${SOURCE_FILE}" ]; then
    echo "[ERROR] ${SOURCE_FILE} がデスクトップに見つかりません。"
    exit 1
fi

# 3. GCCによるコンパイルを実行（コンソール非表示オプション付き）
# -Wall    : 全ての警告を表示
# -mwindows: コンソールウィンドウを非表示にする
gcc -Wall "${SOURCE_FILE}" -o "${OUTPUT_EXE}" -mwindows

echo "[SUCCESS] ビルドが正常に完了しました: ${OUTPUT_EXE}.exe"
echo "----------------------------------------"
echo "[INFO] 実行ファイルを実行します..."

# 4. 生成された実行ファイルの起動
./${OUTPUT_EXE}.exe

echo "[INFO] 終了しました。"
echo "========================================"
