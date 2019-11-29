// assumes that strings consist of lowercase latin letters
const int M = ((int)1e5 + 64) / 32 * 32;
// maximum value of m
using bs = bitset<M>;
using uint = unsigned int;
const ll bnd = (1LL << 32);

// WARNING: invokes undefined behaviour of modifying ans through pointer to another data type (uint)
// seems to work, but be wary
bs sum (const bs &bl, const bs &br)
{
    const int steps = M / 32;
    const uint* l = (uint*)&bl;
    const uint* r = (uint*)&br;
    
    bs ans;
    uint* res = (uint*)&ans;

    int carry = 0;
    forn (i, steps)
    {
        ll cur = ll(*l++) + ll(*r++) + carry;
        carry = (cur >= bnd);
        cur = (cur >= bnd ? cur - bnd : cur);
        *res++ = uint(cur); 
    }

    return ans;
}

int fast_lcs (const string &s, const string &t)
{
    const int m = sz(t);
    const int let = 26;

    vector<bs> has(let);
    vector<bs> rev = has;

    forn (i, m)
    {
        const int pos = t[i] - 'a';
        has[pos].set(i);
        forn (j, let) if (j != pos)
            rev[j].set(i);
    }

    bs row;
    forn (i, m)
        row.set(i);

    int cnt = 0;
    for (char ch : s)
    {
        const int pos = ch - 'a';
        
        bs next = sum(row, row & has[pos]) | (row & rev[pos]);
        cnt += next[m];
        next[m] = 0;

        row = next;
    }

    return cnt;
}
