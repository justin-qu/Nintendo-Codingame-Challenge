#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>
#include <iomanip>

using namespace std;

// solution.h
// ----------------------------------------------------------------------
template <size_t bitlen>
class GF2Polynomial
{
private:
    bitset<bitlen> bits;
    size_t degree;

    const size_t findDegree(size_t start = bitlen - 1) const;

public:
    GF2Polynomial<bitlen>() : bits(bitset<bitlen>()), degree(0) {}
    GF2Polynomial<bitlen>(const bitset<bitlen>& y) : bits(bitset<bitlen>(y)), degree(findDegree()) {}
    GF2Polynomial<bitlen>(const GF2Polynomial<bitlen>& y) : bits(bitset<bitlen>(y.bits)), degree(y.degree) {}

    GF2Polynomial<bitlen> operator+(const GF2Polynomial<bitlen>& y);
    GF2Polynomial<bitlen> operator-(const GF2Polynomial<bitlen>& y);
    GF2Polynomial<bitlen> operator*(const GF2Polynomial<bitlen>& y);
    GF2Polynomial<bitlen> operator/(const GF2Polynomial<bitlen>& y);
    GF2Polynomial<bitlen> operator%(const GF2Polynomial<bitlen>& y);
    GF2Polynomial<bitlen> operator<<(const size_t n);
    GF2Polynomial<bitlen> operator>>(const size_t n);
    GF2Polynomial<bitlen>& operator=(const GF2Polynomial<bitlen>& y);
    GF2Polynomial<bitlen>& operator+=(const GF2Polynomial<bitlen>& y);
    GF2Polynomial<bitlen>& operator-=(const GF2Polynomial<bitlen>& y);
    GF2Polynomial<bitlen>& operator*=(const GF2Polynomial<bitlen>& y);
    GF2Polynomial<bitlen>& operator/=(const GF2Polynomial<bitlen>& y);
    GF2Polynomial<bitlen>& operator%=(const GF2Polynomial<bitlen>& y);
    GF2Polynomial<bitlen>& operator<<=(const size_t n);
    GF2Polynomial<bitlen>& operator>>=(const size_t n);
    const bool operator[](const size_t index) const;

    const bool isZero() const;
    const bool isOne() const;
    GF2Polynomial<bitlen> derivative() const;
    GF2Polynomial<bitlen> squareMod(const GF2Polynomial<bitlen>& y) const;
    GF2Polynomial<bitlen> gcd(const GF2Polynomial<bitlen>& y) const;
    vector<GF2Polynomial<bitlen>> factor() const;
    vector<GF2Polynomial<bitlen>> factorDDF() const;
    vector<GF2Polynomial<bitlen>> factorEDF(const size_t factorDegree) const;
    string str(size_t numBits) const;
    void randPoly(size_t degree);
    const size_t getDegree() const;
};



template <size_t bitlen> const size_t GF2Polynomial<bitlen>::findDegree(size_t start) const
{
    for (int i = start; i > 0; i--)
    {
        if (bits[i])
        {
            return i;
        }
    }

    return 0;
}

template <size_t bitlen> GF2Polynomial<bitlen> GF2Polynomial<bitlen>::operator+(const GF2Polynomial<bitlen>& y)
{
    GF2Polynomial result = GF2Polynomial(*this);
    result.bits ^= y.bits;
    result.degree = result.findDegree(max(degree, y.degree));
    return result;
}

template <size_t bitlen> GF2Polynomial<bitlen> GF2Polynomial<bitlen>::operator-(const GF2Polynomial<bitlen>& y)
{
    GF2Polynomial result = GF2Polynomial(*this);
    result.bits ^= y.bits;
    result.degree = result.findDegree(max(degree, y.degree));
    return result;
}

