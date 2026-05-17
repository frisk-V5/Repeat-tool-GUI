set -e
USER_NAME="asahi"
SOURCE_FILE="main.c"
OUTPUT_EXE="program"
TARGET_DIR="/c/Users/${USER_NAME}/Desktop"
cd "${TARGET_DIR}"
echo "========================================"
echo "[INFO] ビルドを開始します..."
echo "[Target] ${TARGET_DIR}/${SOURCE_FILE}"
echo "========================================"
if [ ! -f "${SOURCE_FILE}" ]; then
    echo "[ERROR] ${SOURCE_FILE} がデスクトップに見つかりません。"
    exit 1
fi
gcc -Wall "${SOURCE_FILE}" -o "${OUTPUT_EXE}" -mwindows
echo "[SUCCESS] ビルドが正常に完了しました: ${OUTPUT_EXE}.exe"
echo "----------------------------------------"
echo "[INFO] 実行ファイルを実行します..."
./${OUTPUT_EXE}.exe
echo "[INFO] 終了しました。"
echo "========================================"
