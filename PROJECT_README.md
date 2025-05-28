# AWS SDK for C++ - Professional Examples Project

This project provides a comprehensive demonstration of the AWS SDK for C++ with a professional-grade CMake project structure, properly organized source code, tests, documentation, and packaging support.

> **Note:** This project was restructured from a simpler examples codebase into a professional-grade CMake project with proper organization. The original example files in the root directory have been refactored into a proper library structure with organized applications.

## Project Structure

The project follows modern C++ project organization practices:

```
.
├── cmake/              # CMake modules and configuration files
│   ├── cmake_uninstall.cmake.in    # Template for uninstall target
│   ├── Doxyfile.in                 # Template for Doxygen configuration
│   ├── FindDoxygen.cmake           # Custom Doxygen finder
│   └── PackageConfig.cmake         # Packaging configuration
├── doc/                # Documentation files
├── include/            # Public headers
│   └── awsexamples/    # Project headers
│       ├── AwsUtils.h             # AWS utilities and initialization
│       ├── S3Manager.h            # S3 service management
│       ├── DynamoDBManager.h      # DynamoDB service management
│       └── EC2Manager.h           # EC2 service management
├── src/                # Source code
│   ├── app/            # Applications
│   │   ├── CMakeLists.txt         # Application build configuration
│   │   ├── S3Example.cpp          # S3 example application
│   │   ├── DynamoDBExample.cpp    # DynamoDB example application
│   │   └── EC2Example.cpp         # EC2 example application
│   └── lib/            # Library implementations
│       ├── CMakeLists.txt         # Library build configuration
│       ├── AwsUtils.cpp           # AWS utilities implementation
│       ├── S3Manager.cpp          # S3 manager implementation
│       ├── DynamoDBManager.cpp    # DynamoDB manager implementation
│       └── EC2Manager.cpp         # EC2 manager implementation
├── test/               # Test files
│   ├── CMakeLists.txt            # Test build configuration
│   └── S3ManagerTest.cpp         # S3 manager unit tests
├── scripts/            # Scripts for build automation, etc.
├── build/              # Build directory (generated)
├── .clang-format       # Code formatting configuration
├── .clang-tidy         # Linting configuration
├── build.sh            # Build automation script
├── setup_credentials.sh # AWS credentials setup script
├── CMakeLists.txt      # Main CMake configuration
├── LICENSE             # License file (Apache 2.0)
├── PROJECT_README.md   # This file
└── README.md           # Original AWS SDK guide

## Key Features

- **Modular architecture** with proper separation of concerns
- **Professional CMake setup** with modern practices
- **Library and applications** structure for code reuse
- **Unit tests** for core components
- **Doxygen documentation** support
- **Code formatting** with clang-format
- **Linting** with clang-tidy
- **Package generation** for DEB and RPM via CPack
- **Comprehensive AWS service examples** including S3, DynamoDB, and EC2

## Prerequisites

To build and run this project, you need:

1. C++ development environment with a modern compiler (GCC 8+, Clang 6+, or MSVC 2019+)
2. CMake (version 3.14 or higher)
3. AWS SDK for C++ installed
4. Doxygen (optional, for documentation)
5. clang-format (optional, for code formatting)
6. clang-tidy (optional, for linting)
7. RPM or DEB tools (optional, for packaging)

### Installing the AWS SDK for C++

#### Using vcpkg (Recommended)
```bash
# Install vcpkg if not already installed
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh  # Linux/macOS
# or
./bootstrap-vcpkg.bat  # Windows

# Install AWS SDK
./vcpkg install aws-sdk-cpp[core,s3,ec2,dynamodb]
```

#### Using CMake
```bash
git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp
cd aws-sdk-cpp
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_ONLY="core;s3;ec2;dynamodb"
make -j$(nproc)
sudo make install
```

## Building the Examples

### Using the Build Script

The easiest way to build is to use the provided script, which supports many options:

```bash
# Make the script executable
chmod +x build.sh

# Show all build options
./build.sh --help

# Basic build (release mode)
./build.sh

# Debug build
./build.sh --debug

# Build with tests
./build.sh --test

# Build with documentation
./build.sh --docs

# Build Debian package
./build.sh --deb

# Build RPM package
./build.sh --rpm

# Build and install
./build.sh --install --prefix=/usr/local
```

### Manual Build

If you prefer to build manually:

```bash
# Create and enter build directory
mkdir -p build && cd build

# Configure with CMake
cmake ..

# Build
cmake --build . -- -j$(nproc)

# Run tests (optional)
ctest

# Install (optional)
cmake --build . --target install

# Generate packages (optional)
cpack
```

### Build Output

After a successful build:

- Executables are placed in the `bin/` directory
- Libraries are placed in the `lib/` directory
- Documentation (if built) is in the `docs/` directory

## Setting Up AWS Credentials

Before running the examples, you need to configure your AWS credentials. Use the provided script:

```bash
# Make the script executable
chmod +x setup_credentials.sh

# Run the script
./setup_credentials.sh
```

Alternatively, you can manually create `~/.aws/credentials` and `~/.aws/config` files:

**~/.aws/credentials**
```
[default]
aws_access_key_id = YOUR_ACCESS_KEY
aws_secret_access_key = YOUR_SECRET_KEY
```

**~/.aws/config**
```
[default]
region = us-west-2
output = json
```

## Running the Examples

After building, you can run the examples:

```bash
# Navigate to the build directory
cd build/bin

