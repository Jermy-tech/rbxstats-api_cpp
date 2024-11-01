# RbxStats C++ API Client

A C++ client library for interacting with the [RbxStats API](https://api.rbxstats.xyz). This library provides a convenient interface to access various endpoints of the RbxStats API, including offsets, exploits, and game information.

## Features

- Access all Roblox offsets and their details.
- Retrieve information about current and past exploits.
- Get the latest and future versions of Roblox.
- Fetch game details by ID.

## Requirements

- C++11 or later
- [libcurl](https://curl.se/libcurl/)
- [nlohmann/json](https://github.com/nlohmann/json) for JSON handling

## Installation

### For Windows

1. **Install `libcurl`**:
   - Download the latest version of `libcurl` from [curl.se](https://curl.se/download.html) and follow the instructions for building and linking it with your project.
   - You can also use a package manager like [vcpkg](https://github.com/microsoft/vcpkg):
     ```bash
     git clone https://github.com/microsoft/vcpkg.git
     .\vcpkg\bootstrap-vcpkg.bat
     .\vcpkg\vcpkg install curl
     ```

2. **Install `nlohmann/json`**:
   - You can add the single-header version of `nlohmann/json` to your project, or install it via vcpkg:
     ```bash
     .\vcpkg\vcpkg install nlohmann-json
     ```

### For Linux/Mac

1. **Install `libcurl`**:
   - For Ubuntu/Debian:
     ```bash
     sudo apt-get install libcurl4-openssl-dev
     ```
   - For macOS:
     ```bash
     brew install curl
     ```

2. **Install `nlohmann/json`**:
   You can add the single-header version of `nlohmann/json` to your project, or install it via your package manager:
   - For Ubuntu:
     ```bash
     sudo apt-get install nlohmann-json-dev
     ```
   - For macOS:
     ```bash
     brew install nlohmann-json
     ```

## Usage

To use the RbxStats C++ API Client, include the `rbxstats_api.h` header file in your project and link against the required libraries.

### Example

Here is a simple example of how to use the RbxStats API client to fetch all offsets:

```cpp
#include "rbxstats_api.h"
#include <iostream>

int main() {
    RbxStatsClient client("YOUR_API_KEY");

    try {
        json offsets = client.offsets.get_all();
        std::cout << "Offsets: " << offsets.dump(4) << std::endl;

        std::string plain_offsets = client.offsets.get_all_plain();
        std::cout << "Plain Offsets: " << plain_offsets << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
```

### API Endpoints

#### Offsets

- `get_all()`: Fetch all Roblox offsets in JSON format.
- `get_all_plain()`: Fetch all Roblox offsets in plain text format.
- `get_offset_by_name(const std::string &name)`: Get specific offset by name.
- `get_offsets_by_prefix(const std::string &prefix)`: Fetch all offsets with a specific prefix.
- `get_camera()`: Fetch all camera offsets.

#### Exploits

- `get_all()`: Fetch all current working exploits.
- `get_windows()`: Fetch all current Windows exploits.
- `get_mac()`: Fetch all current Mac exploits.
- `get_undetected()`: Fetch all currently undetected exploits.

#### Versions

- `get_latest()`: Get the latest Roblox version.
- `get_future()`: Get the future Roblox version.

#### Game

- `get_game_by_id(int game_id)`: Get game information by game ID.
- `get_game_by_id_plain(int game_id)`: Get game information in plain text.

## Error Handling

If an error occurs during a request, the library throws a `std::runtime_error` with an appropriate error message. Make sure to wrap your calls in a try-catch block to handle exceptions properly.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please feel free to submit a pull request or open an issue for discussion.

```

### Notes
- Replace `"YOUR_API_KEY"` with the actual API key when running examples.
- Update the contact email and any other relevant information as needed.
- Add more details to the "Contributing" section if necessary, depending on how you want to handle contributions.
