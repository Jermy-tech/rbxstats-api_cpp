# RbxStats C++ API Client

A C++ client library for interacting with the [RbxStats API](https://api.rbxstats.xyz). This library provides a convenient interface to access various endpoints of the RbxStats API, including offsets, exploits, and game information.

## Features

- Access all Roblox offsets and their details.
- Retrieve information about current and past exploits.
- Get the latest and future versions of Roblox.
- Fetch game details by ID.

## Requirements

- C++11 or later
- Windows (for WinINet)

## Installation

### For Windows

1. **Set Up Your Project**: 
   - Ensure you have a C++ development environment set up. You can use an IDE like Visual Studio.

2. **Include WinINet**: 
   - Make sure your project links against the `wininet.lib`. You can add it in your project properties under Linker -> Input -> Additional Dependencies.

## Usage

To use the RbxStats C++ API Client, include the `rbxstats_api.h` header file in your project.

### Example

Here is a simple example of how to use the RbxStats API client to fetch all offsets:

```cpp
#include "rbxstats.h"
#include <iostream>
#include <map> // Include for std::map
#include <string> // Include for std::string

int main() {
    RbxStatsClient client("YOUR_API_KEY");

    try {
        // Get all offsets
        auto offsets = client.offsets.get_all();
        std::cout << "Offsets:\n";
        for (const auto &entry : offsets) {
            std::cout << entry.first << ": " << entry.second << std::endl;
        }

        // Get game information by ID
        int game_id = 123456; // Replace with a valid game ID
        auto game_info = client.game.get_game_by_id(game_id);
        std::cout << "\nGame Info:\n";
        for (const auto &entry : game_info) {
            std::cout << entry.first << ": " << entry.second << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
```

### API Endpoints

#### Offsets

- `get_all()`: Fetch all Roblox offsets in JSON format.
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

## Error Handling

If an error occurs during a request, the library throws a `std::runtime_error` with an appropriate error message. Make sure to wrap your calls in a try-catch block to handle exceptions properly.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please feel free to submit a pull request or open an issue for discussion.
```

### Notes
- The JSON parsing part is handled in the new implementation without relying on external libraries, so you might want to ensure that the JSON parsing functionality in your code is robust.
- Replace `"YOUR_API_KEY"` with the actual API key when running examples.
- Make sure to test the example code in your development environment to ensure it works as intended with the new API client structure.
