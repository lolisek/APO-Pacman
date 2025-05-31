# Pacman for MZ_APO Board

## Overview
This project implements a Pacman game designed for the MicroZed-based MZ_APO board. The game features classic Pacman gameplay, including ghosts, pellets, power-ups, and a scoreboard. It is optimized for the MZ_APO hardware, utilizing its peripherals such as the LCD display, knobs, and LEDs.

## Features
- Classic Pacman gameplay with:
  - Ghosts and unique behavior.
  - Pellets and power pellets.
  - Frightened mode for ghosts.
- Interactive main menu.
- Scoreboard to track high scores.
- Hardware integration:
  - LCD display for game rendering.
  - Knobs for user input.
  - LEDs for visual feedback.

## Hardware Requirements
- MicroZed-based MZ_APO board.
- Connected LCD display and knobs.

## Software Requirements
- Cross-compiler for ARM (e.g., `arm-linux-gnueabihf-gcc`).
- Required libraries for the MZ_APO board.
- Make utility.

## Setup Instructions
1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd APO-Pacman
   ```
2. Move to the `/Pacman` folder
3. Build the project:
   ```bash
   make
   ```
4. Make sure that you connected the MZ_APO board to the network and the power source
5. Calculate the ip address of the device from the display
6. Run:
    ```bash
    make run TARGET_IP=<IP-OF-DEVICE>
    ```

## Controls
- Use the red knob to navigate the menu and control Pacman.
- Press the red knob to select menu options or confirm actions.
- Use blue knob for exiting

## File Structure
- `src/`: Contains the source code for the game.
- `include/`: Header files for the project.
- `resources/`: Game assets such as images and level files.

## License
This project is licensed under the MIT License. See the `LICENSE` file for details.

## Acknowledgments
- Designed for the MZ_APO board by Petr Porazil at PiKRON.
- Inspired by the classic Pacman game.

## Contributor
- Veronica Marie Ohrazdová
- Duc Minh Nguyen