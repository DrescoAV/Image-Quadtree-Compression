# Quadtree Image Compression

## Author: Andrei-Valerian Andreescu

## Table of Contents

- [Quadtree Image Compression](#quadtree-image-compression)
  - [Author: Andrei-Valerian Andreescu](#author-andrei-valerian-andreescu)
  - [Table of Contents](#table-of-contents)
  - [Description](#description)
  - [Features](#features)
  - [Limitations](#limitations)
  - [Example](#example)
  - [Demo](#demo)
  - [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Installation](#installation)
    - [Usage](#usage)
    - [How to run the checker](#how-to-run-the-checker)
  - [Contributing](#contributing)
  - [License](#license)

## Description

Quadtree Image Compression is a C program that enables you to perform image compression and decompression using a quadtree data structure. This program can take a PPM image as input, compress it using quadtree-based techniques, and store the compressed data or decompress it back to an image. It utilizes a quadtree data structure to divide the image into smaller squares and compresses regions with similar colors, resulting in efficient storage.

## Features

- Image compression using a quadtree structure.
- Decompression of compressed images.
- Statistics generation for compressed images.
- Supports PPM image format.
- Customizable compression factor.

## Limitations

- The PPM image must be either 2x2, 4x4, 8x8, 16x16, 32x32, 64x64, 128x128, 256x256, 512x512, 1024x1024, etc.

## Example

![Exeample explained]()
![Compression factor]()

## Demo

![Quadtree Compression](link)

## Getting Started

Follow these steps to get started with Quadtree Image Compression.

### Prerequisites

Before you begin, make sure you have the following prerequisites:

- A C compiler (e.g., GCC)
- PPM format images for compression
- A Linux distribution to run the auto checker feature.

### Installation

1. Clone this repository to your local machine:

   ```shell
   git clone sa pun linku
   ```

2. Compile the program by typing "make" into your terminal.

### Usage

- Showing in output file statistics about the quadtree the image creates: ./quadtree -c1 compression_factor input_file output_file

- Compressing image to output file: ./quadtree -c2 compression_factor input_file output_file

- Decompressing image to output file: ./quadtree -d input_file output_file

### How to run the checker

```bash
./check.sh
```

## Contributing

Contributions to the Quadtree Compression project are welcome! If you'd like to contribute, please follow these guidelines:

1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Make your changes and ensure they pass any existing tests.
4. Create a pull request with a clear description of your changes.

We appreciate your contributions to make this project better.

## License

This project is licensed under the MIT License. See the [LICENSE](https://github.com/DrescoAV/Image-Quadtree-Compression/blob/main/LICENSE) file for details.
