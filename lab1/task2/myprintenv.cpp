/**
 * Copyright (C) 2025 José Enrique Vilca Campana
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <cstdio>
#include <cstdlib>
#include <print>
#include <unistd.h>

extern char **environ;

void printenv()
{
  int i = 0;
  while (environ[i] != NULL) {
    printf("%s\n", environ[i]);
    i++;
  }
}

int main()
{
  pid_t childPid;
  enum { child_PID = 0 };
  switch (childPid = fork()) {
  case child_PID:// child
    printenv();
    exit(0);
  default:// parent
    // printenv();
    exit(0);
  }
}