template <size_t bitlen> GF2Polynomial<bitlen> GF2Polynomial<bitlen>::operator*(const GF2Polynomial<bitlen>& y)
{
    GF2Polynomial result = GF2Polynomial();

    for (int i = degree; i >= 0; i--)
    {
        result.bits <<= 1;

        if (bits[i])
        {
            result.bits ^= y.bits;
        }
    }

    if (degree >= bitlen - y.degree)
    {
        result.degree = result.findDegree();
    }
    else
    {
        result.degree = result.findDegree(degree + y.degree);
    }

    return result;
}

template <size_t bitlen> GF2Polynomial<bitlen> GF2Polynomial<bitlen>::operator/(const GF2Polynomial<bitlen>& y)
{
    GF2Polynomial result = GF2Polynomial();

    if (y.isZero())
    {
        throw runtime_error("Math Error: Attempted to divide by Zero\n");
    }

    if (y.degree > degree)
    {
        return result;
    }

    bitset numerator = bitset(bits);
    bitset denominator = bitset(y.bits);
    denominator <<= degree - y.degree;

    for (int i = degree; i >= (int)y.degree; i--)
    {
        if (numerator[i])
        {
            numerator ^= denominator;
            result.bits[i - y.degree] = 1;
        }

        denominator >>= 1;
    }

    result.degree = degree - y.degree;

    return result;
}

template <size_t bitlen> GF2Polynomial<bitlen> GF2Polynomial<bitlen>::operator%(const GF2Polynomial<bitlen>& y)
{
    GF2Polynomial result = GF2Polynomial(*this);

    if (y.isZero())
    {
        throw runtime_error("Math Error: Attempted to divide by Zero\n");
    }

    if (y.degree > degree)
    {
        return result;
    }

    bitset denominator = bitset(y.bits);
    denominator <<= degree - y.degree;

    for (int i = degree; i >= (int)y.degree; i--)
    {
        if (result.bits[i])
        {
            result.bits ^= denominator;
        }

        denominator >>= 1;
    }

    result.degree = result.findDegree(y.degree);

    return result;
}

template <size_t bitlen> GF2Polynomial<bitlen> GF2Polynomial<bitlen>::operator<<(const size_t n)
{
    GF2Polynomial result = GF2Polynomial(*this);
    result.bits <<= n;

    if (degree >= bitlen - n)
    {
        result.degree = result.findDegree();
    }
    else
    {
        result.degree = degree + n;
    }

    return result;
}

template <size_t bitlen> GF2Polynomial<bitlen> GF2Polynomial<bitlen>::operator>>(const size_t n)
{
    GF2Polynomial result = GF2Polynomial(*this);
    result.bits >>= n;

    if (n > degree)
    {
        result.degree = 0;
    }
    else
    {
        result.degree = degree - n;
    }

    return result;
}

template <size_t bitlen> GF2Polynomial<bitlen>& GF2Polynomial<bitlen>::operator=(const GF2Polynomial<bitlen>& y)
{
    bits = y.bits;
    degree = y.degree;

    return *this;
}

template <size_t bitlen> GF2Polynomial<bitlen>& GF2Polynomial<bitlen>::operator+=(const GF2Polynomial<bitlen>& y)
{
    bits ^= y.bits;
    degree = findDegree(max(degree, y.degree));

    return *this;
}

template <size_t bitlen> GF2Polynomial<bitlen>& GF2Polynomial<bitlen>::operator-=(const GF2Polynomial<bitlen>& y)
{
    bits ^= y.bits;
    degree = findDegree(max(degree, y.degree));;

    return *this;
}

template <size_t bitlen> GF2Polynomial<bitlen>& GF2Polynomial<bitlen>::operator*=(const GF2Polynomial<bitlen>& y)
{
    bitset<bitlen> result;

    for (int i = degree; i >= 0; i--)
    {
        result <<= 1;

        if (bits[i])
        {
            result ^= y.bits;
        }
    }

    bits = result;
    if (degree >= bitlen - y.degree)
    {
        degree = findDegree();
    }
    else
    {
        degree = findDegree(degree + y.degree);
    }

    return *this;
}

