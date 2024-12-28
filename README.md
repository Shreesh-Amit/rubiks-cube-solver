# Rubik's Cube Simulator

## Project Description
This project is a Rubik's Cube simulator that integrates real-world cube capture and solving algorithms. It combines OpenCV for camera interactions, Python's Kociemba library for solving, and C++ for visualization and UI.

## Features
- **Interactive GUI**: Color and cube face visualizations.
- **Camera Integration**: Captures Rubik's Cube face colors.
- **Automatic Solver**: Utilizes the Kociemba algorithm for optimal solutions.
- **Preview/Solution Display**: Shows the cube state and solving steps.

## Installation

### Dependencies
- **C++ Libraries**: OpenCV, nlohmann/json
- **Python Modules**: Kociemba,JSON
- **Build Tools**: CMake

### Steps
1. Clone the repository:
   ```bash
   git clone https://github.com/apt-shreesh/rubiks-cube-solver.git
   cd rubiks-cube-solver
   ```
   
2. Install Python dependencies:
   ```bash
   pip install kociemba
   ```
   
3. Install OpenCV and nlohmann/json:
   Follow platform-specific instructions for [OpenCV](https://opencv.org/) and [nlohmann/json](https://github.com/nlohmann/json).

4. Install cmake
On Linux:
   ```bash
   sudo apt update
   sudo apt install cmake
   ```
   
On macOS:
   ```bash
   bashbrew install cmake
   ```

On Windows:
  Download the installer from CMake's official website.
  Run the installer and add CMake to your system PATH during installation.

## Compilation
To compile `main.cpp`,use:
  ```
  mkdir build && cd build
  cmake ..
  make
  ```

## Running the project
   ```
   ./solver
   ```

## Usage
- **Capture Cube Faces**: Align the cube and capture face colors using the camera.
- **Interactive Preview**: Adjust cube colors manually when colour detection is not right.
- **Generate Solutions**: Once all faces are captured the solve button appears, clicking on it the solution is displayed on the preview page.

## Contributing
Contributions are welcome. Open issues or submit pull requests with improvements or bug fixes.

## License
This project is licensed under the MIT License. See the LICENSE file for details.


