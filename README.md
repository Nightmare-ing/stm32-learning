# STM32 Learning

This repository is dedicated to learning and experimenting with STM32 microcontrollers along side ATK's STM32 lectures.
However, this repository doesn't use Keil IDE or STM32CubeIDE, instead it uses arm-none-eabi-gcc toolchain and modern CMake build system to build different targets for each lab.  
The logic for each device is abstracted into library `components`, which is independent of the platform and can be reused in other projects. To use those components, in `bsp` we only need to implement interfaces defined in each component's header file, thus separate IO logic from the application logic.  
Then we can test those components, i.e. code logic in a host environment, and use them in embedded environment without any modification. After flashing firmware to STM32 devices, we only need to focus hardware IO status.  
Those tests are driven by GitHub Actions automatically.

This repo is create from [this template](https://github.com/Nightmare-ing/stm32-template.git), and there's description in README for the repo structure.

## Tests

![CI Status](https://github.com/Nightmare-ing/stm32-learning/actions/workflows/ci.yml/badge.svg)

## Labs

### Lab 2: Button to LED

In this lab, we use one button to control one LED. When the button is pressed, the LED will turn on, and when the button is pressed again, the LED will turn off.
We use state machine to debounce the button, instead of using delay.
