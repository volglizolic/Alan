# Alan
A simple strict-typed functional programming language

Alan programming language looks and feels like pascal.

Variables:
There are two types of variables: int and byte (both can be 1D array eg:int x[15] or simple variable eg:byte x)
Variable definition examples:
  i : int;
  b : byte;
  s : byte [20];

Commends:
One line commends start with --
Multiple line commends start with: (* and end with: *) (multiple line commends can be enfolded)

Functions:
There are both functions and procedures. Functions return either int or byte. Procedure have no return type (void).
Function parameters can be passed by reference using the keyword reference or by value (by default).
Function definition examples:
  p1 () : proc
  p2 (n : int) : proc
  p3 (a : int, b : int, b : reference byte) : proc
  f1 (int x) : int
  f2 (s : reference byte []) : int

Input/Output:
Alan has 4 input and 4 output functions in order for the user to comunicate with the program:
  Output:
    writeInteger (n : int) : proc
    writeByte (b : byte) : proc
    writeChar (b : byte) : proc
    writeString (s : reference byte []) : proc
  Input:
    readInteger () : int 
    readByte () : byte
    readChar () : byte
    readString (n : int, s : reference byte []) : proc

Type convertions:
There are two functions to convert variables from byte to int and int to byte:
  extend (b : byte) : int
  shrink (i : int) : byte

String procecing:
  strlen (s : reference byte []) : int 
  strcmp (s1 : reference byte [], s2 : reference byte []) : int
  strcpy (trg : reference byte [], src : reference byte []) : proc
  strcat (trg : reference byte [], src : reference byte []) : proc

Alan grammar (⟨id⟩, ⟨int-const⟩, ⟨char-const⟩ and ⟨string-literal⟩ are terminals):
⟨program⟩ ::= ⟨func-def⟩
⟨func-def⟩ ::= ⟨id⟩ “(” [⟨fpar-list⟩] “)” “:” ⟨r-type⟩ (⟨local-def⟩)* ⟨compound-stmt⟩
⟨fpar-list⟩ ::= ⟨fpar-def⟩ ( “,” ⟨fpar-def⟩)∗
⟨fpar-def⟩ ::= ⟨id⟩ “:” [ “reference” ] ⟨type⟩
⟨data-type⟩ ::= “int” | “byte”
⟨type⟩ ::= ⟨data-type⟩ [ “[” “]” ]
⟨r-type⟩ ::= ⟨data-type⟩ | “proc”
⟨local-def⟩ ::= ⟨func-def⟩ | ⟨var-def⟩
⟨var-def⟩ ::= ⟨id⟩ “:” ⟨data-type⟩ [ “[” ⟨int-const⟩ “]” ] “;”
⟨stmt⟩ ::= “;” | ⟨l-value⟩ “=” ⟨expr⟩ “;” | ⟨compound-stmt⟩ | ⟨func-call⟩ “;”
         | “if” “(” ⟨cond⟩ “)” ⟨stmt⟩ [ “else” ⟨stmt⟩]
         | “while” “(” ⟨cond⟩ “)” ⟨stmt⟩ | “return” [⟨expr⟩] “;”
⟨compound-stmt⟩ ::= “{” (⟨stmt⟩)∗ “}”
⟨func-call⟩ ::= ⟨id⟩ “(” [⟨expr-list⟩] “)”
⟨expr-list⟩ ::= ⟨expr⟩ ( “,” ⟨expr⟩) ∗
⟨expr⟩ ::= ⟨int-const⟩ | ⟨char-const⟩ | ⟨l-value⟩ | “(” ⟨expr⟩ “)” | ⟨func-call⟩
         | ( “+” | “-” ) ⟨expr⟩ | ⟨expr⟩ ( “+” | “-” | “*” | “/” | “%” ) ⟨expr⟩
⟨l-value⟩ ::= ⟨id⟩ [ “[” ⟨expr⟩ “]” ] | ⟨string-literal⟩
⟨cond⟩ ::= “true” | “false” | “(” ⟨cond⟩ “)” | “!” ⟨cond⟩
         | ⟨expr⟩ ( “==” | “!=” | “<” | “>” | “<=” | “>=” ) ⟨expr⟩
         | ⟨cond⟩ ( “&” | “|” ) ⟨cond⟩

Alan Programmes Examples:

Hello World!:

1 helloWorld () : proc
2 {
3   writeString(”Hello world!\n”);
4 }


Hanoi Towers:

1 solve () : proc
2
3   hanoi (rings : int, source : reference byte [],
4          target : reference byte [], auxiliary : reference byte []) : proc
5
6     move (source : reference byte [], target : reference byte[]) : proc
7     {
8       writeString(”Moving from ”);
9       writeString(source);
10      writeString(” to ”);
11      writeString(target);
12      writeString(”.\n”);
13    }
14
15  { -- hanoi
16    if (rings >= 1) {
17      hanoi(rings-1, source, auxiliary, target);
18      move(source, target);
19      hanoi(rings-1, auxiliary, target, source);
20    }
21  } -- hanoi
22
23 NumberOfRings : int;
24
25 { -- solve
26  writeString(”Rings: ”);
27  NumberOfRings = readInteger();
28  hanoi(NumberOfRings, ”left”, ”right”, ”middle”);
29 } -- solve


Bubble Short:

1 main () : proc
2
3   bsort (n : int, x : reference int []) : proc
4
5     swap (x : reference int, y : reference int) : proc
6       t : int;
7     {
8       t = x;
9       x = y;
10      y = t;
11    }
12
13    changed : byte;
14    i : int;
15
16  { -- bsort
17    changed = ’y’;
18    while (changed == ’y’) {
19      changed = ’n’;
20      i = 0;
21      while (i < n-1) {
22        if (x[i] > x[i+1]) {
23          swap(x[i], x[i+1]);
24          changed = ’y’;
25        }
26        i = i+1;
27      }
28    }
29  } -- bsort
30
31  writeArray (msg : reference byte [], n : int, x : reference int []) : proc
32    i : int;
33  {
34    writeString(msg);
35    i = 0;
36    while (i < n) {
37      if (i > 0) writeString(”, ”);
38      writeInteger(x[i]);
39      i = i+1;
40    }
41    writeString(”\n”);
42  }
43
44 seed : int;
45 x : int [16];
46 i : int;
47
48 { -- main
49  seed = 65;
50  i = 0;
51  while (i < 16) {
52    seed = (seed * 137 + 220 + i) % 101;
53    x[i] = seed;
54    i = i+1;
55  }
56  writeArray(”Initial array: ”, 16, x);
57  bsort(16, x);
58  writeArray(”Sorted array: ”, 16, x);
59 } -- main
