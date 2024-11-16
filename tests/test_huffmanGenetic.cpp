#include <gtest/gtest.h>
#include "../include/HuffmanGenome.h"

TEST(HuffmanTest, EncodingAndDecoding) {
    HuffmanGenome huffman;
    std::string testSequence1 = "AAAAAACCCCTG";
    huffman.encode(testSequence1);
    std::string encoded1 = huffman.getEncodedSequence();
    std::string decoded1 = huffman.decode(encoded1);

    EXPECT_EQ(decoded1, testSequence1);

    std::string testSequence2 = "AAAAAAAAAA";
    huffman.encode(testSequence2);
    std::string encoded2 = huffman.getEncodedSequence();
    std::string decoded2 = huffman.decode(encoded2);

    EXPECT_EQ(decoded2, testSequence2);
}

TEST(HuffmanTest, CompressionRatio) {
    HuffmanGenome huffman;
    std::string testSequence = "AAAAAAAAAAAAAAAAAAAAAACCG";
    huffman.encode(testSequence);
    double compressionRatio = huffman.getCompressionRatio();
   double expectedRatio = 7.14; // Adjusted expected value
EXPECT_NEAR(compressionRatio, expectedRatio, 0.01); // Use tighter tolerance if possible

}

TEST(HuffmanTest, EdgeCases) {
    HuffmanGenome huffman;

    std::string emptySequence = "";
    huffman.encode(emptySequence);
    EXPECT_EQ(huffman.getEncodedSequence(), "");
    EXPECT_EQ(huffman.decode(""), "");

    std::string singleCharSequence = "AAAAA";
    huffman.encode(singleCharSequence);
    std::string encoded = huffman.getEncodedSequence();
    std::string decoded = huffman.decode(encoded);
    EXPECT_EQ(decoded, singleCharSequence);

    std::string allBasesOnce = "ACTG";
    huffman.encode(allBasesOnce);
    encoded = huffman.getEncodedSequence();
    decoded = huffman.decode(encoded);
    EXPECT_EQ(decoded, allBasesOnce);
}
