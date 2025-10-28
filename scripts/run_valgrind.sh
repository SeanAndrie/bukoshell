#!/bin/bash
# =========================================================
#  Minishell Valgrind Runner
#  Runs minishell with detailed Valgrind analysis
# =========================================================

# Colors for readability
GREEN="\033[0;32m"
RED="\033[0;31m"
RESET="\033[0m"

# Project root (one directory up from this script)
ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
EXEC="$ROOT_DIR/minishell"
SUPP_DIR="$ROOT_DIR/suppress"

# Suppression files
SUPP_FILES=(
    "$SUPP_DIR/bin.supp"
    "$SUPP_DIR/readline.supp"
)

# Optional: test script argument
TARGET_SCRIPT="${1:-}"

# Check if executable exists
if [ ! -x "$EXEC" ]; then
    echo -e "${RED}Error:${RESET} '$EXEC' not found or not executable."
    exit 1
fi

# Build suppression arguments dynamically
SUPP_ARGS=""
for file in "${SUPP_FILES[@]}"; do
    if [ -f "$file" ]; then
        SUPP_ARGS+=" --suppressions=$file"
    else
        echo -e "${RED}Warning:${RESET} Suppression file not found: $file"
    fi
done

# Valgrind command
VALGRIND_CMD=(
    valgrind
    --leak-check=full
    --leak-resolution=high
    -s
    --track-origins=yes
    --num-callers=500
    --show-mismatched-frees=yes
    --show-leak-kinds=all
    --track-fds=yes
    --trace-children=yes
    --gen-suppressions=no
    --error-limit=no
    --undef-value-errors=yes
    --expensive-definedness-checks=yes
    --read-var-info=yes
    --keep-debuginfo=yes
)

# Header
echo -e "${GREEN}Running Valgrind on minishell...${RESET}"
echo "Suppression files: $SUPP_ARGS"
echo

# Run minishell directly or run test script if provided
if [ -n "$TARGET_SCRIPT" ]; then
    echo "Running test script: $TARGET_SCRIPT"
    echo
    "${VALGRIND_CMD[@]}" $SUPP_ARGS bash "$TARGET_SCRIPT"
else
    "${VALGRIND_CMD[@]}" $SUPP_ARGS "$EXEC"
fi
