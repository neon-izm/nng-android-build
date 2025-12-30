#!/bin/bash

# Setup script for sample-app to use AAR from GitHub Release
# This script downloads the AAR from GitHub Release and configures sample-app to use it

set -e

# Configuration
REPO_OWNER="neon-izm"
REPO_NAME="nng-android-build"
RELEASE_TAG="${1:-v1.11}"  # Default to v1.11, can be overridden
AAR_FILENAME="android-library-release.aar"
LIBS_DIR="sample-app/libs"
GRADLE_FILE="sample-app/build.gradle"
SETTINGS_FILE="settings.gradle"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "=========================================="
echo "NNG Android Library - Sample App Setup"
echo "=========================================="
echo ""
echo "This script will:"
echo "  1. Download AAR from GitHub Release (${RELEASE_TAG})"
echo "  2. Place it in ${LIBS_DIR}/"
echo "  3. Update sample-app/build.gradle to use the AAR"
echo ""

# Check if GitHub CLI is available
if ! command -v gh &> /dev/null; then
    echo -e "${RED}Error: GitHub CLI (gh) is not installed.${NC}"
    echo "Please install it from: https://cli.github.com/"
    exit 1
fi

# Check if authenticated
if ! gh auth status &> /dev/null; then
    echo -e "${YELLOW}Warning: Not authenticated with GitHub CLI.${NC}"
    echo "Attempting to authenticate..."
    gh auth login
fi

# Create libs directory
echo "📁 Creating libs directory..."
mkdir -p "${LIBS_DIR}"

# Download AAR from GitHub Release
echo ""
echo "📥 Downloading AAR from GitHub Release (${RELEASE_TAG})..."

# Get release assets
RELEASE_URL=$(gh release view "${RELEASE_TAG}" --repo "${REPO_OWNER}/${REPO_NAME}" --json assets --jq '.assets[] | select(.name == "'"${AAR_FILENAME}"'") | .url' 2>/dev/null || echo "")

if [ -z "$RELEASE_URL" ]; then
    echo -e "${RED}Error: Could not find ${AAR_FILENAME} in release ${RELEASE_TAG}${NC}"
    echo ""
    echo "Available releases:"
    gh release list --repo "${REPO_OWNER}/${REPO_NAME}" --limit 5
    exit 1
fi

# Download using gh release download
echo "Downloading ${AAR_FILENAME}..."
gh release download "${RELEASE_TAG}" \
    --repo "${REPO_OWNER}/${REPO_NAME}" \
    --pattern "${AAR_FILENAME}" \
    --dir "${LIBS_DIR}" \
    --clobber

if [ ! -f "${LIBS_DIR}/${AAR_FILENAME}" ]; then
    echo -e "${RED}Error: Failed to download AAR file${NC}"
    exit 1
fi

echo -e "${GREEN}✅ AAR downloaded successfully${NC}"
echo "   Location: ${LIBS_DIR}/${AAR_FILENAME}"
echo "   Size: $(du -h "${LIBS_DIR}/${AAR_FILENAME}" | cut -f1)"

# Backup original build.gradle
if [ -f "${GRADLE_FILE}" ]; then
    if [ ! -f "${GRADLE_FILE}.backup" ]; then
        echo ""
        echo "💾 Backing up original build.gradle..."
        cp "${GRADLE_FILE}" "${GRADLE_FILE}.backup"
    fi
fi

# Update build.gradle to use AAR file
echo ""
echo "📝 Updating sample-app/build.gradle..."

# Check if already using AAR
if grep -q "files('libs/${AAR_FILENAME}')" "${GRADLE_FILE}"; then
    echo -e "${YELLOW}⚠️  build.gradle already references the AAR file${NC}"
    echo "   Skipping build.gradle update"
else
    # Replace project dependency with file dependency
    if grep -q "implementation project(':android-library')" "${GRADLE_FILE}"; then
        # Use sed to replace the line
        if [[ "$OSTYPE" == "darwin"* ]]; then
            # macOS
            sed -i '' "s|implementation project(':android-library')|implementation files('libs/${AAR_FILENAME}')|g" "${GRADLE_FILE}"
        else
            # Linux
            sed -i "s|implementation project(':android-library')|implementation files('libs/${AAR_FILENAME}')|g" "${GRADLE_FILE}"
        fi
        echo -e "${GREEN}✅ Updated build.gradle to use AAR file${NC}"
    else
        echo -e "${YELLOW}⚠️  No project(':android-library') dependency found${NC}"
        echo "   You may need to manually add: implementation files('libs/${AAR_FILENAME}')"
    fi
fi

# Update settings.gradle to comment out android-library
echo ""
echo "📝 Updating settings.gradle..."
if grep -q "include ':android-library'" "${SETTINGS_FILE}"; then
    if [[ "$OSTYPE" == "darwin"* ]]; then
        # macOS: Comment out the line
        sed -i '' "s|include ':android-library'|// include ':android-library'  // Commented out: Using AAR from GitHub Release|g" "${SETTINGS_FILE}"
    else
        # Linux: Comment out the line
        sed -i "s|include ':android-library'|// include ':android-library'  // Commented out: Using AAR from GitHub Release|g" "${SETTINGS_FILE}"
    fi
    echo -e "${GREEN}✅ Commented out android-library in settings.gradle${NC}"
fi

# Verify AAR file
echo ""
echo "🔍 Verifying AAR file..."
if unzip -l "${LIBS_DIR}/${AAR_FILENAME}" | grep -q "AndroidManifest.xml"; then
    echo -e "${GREEN}✅ AAR file is valid${NC}"
else
    echo -e "${RED}⚠️  Warning: AAR file may be corrupted${NC}"
fi

# Summary
echo ""
echo "=========================================="
echo -e "${GREEN}✅ Setup completed successfully!${NC}"
echo "=========================================="
echo ""
echo "Next steps:"
echo "  1. Build the sample app:"
echo "     ./gradlew sample-app:assembleDebug"
echo ""
echo "  2. Install on device:"
echo "     adb install sample-app/build/outputs/apk/debug/sample-app-debug.apk"
echo ""
echo "To restore original configuration:"
echo "  cp ${GRADLE_FILE}.backup ${GRADLE_FILE}"
echo ""
echo "AAR file location: ${LIBS_DIR}/${AAR_FILENAME}"
echo "Release version: ${RELEASE_TAG}"
echo ""

