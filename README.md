# File compression using Huffman coding

After learning about Huffman coding algorithm in class, I wrote this project in C to learn how computers
compress textfiles. This program can compress upto 30% of the file depending on the contents of the textfile. It compressed a sample 100 KB text file into 67 KB. Since Huffman compression is lossless compression, this is completely safe to use without fear of data loss.

### How to build

Run `gcc hashmap.c priority.c huffman.c -Wall -Wextra -o huffman`

### Compressing a file

Run `./huffman -comp filename`

### Decompressing a file

Run `./huffman -decomp filename`
