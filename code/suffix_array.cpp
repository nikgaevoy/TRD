namespace suff_arr {

const int MAXN = 2e5 + 10;

string s;
int n;
int p[MAXN];
int lcp[MAXN];
int pos[MAXN];
int c[MAXN];

void print() {
#ifndef LOCAL
    return;
#endif
    eprintf("p:\n");
    forn(i, sz(s)) {
        eprintf("i=%d -- %d: %s, lcp=%d, c=%d\n", i, p[i], s.substr(p[i], sz(s) - p[i]).data(), lcp[i], c[p[i]]);
    }
    eprintf("\n");
}

void build(const string& s_) {
    static int cnt[MAXN];
    static int np[MAXN];
    static int nc[MAXN];

    s = s_;
    n = sz(s);

    memset (cnt, 0, sizeof cnt);
    for (char ch : s) {
        ++cnt[int(ch)];
    }
    forn(i, 256) {
        cnt[i + 1] += cnt[i];
    }
    forn(i, sz(s)) {
        p[--cnt[int(s[i])]] = i;
    }

    int cls = 1;
    c[p[0]] = cls - 1;
    for (int i = 1; i < n; ++i) {
        if  (s[p[i]] != s[p[i - 1]]) {
            ++cls;
        }
        c[p[i]] = cls - 1;
    }

    for (int len = 1; len <= n; len *= 2) {
        memset (cnt, 0, sizeof(int) * cls);
        forn(i, n) {
            ++cnt[c[i]];
        }
        forn(i, cls - 1) {
            cnt[i + 1] += cnt[i];
        }
        ford(i, n) {
            const int j = p[i];
            int j2 = (j - len + n) % n;
            np[--cnt[c[j2]]] = j2;
        }
        memcpy(p, np, sizeof(int) * n);

        cls = 1;
        nc[p[0]] = cls - 1;
        for (int i = 1; i < n; ++i) {
            if  (c[p[i]] != c[p[i - 1]] || c[(p[i] + len) % n] != c[(p[i - 1] + len) % n]) {
                ++cls;
            }
            nc[p[i]] = cls - 1;
        }
        memcpy(c, nc, sizeof(int) * n);
    }

    forn(i, n) {
        pos[p[i]] = i;
    }

    int pref = 0;
    forn(i, n) {
        int pi = pos[i];
        if (pi == n - 1) {
            continue;
        }
        int j = p[pi + 1];
        while (i + pref < n && j + pref < n && s[i + pref] == s[j + pref]) {
            ++pref;
        }
        lcp[pi] = pref;
        pref = max(0, pref - 1);
    }

//        print();
}

};