# Run the S3 example
./s3-example

# Run the DynamoDB example
./dynamodb-example

# Run the EC2 example
./ec2-example

# Original examples:
./aws-example             # Main example
./s3-example              # S3 operations example
./dynamodb-example        # DynamoDB operations example
./ec2-example             # EC2 operations example

# Library-based examples:
./s3-manager-example      # S3 operations using our library
./dynamodb-manager-example # DynamoDB operations using our library
./ec2-manager-example     # EC2 operations using our library
```

## Using the Library in Your Own Projects

This project provides a reusable C++ library for AWS operations that you can use in your own projects:

```cpp
#include "awsexamples/AwsUtils.h"
#include "awsexamples/S3Manager.h"
#include "awsexamples/DynamoDBManager.h"
#include "awsexamples/EC2Manager.h"

int main() {
    // Initialize AWS SDK
    awsexamples::utils::AwsApiInitializer awsInit;
    
    // Create an S3 manager and use it
    awsexamples::S3Manager s3;
    s3.ListBuckets();
    
    // Create a DynamoDB manager and use it
    awsexamples::DynamoDBManager dynamodb;
    dynamodb.CreateTable("MyTable");
    
    // Create an EC2 manager and use it
    awsexamples::EC2Manager ec2;
    ec2.ListInstances();
    
    return 0;
}
```

## Project Components

### Libraries

- **awsexamples**: The main library with classes for AWS service operations

### Applications

- **Original Examples**: Direct AWS SDK usage examples from the guide
- **Library-Based Examples**: Examples showing how to use our abstraction library

### Documentation

Documentation is generated using Doxygen and includes:
- Class and function documentation
- Library usage examples
- Implementation details

To build documentation:
```bash
./build.sh --docs
```

Then open `build/docs/awsexamples/html/index.html` in your browser.

### Testing

The project includes unit tests for the library components:
```bash
./build.sh --test
```

## Example Descriptions

### Main Example (`aws-example`)
Simple S3 operations: list buckets, create bucket, upload file.

### S3 Example (`s3-example`)
Comprehensive S3 operations:
- List buckets
- Create and delete buckets
- Upload and download files
- Upload text content
- List objects in a bucket
- Delete objects

### DynamoDB Example (`dynamodb-example`)
DynamoDB operations:
- Create table
- Add items
- Get item by key
- Scan table for all items

### EC2 Example (`ec2-example`)
EC2 operations:
- List EC2 instances
- Start/stop instances
- Launch new instances
- Terminate instances

## Notes and Best Practices

1. **Always initialize and shutdown the SDK properly**
   ```cpp
   Aws::SDKOptions options;
   Aws::InitAPI(options);
   {
       // AWS operations here
   }
   Aws::ShutdownAPI(options);
   ```

2. **Handle errors appropriately**
   ```cpp
   auto outcome = s3_client.SomeOperation(request);
   if (outcome.IsSuccess()) {
       // Handle success
   } else {
       // Handle error
       auto error = outcome.GetError();
       std::cerr << "Error: " << error.GetMessage() << std::endl;
   }
   ```

3. **Use IAM roles instead of hardcoded credentials when possible**

4. **Be aware of AWS service charges**
   The examples in this project create and use real AWS resources that may incur charges to your AWS account.

5. **Clean up resources**
   The examples try to clean up resources (like S3 buckets and EC2 instances) but always verify that you don't have unwanted resources running.

## Packaging

The project supports creating distributable packages:

### DEB Packages (Debian, Ubuntu)

```bash
./build.sh --deb
```

The generated `.deb` file will be in the `build/` directory.

### RPM Packages (RHEL, CentOS, Fedora)

```bash
./build.sh --rpm
```

The generated `.rpm` file will be in the `build/` directory.

### Source Packages

```bash
./build.sh --package
```

## Development Guidelines

### Code Formatting

The project uses clang-format for consistent code style. The configuration is in `.clang-format`.

To format your code:

```bash
find include src -name '*.h' -o -name '*.cpp' | xargs clang-format -i
```

### Linting

The project uses clang-tidy for static analysis. The configuration is in `.clang-tidy`.

To run the linter:

```bash
find include src -name '*.h' -o -name '*.cpp' | xargs clang-tidy -p build
```

### Adding a New Feature

1. Create header files in `include/awsexamples/`
2. Implement classes in `src/lib/`
3. Add unit tests in `test/`
4. Update CMake files as needed
5. Add examples to `src/app/`

### Versioning

The project uses semantic versioning (MAJOR.MINOR.PATCH):
- Increment MAJOR for incompatible API changes
- Increment MINOR for backwards-compatible functionality
- Increment PATCH for backwards-compatible bug fixes

To change the version, edit the `project()` call in the main `CMakeLists.txt`.

## Additional Resources

- [AWS SDK for C++ Documentation](https://docs.aws.amazon.com/sdk-for-cpp/latest/developer-guide/welcome.html)
- [AWS SDK for C++ API Reference](https://sdk.amazonaws.com/cpp/api/LATEST/index.html)
- [AWS SDK for C++ GitHub Repository](https://github.com/aws/aws-sdk-cpp)
- [CMake Documentation](https://cmake.org/documentation/)
- [Doxygen Manual](https://www.doxygen.nl/manual/)

## License

This project is licensed under the Apache License 2.0.