template <size_t bitlen> GF2Polynomial<bitlen>& GF2Polynomial<bitlen>::operator/=(const GF2Polynomial<bitlen>& y)
{
    bitset<bitlen> result;

    if (y.isZero())
    {
        throw runtime_error("Math Error: Attempted to divide by Zero\n");
    }

    if (y.degree > degree)
    {
        bits = result;
        degree = 0;
        return *this;
    }

    bitset denominator = bitset(y.bits);
    denominator <<= degree - y.degree;

    for (int i = degree; i >= (int)y.degree; i--)
    {
        if (bits[i])
        {
            bits ^= denominator;
            result[i - y.degree] = 1;
        }

        denominator >>= 1;
    }

    bits = result;
    degree = degree - y.degree;

    return *this;
}

template <size_t bitlen> GF2Polynomial<bitlen>& GF2Polynomial<bitlen>::operator%=(const GF2Polynomial<bitlen>& y)
{
    if (y.isZero())
    {
        throw runtime_error("Math Error: Attempted to divide by Zero\n");
    }

    if (y.degree > degree)
    {
        return *this;
    }

    bitset denominator = bitset(y.bits);
    denominator <<= degree - y.degree;

    for (int i = degree; i >= (int)y.degree; i--)
    {
        if (bits[i])
        {
            bits ^= denominator;
        }

        denominator >>= 1;
    }

    degree = findDegree(y.degree);

    return *this;
}

template <size_t bitlen> GF2Polynomial<bitlen>& GF2Polynomial<bitlen>::operator<<=(const size_t n)
{
    bits <<= n;

    if (degree >= bitlen - n)
    {
        degree = findDegree();
    }
    else
    {
        degree += n;
    }

    return *this;
}

template <size_t bitlen> GF2Polynomial<bitlen>& GF2Polynomial<bitlen>::operator>>=(const size_t n)
{
    bits >>= n;

    if (n > degree)
    {
        degree = 0;
    }
    else
    {
        degree -= n;
    }

    return *this;
}

template <size_t bitlen> const bool GF2Polynomial<bitlen>::operator[](const size_t index) const
{
    return bits[index];
}

template <size_t bitlen> const bool GF2Polynomial<bitlen>::isZero() const
{
    if (degree == 0 and bits[0] == 0)
    {
        return true;
    }

    return false;
}

template <size_t bitlen> const bool GF2Polynomial<bitlen>::isOne() const
{
    if (degree == 0 and bits[0] == 1)
    {
        return true;
    }

    return false;
}

template <size_t bitlen> GF2Polynomial<bitlen> GF2Polynomial<bitlen>::derivative() const
{
    GF2Polynomial result = GF2Polynomial(*this);

    if (degree == 0)
    {
        result.bits >>= 1;
        return result;
    }

    bitset<bitlen> mask;
    for (int i = 0; i <= degree; i += 2)
    {
        mask[i] = 1;
    }

    result.bits >>= 1;
    result.bits &= mask;
    result.degree = result.findDegree(degree - 1);

    return result;
}

template <size_t bitlen> GF2Polynomial<bitlen> GF2Polynomial<bitlen>::squareMod(const GF2Polynomial<bitlen>& y) const
{
    GF2Polynomial<bitlen> result;

    if (y.isZero())
    {
        throw runtime_error("Math Error: Attempted to divide by Zero\n");
    }

    if (bitlen > 2 * degree)
    {
        for (int i = 0; i <= degree; i++)
        {
            result.bits[2 * i] = bits[i];
        }

        result.degree = 2 * degree;
        result %= y;

        return result;
    }

    bitset<2 * bitlen> bigBits;

    for (int i = 0; i <= degree; i++)
    {
        bigBits[2 * i] = bits[i];
    }

    bitset denominator = bitset<2 * bitlen>(y.bits.to_string());
    denominator <<= (2 * degree) - y.degree;

    for (int i = 2 * degree; i >= (int)y.degree; i--)
    {
        if (bigBits[i])
        {
            bigBits ^= denominator;
        }

        denominator >>= 1;
    }

    bigBits <<= bitlen;
    result.bits = bitset<bitlen>(bigBits.to_string());
    result.degree = result.findDegree(y.degree);

    return result;
}

