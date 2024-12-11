
# Compression Algorithms for Genomic Data




## Description
This project focuses on developing and comparing optimized compression algorithms for genomic data. The implemented algorithms, including Huffman Encoding and Run-Length Encoding (RLE), are designed to efficiently handle genomic sequences composed of nucleotide bases ('A', 'C', 'T', 'G'). This project aims to provide a robust tool for compressing genomic data, making it suitable for storage and analysis in the medical informatics and biotechnological field.
# Required Technology

1. **C++ Compiler**
   - Any one of the following:
       - `g++` (GNU C++ Compiler)
       - `clang++` (LLVM C++ Compiler)
       - `cl` (MSVC Compiler for Windows)
  

2. **CMake**
   - **Minimum version**: 3.10 

3. **Build Tool**
   - Any one of the following:
       - `make`
       - `mingw32-make`
       - `ninja`

4. **Operating System-Specific Tools**
   - **Unix-like systems** (Linux/Mac):
       - `make` or `ninja`
   - **Windows**:
       - `mingw32-make` or `ninja`
       - Ensure executables can run on the platform (`.exe` extensions).

5. **Permissions for Installation**
   - Write access to `/usr/local/bin` (on Unix-like systems) or an equivalent global binary directory.
   - Write access to the path details on Windows systems

### Additional Notes
- **PATH Configuration**:
  If the script cannot copy the `compressor` executable to the installation directory (`/usr/local/bin`), you need to manually execute the provided command with `sudo`. If this is the case, the proper error message will appear.

- **Testing Environment**:
  Ensure the `./tests` executable can run successfully after setup.


# Setup
1. **Clone this repository** in and open in **Visual Studio Code**:
   ```bash
   git clone https://github.com/savinadd/Algorithms-for-Genomic-Compression
2. **Run the installation script** in a terminal within Visual Studio Code
    ```bash
    ./install.sh
3. **Successful** setup is indicated by the following message at the end of the installation:
```bash
Setup complete! The project is ready to use.
To run the main program, use './compressor.exe' from the project root or 'compressor' if added to PATH.

```

If there is any missing technology on the computer attempting to run the compressor that is necessary, there will be a message detailing exactly what is missing and the installation script will exit. Please install all the necessary tools. 
# Usage

## File formats

This tool supports only ```.txt ``` files. However, it does support the use of ```.fasta ``` files by way of a converter to a ```.txt ``` file. To use it, run:
```bash
./clean_fasta.sh
```
You will be prompted to enter in the path to the file. The tool supports UNIX-style paths as well as Windows paths. The converted file will be written to a ```.txt ```in the root of the Optimized-Algorithms-for-Genomic-Compression folder. The output file name can be chosen or a default will be chosen. Here is an example: 

```bash
MINGW64 ~/Desktop/Optimized-Algorithms-for-Genomic-Compression (main)
$ ./clean_fasta.sh
Enter the path to the input FASTA file: C:\Users\Savina\Downloads\sequence.fasta
Enter the path for the output file (default: cleaned_sequence.txt): example.txt
Cleaned sequence written to 'example.txt'
```
## Using the Compressor

The compressor has a help menu. To access this help, run:

 ```compressor -h ```
 or```compressor -help ```

It has the necessary information to get started with the compressor. 


## Example
In the data directory within the project, there are test data files. If we want to compress the file named  ```datatest.txt``` and output the compressed file to a file called ```output.bin```, for example, using the HuffmanGenome compressor version, we can run: 
```bash
compressor -c -i data/datatest.txt -o output.bin -m huffmangenome
```
To decompress the file created by this and save the decompressed output to a file called ```output2.txt```, we run:
```bash
compressor -d -i output.bin  -o output2.txt -m huffmangenome
```

