# Print stack limit in Kb
ulimit -s

# Set stack limit in Kb, session-local, so resets after terminal restart
ulimit -S -s 131072

# Profile time
time ./olymp

# Profile time, memory, etc.
# Make sure to use the full path
/usr/bin/time -v ./olymp
