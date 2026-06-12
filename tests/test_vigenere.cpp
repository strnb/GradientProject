#include <gtest/gtest.h>
#include <string>
#include "../shared/vigenere.h"

// ─── vigenereEncrypt ──────────────────────────────────────────────────────────

TEST(VigenereEncrypt, BasicEncryption) {
    // Classic example: "ATTACKATDAWN" key "LEMON"
    std::string result = vigenereEncrypt("ATTACKATDAWN", "LEMON");
    EXPECT_EQ(result, "LXFOPVEFRNHR");
}

TEST(VigenereEncrypt, EmptyKeyReturnsEmpty) {
    EXPECT_EQ(vigenereEncrypt("HELLO", ""), "");
}

TEST(VigenereEncrypt, EmptyTextReturnsEmpty) {
    EXPECT_EQ(vigenereEncrypt("", "KEY"), "");
}

TEST(VigenereEncrypt, NonAlphaPassThrough) {
    std::string result = vigenereEncrypt("HE LLO!", "KEY");
    // Пробелы и символы должны сохраняться
    EXPECT_EQ(result[2], ' ');
    EXPECT_EQ(result[6], '!');
}

TEST(VigenereEncrypt, LowercaseInputIsUppercased) {
    std::string upper = vigenereEncrypt("HELLO", "KEY");
    std::string lower = vigenereEncrypt("hello", "KEY");
    EXPECT_EQ(upper, lower);
}

TEST(VigenereEncrypt, LowercaseKeyWorks) {
    std::string upper_key = vigenereEncrypt("HELLO", "KEY");
    std::string lower_key = vigenereEncrypt("HELLO", "key");
    EXPECT_EQ(upper_key, lower_key);
}

TEST(VigenereEncrypt, KeyWrapsAround) {
    // Ключ из одной буквы 'A' → shift=0 → текст не меняется
    std::string result = vigenereEncrypt("HELLO", "A");
    EXPECT_EQ(result, "HELLO");
}

TEST(VigenereEncrypt, WrapAroundAlphabet) {
    // 'Z' + shift 1 ('B') = 'A'
    std::string result = vigenereEncrypt("Z", "B");
    EXPECT_EQ(result, "A");
}

// ─── vigenereDecrypt ──────────────────────────────────────────────────────────

TEST(VigenereDecrypt, BasicDecryption) {
    std::string result = vigenereDecrypt("LXFOPVEFRNHR", "LEMON");
    EXPECT_EQ(result, "ATTACKATDAWN");
}

TEST(VigenereDecrypt, EmptyKeyReturnsEmpty) {
    EXPECT_EQ(vigenereDecrypt("HELLO", ""), "");
}

TEST(VigenereDecrypt, EmptyTextReturnsEmpty) {
    EXPECT_EQ(vigenereDecrypt("", "KEY"), "");
}

TEST(VigenereDecrypt, NonAlphaPassThrough) {
    std::string result = vigenereDecrypt("HE LLO!", "KEY");
    EXPECT_EQ(result[2], ' ');
    EXPECT_EQ(result[6], '!');
}

TEST(VigenereDecrypt, WrapAroundAlphabet) {
    // Decrypt 'A' with key 'B': 'A' - 1 + 26 mod 26 = 'Z'
    std::string result = vigenereDecrypt("A", "B");
    EXPECT_EQ(result, "Z");
}

// ─── Round-trip ───────────────────────────────────────────────────────────────

TEST(VigenereRoundTrip, EncryptThenDecrypt) {
    std::string original = "HELLOWORLD";
    std::string key = "SECRET";
    std::string encrypted = vigenereEncrypt(original, key);
    std::string decrypted = vigenereDecrypt(encrypted, key);
    EXPECT_EQ(decrypted, original);
}

TEST(VigenereRoundTrip, LongText) {
    std::string original = "THEQUICKBROWNFOXJUMPSOVERTHELAZYDOG";
    std::string key = "CRYPTOGRAPHY";
    std::string decrypted = vigenereDecrypt(vigenereEncrypt(original, key), key);
    EXPECT_EQ(decrypted, original);
}

TEST(VigenereRoundTrip, TextWithSpacesAndPunctuation) {
    std::string original = "HELLO WORLD!";
    std::string key = "KEY";
    std::string decrypted = vigenereDecrypt(vigenereEncrypt(original, key), key);
    EXPECT_EQ(decrypted, original);
}

TEST(VigenereRoundTrip, SingleCharacterKey) {
    std::string original = "TESTMESSAGE";
    std::string decrypted = vigenereDecrypt(vigenereEncrypt(original, "M"), "M");
    EXPECT_EQ(decrypted, original);
}
