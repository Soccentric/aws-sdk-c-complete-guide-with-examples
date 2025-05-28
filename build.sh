#!/bin/bash

# Build script for AWS SDK for C++ Professional Examples
# This project has a proper directory structure:
# - src/lib/: Core AWS service wrapper classes
# - src/app/: Example applications using the library
# - include/awsexamples/: Public API headers
# - test/: Unit tests

# Set default options
BUILD_TYPE="Release"
JOBS=$(nproc 2>/dev/null || echo 2)
DO_CLEAN=0
DO_INSTALL=0
DO_TEST=0
DO_PACKAGE=0
DO_DOCS=0
PACKAGE_TYPE=""
CMAKE_EXTRA_ARGS=""
INSTALL_PREFIX="/usr/local"

# Function to show usage
show_usage() {
    echo "AWS SDK for C++ Examples Build Script"
    echo "===================================="
    echo "Usage: $0 [options]"
    echo ""
    echo "Build options:"
    echo "  --debug               Build in debug mode"
    echo "  --release             Build in release mode (default)"
    echo "  --clean               Clean build directory before building"
    echo "  --jobs N              Use N parallel jobs for building (default: auto-detect)"
    echo ""
    echo "Installation options:"
    echo "  --install             Install after building"
    echo "  --prefix PATH         Installation prefix (default: /usr/local)"
    echo ""
    echo "Testing options:"
    echo "  --test                Build and run tests"
    echo ""
    echo "Documentation options:"
    echo "  --docs                Build documentation"
    echo ""
    echo "Packaging options:"
    echo "  --package             Create binary packages after building"
    echo "  --deb                 Create Debian package (.deb)"
    echo "  --rpm                 Create RPM package (.rpm)"
    echo ""
    echo "General options:"
    echo "  --help                Show this help message"
    echo ""
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
  case $1 in
    --debug)
      BUILD_TYPE="Debug"
      shift
      ;;
    --release)
      BUILD_TYPE="Release"
      shift
      ;;
    --clean)
      DO_CLEAN=1
      shift
      ;;
    --install)
      DO_INSTALL=1
      shift
      ;;
    --prefix)
      INSTALL_PREFIX="$2"
      shift
      shift
      ;;
    --jobs)
      JOBS="$2"
      shift
      shift
      ;;
    --test)
      DO_TEST=1
      shift
      ;;
    --docs)
      DO_DOCS=1
      shift
      ;;
    --package)
      DO_PACKAGE=1
      shift
      ;;
    --deb)
      DO_PACKAGE=1
      PACKAGE_TYPE="DEB"
      shift
      ;;
    --rpm)
      DO_PACKAGE=1
      PACKAGE_TYPE="RPM"
      shift
      ;;
    --help)
      show_usage
      exit 0
      ;;
    *)
      echo "Error: Unknown option: $1"
      echo "Use --help for usage information"
      exit 1
      ;;
  esac
done

echo "AWS SDK for C++ Examples Build Script"
echo "===================================="
echo "Build type: $BUILD_TYPE"
echo "Jobs: $JOBS"
if [ $DO_CLEAN -eq 1 ]; then
    echo "Clean build: Yes"
fi
if [ $DO_INSTALL -eq 1 ]; then
    echo "Install: Yes (prefix: $INSTALL_PREFIX)"
fi
if [ $DO_TEST -eq 1 ]; then
    echo "Run tests: Yes"
fi
if [ $DO_DOCS -eq 1 ]; then
    echo "Build docs: Yes"
fi
if [ $DO_PACKAGE -eq 1 ]; then
    echo "Create package: Yes"
    if [ ! -z "$PACKAGE_TYPE" ]; then
        echo "Package type: $PACKAGE_TYPE"
    fi
fi
echo ""

# Check if cmake is installed
if ! command -v cmake &> /dev/null; then
    echo "Error: cmake is not installed or not in PATH"
    echo "Please install cmake and try again"
    exit 1
fi

# Clean build directory if requested
if [ $DO_CLEAN -eq 1 ] && [ -d "build" ]; then
    echo "Cleaning build directory..."
    rm -rf build
fi

# Create and enter the build directory
mkdir -p build
cd build

# Prepare CMake arguments
CMAKE_ARGS="-DCMAKE_BUILD_TYPE=$BUILD_TYPE"
if [ $DO_INSTALL -eq 1 ]; then
    CMAKE_ARGS="$CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX"
fi

if [ $DO_TEST -eq 0 ]; then
    CMAKE_ARGS="$CMAKE_ARGS -DBUILD_TESTS=OFF"
fi

if [ $DO_DOCS -eq 1 ]; then
    CMAKE_ARGS="$CMAKE_ARGS -DBUILD_DOCS=ON"
fi

# Generate build files
echo "Generating build files..."
cmake .. $CMAKE_ARGS $CMAKE_EXTRA_ARGS

if [ $? -ne 0 ]; then
    echo "CMake configuration failed. Please check the error messages above."
    exit 1
fi

# Build the project
echo "Building project..."
cmake --build . -- -j$JOBS

# Check if build was successful
if [ $? -ne 0 ]; then
    echo "Build failed. Please check the error messages above."
    exit 1
fi

echo "Build successful!"

# Run tests if requested
if [ $DO_TEST -eq 1 ]; then
    echo ""
    echo "Running tests..."
    ctest --output-on-failure
    
    if [ $? -ne 0 ]; then
        echo "Some tests failed. Please check the error messages above."
    else
        echo "All tests passed!"
    fi
fi

# Build documentation if requested
if [ $DO_DOCS -eq 1 ]; then
    echo ""
    echo "Building documentation..."
    cmake --build . --target docs
    
    if [ $? -ne 0 ]; then
        echo "Documentation generation failed. Please check the error messages above."
    else
        echo "Documentation built successfully."
    fi
fi

# Install if requested
if [ $DO_INSTALL -eq 1 ]; then
    echo ""
    echo "Installing to $INSTALL_PREFIX..."
    cmake --build . --target install
    
    if [ $? -ne 0 ]; then
        echo "Installation failed. Please check the error messages above."
        exit 1
    fi
    echo "Installation successful!"
fi

# Create packages if requested
if [ $DO_PACKAGE -eq 1 ]; then
    echo ""
    echo "Creating packages..."
    
    CPACK_ARGS=""
    if [ ! -z "$PACKAGE_TYPE" ]; then
        CPACK_ARGS="-G $PACKAGE_TYPE"
    fi
    
    cpack $CPACK_ARGS
    
    if [ $? -ne 0 ]; then
        echo "Package creation failed. Please check the error messages above."
        exit 1
    fi
    echo "Package creation successful!"
    
    # List created packages
    echo ""
    echo "Created packages:"
    ls -l *.deb *.rpm *.tar.gz *.zip 2>/dev/null
fi

# Print success message
echo ""
echo "Build process completed successfully!"
echo ""
echo "The following executables are available in bin/:"
ls -la bin/
echo ""
echo "Make sure you have set up your AWS credentials before running the examples:"
echo "  ../setup_credentials.sh"
