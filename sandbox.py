## Some assorted functions written to experiment with various things.

import pickle

def expand_encoding(size):
    b = []
    
    for _ in range(size//16):
        b.append([''] * 32)

    for i in range(size):
        for j in range(size):
            b[(i+j)//32][(i+j)%32] += 'a[{:02d}][{:02d}] & a[{:02d}][{:02d}] ^ '.format(i//32, i%32, j//32 + size//32, j%32)

    with open("func_expansion_{}.txt".format(size), "w") as f:
        for i in range(size//16):
            for j in range(32):
                f.write('b[{:02d}][{:02d}] = {}\n'.format(i, j, b[i][j][:-3]))
                
        
def encode(size, s):
    a = [int(x, 16) for x in s.split()]
    b = [0] * (size // 16)

    for i in range(size):
        for j in range(size):
            b[(i+j)//32] ^= ((a[i//32] >> (i%32)) & (a[(j//32)+(size//32)] >> (j%32)) & 1) << ((i+j)%32)

    output = ''
    for x in b:
        output += '{:08x} '.format(x)

    return output[:-1]

def mod_gf2(a, deg_A, b, deg_B):
    if deg_B > deg_A:
        return a

    b <<= (deg_A - deg_B)

    mask = 1 << deg_A
    while(deg_A >= deg_B):
        if a & mask:
            a ^= b

        deg_A -= 1
        mask >>= 1
        b >>= 1
            
    return a

def is_irreducable(a, deg_A, known_irreducables):
    for degree in known_irreducables:
        for irreducable in known_irreducables[degree]:
            if not mod_gf2(a, deg_A, irreducable, degree):
                return False

    return True
    

def calc_irreducables(size, new=False):
    try:
        with open('irreducable_dict.pickle', 'rb') as handle:
            output = pickle.load(handle)
    except:
        output = {1: [2, 3]}

    if new:
        output = {1: [2, 3]}

    for bits in range(max(output.keys()) + 1, size):
        print(bits) 
        output[bits] = []
        for num in range(2**bits, 2**(bits+1)):
            if is_irreducable(num, bits, output):
                output[bits].append(num)

        with open('irreducable_dict.pickle', 'wb') as handle:
            pickle.dump(output, handle, protocol=pickle.HIGHEST_PROTOCOL)
    
    return output
        
    
    
    
