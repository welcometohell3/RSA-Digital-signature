#include "num.hpp"
#include "md4.hpp"
#include <random>

class RSA
{
public:
    RSA()
    {
        // Генерируем ключи при создании объекта
        generateKeys();
    }

    // Шифрование сообщения
    Num encrypt(const std::string &message)
    {
        Num m = Num::fromString(message, 26, "abcdefghijklmnopqrstuvwxyz");
        // std::cout << toString(m) << std::endl;
        Num c = m.mod_pow(e, n);
        // std::cout << toString(c) << std::endl;
        return c;
    }

    // Расшифрование сообщения
    Num decrypt(Num c)
    {
        Num m = c.mod_pow(d, n);
        return m;
    }

    std::string toString(const Num &num, uint64_t base = 10, const char *alphabet = "0123456789abcdefghijklmnopqrstuvwxyz")
    {
        std::vector<char> text;

        num.print(text, base, alphabet);
        text.pop_back();
        // std::cout << text.size() << std::endl;
        return std::string(text.begin(), text.end());
    }

    // Постановка подписи
    Num sign(const std::string &message)
    {
        MD4 md4;
        std::string hash = md4.hash(message);
        // std::cout << hash << std::endl;
        Num m = Num::fromString(hash, 16, "0123456789abcdef");
        // std::cout << m << std::endl;
        // std::cout << h << std::endl;
        Num s = m.mod_pow(d, n);
        // std::string hash = md4.hash(toString(s, 37));

        return s;
    }

    // Проверка подписи
    bool verify(const std::string &m, const Num &s)
    {
        MD4 md4;
        // std::string strSig = md4.hash(signature);
        // Num s = Num::fromString(strSig, 37);
        // Num m1 = s.mod_pow(e, n);
        // std::string m11 = toString(m1, 37);
        Num m1 = s.mod_pow(e, n);
        std::string m11 = toString(m1, 16, "0123456789abcdef");
        std::string hash = md4.hash(m);
        return hash == m11;
    }

    void printVector(const std::vector<char> &vec)
    {
        for (char ch : vec)
        {
            std::cout << ch;
        }
        std::cout << std::endl;
    }
    std::string vectorCharToString(const std::vector<char> &vec)
    {
        return std::string(vec.begin(), vec.end());
    }

private:
    Num n, e, d; // Ключи RSA (открытый и секретный)

    std::string generateRandomPrime(size_t numBits)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> distribution(1ULL << (numBits - 1), (1ULL << numBits) - 1);

        size_t randomNumber;
        while (true)
        {
            randomNumber = distribution(gen);
            if (randomNumber % 2 == 0)
                ++randomNumber;
            if (isPrime(randomNumber))
                break;
        }

        std::ostringstream oss;
        oss << randomNumber;
        return oss.str();
    }

    bool isPrime(size_t num)
    {
        if (num <= 1)
            return false;
        if (num <= 3)
            return true;
        if (num % 2 == 0 || num % 3 == 0)
            return false;
        for (size_t i = 5; i * i <= num; i += 6)
        {
            if (num % i == 0 || num % (i + 2) == 0)
                return false;
        }
        return true;
    }

    // Алгоритм создания открытого и секретного ключей
    void generateKeys()
    {

        // Генерация случайных простых чисел p и q
        Num p = Num::fromString(generateRandomPrime(512));
        Num q = Num::fromString(generateRandomPrime(512));
        // std::cout << p << " " << q << std::endl;

        n = p * q; // Вычисление модуля n

        Num phi = (p - 1) * (q - 1); // Вычисление функции Эйлера от n

        // Выбор открытой экспоненты e
        e = 65537;

        // Вычисление секретной экспоненты d
        d = e.modInverse(phi);
    }
};

int main()
{
    RSA rsa;

    std::string message = "helloworld";

    // Пример использования для шифрования и расшифрования
    // Num encrypted = rsa.encrypt(message);
    // Num decrypted = rsa.decrypt(encrypted);
    // std::string decryptedString = rsa.toString(decrypted, 26, "abcdefghijklmnopqrstuvwxyz");

    // std::cout << "Original message: " << message << std::endl;
    // std::cout << "Encrypted message: " << encrypted << std::endl;
    // std::cout << "Decrypted message: " << decryptedString << std::endl;

    // Пример использования для подписи и проверки подписи
    Num signature = rsa.sign(message);
    // std::cout << signature << std::endl;
    // std::string sigString = rsa.toString(signature);
    // std::cout << sigString << std::endl;
    bool verified = rsa.verify(message, signature);

    std::cout << "Signature: " << signature << std::endl;
    std::cout << "Signature verified: " << (verified ? "true" : "false") << std::endl;

    return 0;
}