//Euclidian Algorithm: https://web.engr.oregonstate.edu/~magana/ECE462-562/Lecture%20Notes/Wikipedia%20gcd%20calculation.pdf
template <size_t bitlen> GF2Polynomial<bitlen> GF2Polynomial<bitlen>::gcd(const GF2Polynomial<bitlen>& y) const
{
    GF2Polynomial result;
    GF2Polynomial temp;

    if (degree > y.degree)
    {
        temp = y;
        result = *this;
    }
    else
    {
        temp = *this;
        result = y;
    }

    while (!temp.isZero())
    {
        result %= temp;
        if (result.isZero())
        {
            return temp;
        }

        temp %= result;
    }

    return result;
}

template <size_t bitlen> vector<GF2Polynomial<bitlen>> GF2Polynomial<bitlen>::factor() const
{
    vector<GF2Polynomial<bitlen>> factors;
    GF2Polynomial derivative = this->derivative();
    GF2Polynomial squareFactor = this->gcd(derivative);
    GF2Polynomial squareFreeFactor = *this;

    if (!squareFactor.isOne())
    {
        squareFreeFactor /= squareFactor;
    }

    factors = squareFreeFactor.factorDDF();

    if (!squareFactor.isOne())
    {
        // Assuming that inputs don't have squares.
        factors.push_back(squareFactor);
    }

    return factors;
}

// DDF Algorithm: https://carleton.ca/math/wp-content/uploads/Factoring-Polynomials-over-Finite-Fields_Melissa-Scott.pdf (There's a Typo)
// https://en.wikipedia.org/wiki/Factorization_of_polynomials_over_finite_fields#Distinct-degree_factorization
template <size_t bitlen> vector<GF2Polynomial<bitlen>> GF2Polynomial<bitlen>::factorDDF() const
{
    vector<GF2Polynomial<bitlen>> factors;
    vector<GF2Polynomial<bitlen>> factorsEDF;
    GF2Polynomial squareFreeFactor = *this;

    GF2Polynomial temp = GF2Polynomial(bitset<bitlen>(2));
    GF2Polynomial two = GF2Polynomial(bitset<bitlen>(2));
    GF2Polynomial<bitlen> mod;
    GF2Polynomial<bitlen> gcd;
    int i = 1;


    while (squareFreeFactor.degree >= 2 * i)
    {
        temp = temp.squareMod(squareFreeFactor);
        mod = (temp % squareFreeFactor) - two;
        gcd = mod.gcd(squareFreeFactor);

        if (!gcd.isOne())
        {
            if (gcd.degree > i)
            {
                factorsEDF = gcd.factorEDF(i);
                factors.insert(factors.end(), factorsEDF.begin(), factorsEDF.end());
            }
            else
            {
                factors.push_back(gcd);
            }
            squareFreeFactor /= gcd;

            if (squareFreeFactor.isOne())
            {
                break;
            }
        }

        i++;
    }

    if (squareFreeFactor.degree > degree / 2)
    {
        factors.push_back(squareFreeFactor);
    }

    return factors;
}

