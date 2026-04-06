#!/usr/bin/env bash
# snobol4ever_clone.sh — initial clone of all snobol4ever repos
#
# Usage:
#   cd ~/wherever-you-want-the-code
#   bash snobol4ever_clone.sh --token ghp_YOUR_TOKEN
#
# Each repo is cloned as a subdirectory here.
# Safe to re-run — already-cloned repos are skipped (use GitHub Desktop to update).

set -euo pipefail

TOKEN="${GH_TOKEN:-}"
while [[ $# -gt 0 ]]; do
    case "$1" in
        --token) TOKEN="$2"; shift 2 ;;
        --ssh)   USE_SSH=1;  shift   ;;
        *)       echo "Unknown option: $1"; exit 1 ;;
    esac
done
USE_SSH="${USE_SSH:-0}"

REPOS=(
    .github
    corpus
    harness
    one4all
    snobol4artifact
    snobol4csharp
    snobol4dotnet
    snobol4jvm
    snobol4python
    x32
    x64
)

for repo in "${REPOS[@]}"; do
    if [[ -d "$repo/.git" ]]; then
        echo "SKIP    $repo  (already cloned — use GitHub Desktop to update)"
        continue
    fi
    if [[ $USE_SSH -eq 1 ]]; then
        url="git@github.com:snobol4ever/${repo}.git"
    elif [[ -n "$TOKEN" ]]; then
        url="https://${TOKEN}@github.com/snobol4ever/${repo}"
    else
        url="https://github.com/snobol4ever/${repo}"
    fi
    echo "CLONE   $repo"
    git clone --quiet "$url" "$repo"
done

echo ""
echo "Done. Open GitHub Desktop, add these folders, and you're set."
