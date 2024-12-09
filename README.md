Original problem hosted here: https://www.codingame.com/ide/puzzle/nintendo-sponsored-contest

Challenge:
-------------------------------------------------------------------------------------------------
The SETI program has received a series of messages from Alpha Centauri. The most frequent message
seems to be: `541a4231 5d324646 27219a26 12497b0e 724eddcb 0e131617 9521bedf 55544dc7`

It is not known why these messages are encoded, but there is a good chance that the Alpha 
Centaurians are trying to evaluate our cognitive abilities before establishing advanced contact.

Our best engineers are working to decode these messages, and they've already succeeded at 
identifying the program that the Centaurians use to encode the messages. This program takes a 
size and a list of numbers as inputs. It then outputs the encoded message (see the pseudo-code 
below).

But so far, no one has been able to decode the messages. We are well aware that this task is by 
far the hardest that we’ve encountered, and that only a true NERD will be able to pull it off!

Here is a pseudo-code version of the encoding program:
```
READ size
READ size / 16 integers in array a
WRITE size / 16 zeros in array b

For i from 0 to size - 1:
    For j from 0 to size - 1:
        b[(i+j)/32] ^= ((a[i/32] >> (i%32)) & (a[j/32 + size/32] >> (j%32)) & 1) << ((i+j)%32)

PRINT b
```
The goal is to determine the series of numbers entered (array a) from the encoded output of the 
program (array b). The numbers – input and output – should be displayed in hexadecimal, 8 
characters padded with 0 (for example, 42 would be displayed as 0000002a).
If you pass the output of your program as input to the encoder above, you should obtain the 
input provided to your program.

If there are several possible decoded values, you should display all the possibilities in 
alphabetical order.

Constraints:
-------------------------------------------------------------------------------------------------
0 < `s` ≤ 256

0 < `N1` ≤ 16 : (Number of input integers in hexadecimal format.)

0 < `N2` ≤ 32 : (Number of output integers in hexadecimal format.)

Sample Input/Output:
-------------------------------------------------------------------------------------------------
Input:
```
32
46508fb7 6677e201
```
Expected Output:
```
b0c152f9 ebf2831f
ebf2831f b0c152f9
```

Solution / Thought Process:
-------------------------------------------------------------------------------------------------
The first thing we need to do is determine what the encoding function is. If we do a bit of 
manipulation to rewrite the function in terms of bits, we can print out the operation on each 
bit. It becomes obvious that there's a pattern and if we shift the lines around, we notice it
looks a lot like multiplication, except there is no carry. 

A google search for "binary 
multiplication without carry" takes us to [Wikipedia (Carryless Product)](https://en.wikipedia.org/wiki/Carry-less_product).
On the wikipedia page, it says that this operation "can be used to model operations over finite fields, in particular 
multiplication of polynomials from `GF(2)[X]`, the polynomial ring over `GF(2)`." So we can look up,
"polynomial factorization over finite fields" and land at [Wikipedia (Factorization Of Polynomials Over Finite Fields)](https://en.wikipedia.org/wiki/Factorization_of_polynomials_over_finite_fields).

Finally, we have an algorithm we can use and all that remains is to understand and implement it, 
which is far easier said than done. Using the Cantor–Zassenhaus algorithm for equal-degree 
factorization is sufficient for solving this problem, since the given input conveniently always 
factors out to two equal degree irreducable polynomials.
