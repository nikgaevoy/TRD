// algorithm itself starts here
void mobius (int* a, int n, int sign)
{
    forn (i, n)
    {
        int free = ((1 << n) - 1) ^ (1 << i);
        for (int mask = free; mask > 0; mask = ((mask - 1) & free))
            (sign == +1 ? add : sub)(a[mask ^ (1 << i)], a[mask]);
        add(a[1 << i], a[0]);
    }
}

// maximum number of bits allowed
const int B = 20;

vi fast_conv (vi a, vi b)
{
    assert(!a.empty());
    const int bits = __builtin_ctz(sz(a));
    assert(sz(a) == (1 << bits) && sz(a) == sz(b));

    static int trans_a[B + 1][1 << B];
    static int trans_b[B + 1][1 << B];
    static int trans_res[B + 1][1 << B];

    forn (cnt, bits + 1)
    {
        for (auto cur : {trans_a, trans_b, trans_res})
            fill(cur[cnt], cur[cnt] + (1 << bits), 0);
    }

    forn (mask, 1 << bits)
    {
        const int cnt = __builtin_popcount(mask);
        trans_a[cnt][mask] = a[mask];
        trans_b[cnt][mask] = b[mask];
    }

    forn (cnt, bits + 1)
    {
        mobius(trans_a[cnt], bits, +1);
        mobius(trans_b[cnt], bits, +1);
    }

    // Not really a valid ranked mobius transform! But algorithm works anyway

    forn (i, bits + 1) forn (j, bits - i + 1) forn (mask, 1 << bits)
        add(trans_res[i + j][mask], mult(trans_a[i][mask], trans_b[j][mask]));

    forn (cnt, bits + 1)
        mobius(trans_res[cnt], bits, -1);

    forn (mask, 1 << bits)
    {
        const int cnt = __builtin_popcount(mask);
        a[mask] = trans_res[cnt][mask];
    }

    return a;
}
