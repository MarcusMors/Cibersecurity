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

using namespace std;

void printenv()
{
  int i = 0;
  while (environ[i] != NULL) {
    printf("%s\n", environ[i]);
    i++;
  }
}

int main(int argc, char *argv[])
{
  if (argc < 2) {
    println("Usage: {} <true|false>", argv[0]);
    return 1;
  }
  bool print_parent{ false };
  if (string(argv[1]) == "true" or string(argv[1]) == "1") {
    print_parent = true;
  } else if (string(argv[1]) == "false" or string(argv[1]) == "0") {
    print_parent = false;
  } else {
    println("Invalid Boolean");
    return 1;
  }

  pid_t childPid;
  enum { child_PID = 0 };
  switch (childPid = fork()) {
  case child_PID:// child
    if (not print_parent) { printenv(); }
    exit(0);
  default:// parent
    if (print_parent) { printenv(); }
    exit(0);
  }
  return 0;
}
