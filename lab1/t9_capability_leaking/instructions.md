
## Capability leaking

```c++
fdx = () //setuid(root)
//...
setuid(); //cambia sus privilegios
//euid = ruid //// root = julio
//execute shell, este shell puede usar el filedescriptor que fue abierto con privilegios de root.
```

- rpp
- camara
- reloj
- grabación regular.
- url directo, no url a directorio

limite: siguiente laboratorio.


