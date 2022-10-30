for (char ch : s) {   // main cycle
    const int pos = ch - 'a';
    bs next = sum(row, row & has[pos]) | (row & rev[pos]);
    cnt += next[m]; next[m] = 0; row = next;
}
