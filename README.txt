Encoding:
  
  In the encoding part, building huffman tree structure was the key. I used the minimal heap to construct the huffman tree. More specifically, we could get like "01a1b1c" such structure. When we're encoding for every character, we could recover the huffman tree based on the structure. In addition, in the first 8 bytes of header, I store the length of encoded string in the first 4 bytes, and another 4 bytes for the length of huffman tree structure. And huffman tree structure was stored in the rest 1016 bytes. After 1024 bytes, there is the encoded binary string.

Decoding:

  In the decoding part, we need to reconstruct the huffman tree to get huffman code dictionary. Every time we read 2 bytes from file for decoding. Also, in order to reduce the memory use, if the number of encoded string is 10000, I will write this string to the file.

Searching
  In the searching part, I used BM search method to do the searching job. In addition, I define a two dimension array to store the original test, and the length of each dimension is equal to the length of search item. Then, this 2d array will be treated as a circle. Once moving forward, the previous several buckets will be refilled with new characters.