// CZ EDF Algorithm: https://en.wikipedia.org/wiki/Factorization_of_polynomials_over_finite_fields#Cantor%E2%80%93Zassenhaus_algorithm
// GF2 Variation for CZ EDF Algo: https://planetmath.org/cantorzassenhaussplit
template <size_t bitlen> vector<GF2Polynomial<bitlen>> GF2Polynomial<bitlen>::factorEDF(size_t factorDegree) const
{
    vector<GF2Polynomial<bitlen>> factors;
    factors.push_back(*this);
    GF2Polynomial f = *this;

    if (degree == factorDegree)
    {
        return factors;
    }

    if (degree % factorDegree != 0)
    {
        throw runtime_error("Value Error: degree must equal k * factorDegree for some k.\n");
    }

    GF2Polynomial<bitlen> h;
    GF2Polynomial<bitlen> g;
    GF2Polynomial<bitlen> factor;

    while (factors.size() < degree / factorDegree)
    {
        h.randPoly(degree - 1);

        if (!f.gcd(h).isOne())
        {
            continue;
        }

        g = h;
        for (int i = 1; i < factorDegree; i++)
        {
            h = h.squareMod(*this);
            g += h;
        }


        for (int j = 0; j < factors.size(); j++)
        {
            factor = factors[j];

            if (factor.degree == factorDegree)
            {
                continue;
            }
            h = factor.gcd(g);
            if (!h.isOne() && h.degree != factor.degree)
            {
                factors.push_back(h);
                factors[j] = factor / h;
            }
        }
    }

    return factors;
}

template <size_t bitlen> string GF2Polynomial<bitlen>::str(size_t numBits) const
{
    stringstream sstream;
    bitset<bitlen> mask = bitset<bitlen>(4294967295);
    bitset<bitlen> bitsCopy = bits;

    sstream << setfill('0') << setw(8) << hex << (bitsCopy & mask).to_ulong();
    bitsCopy >>= 32;

    for (int i = 1; i < (numBits / 32); i++)
    {
        sstream << ' ' << setfill('0') << setw(8) << hex << (bitsCopy & mask).to_ulong();
        bitsCopy >>= 32;
    }

    return sstream.str();
}

template <size_t bitlen> void GF2Polynomial<bitlen>::randPoly(size_t maxDegree)
{
    bits = bitset<bitlen>(0);
    degree = 30;

    for (int i = 0; i < min(bitlen - 1, maxDegree); i += 31)
    {
        bits ^= rand();
        bits <<= 31;
        degree += 31;
    }

    bits ^= rand();
    degree = min(degree, bitlen - 1);

    if (degree > maxDegree)
    {
        bits >>= (degree - maxDegree);
    }

    degree = findDegree(min(maxDegree, bitlen - 1));

    return;
}

template <size_t bitlen> const size_t GF2Polynomial<bitlen>::getDegree() const
{
    return degree;
}
// ---------------------------------------------------------------------------------------------------


int main()
{
    const size_t bitLen = 512;

    int inputLen;
    cin >> inputLen;

    uint32_t chunk;
    bitset<bitLen> b;
    bitset<bitLen> c;

    for (int i = 0; i < inputLen / 16; i++)
    {
        cin >> hex >> chunk;

        c = bitset<bitLen>(chunk);
        c <<= 32 * i;
        b ^= c;
    }

    GF2Polynomial ciphertext = GF2Polynomial(b);

    vector factors = ciphertext.factor();

    // Seems that input only has 2 factors of degree deg(b) / 2.
    // It would be sufficient to only run the following instead.
    // vector factors = ciphertext.factorEDF(ciphertext.getDegree() / 2);

    vector<string> outputs;

    if (ciphertext.getDegree() < inputLen)
    {
        GF2Polynomial one = GF2Polynomial(bitset<bitLen>(1));
        outputs.push_back(one.str(inputLen) + " " + ciphertext.str(inputLen));
        outputs.push_back(ciphertext.str(inputLen) + " " + one.str(inputLen));
    }

    // Assumes only 2 factors
    outputs.push_back(factors[0].str(inputLen) + " " + factors[1].str(inputLen));
    outputs.push_back(factors[1].str(inputLen) + " " + factors[0].str(inputLen));

    sort(outputs.begin(), outputs.end());

    for (string output : outputs)
    {
        cout << output << endl;
    }
}