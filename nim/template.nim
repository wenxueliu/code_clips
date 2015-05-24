#Templates are a simple substitution mechanism that operates on Nim's abstract
#syntax trees. Templates are processed in the semantic pass of the compiler. They
#integrate well with the rest of the language and share none of C's preprocessor
#macros flaws.

template `!=` (a,b: expr): expr =
    # this definition exists in the System moduleo
    not(a == b)

assert(5!=6)    # the compiler rewrites that to: assert(not (5 == 6))

#Templates are especially useful for lazy evaluation purposes. Consider a simple proc for logging:
const
    debug = true

proc log(msg: string) {.inline.} =
    if debug: stdout.writeln(msg)

var
    x = 4
log("x has the value: " & $x)

#if debug is set to false someday, the quite expensive $ and & operations are
#still performed! (The argument evaluation for procedures is eager).

template logPlus(msg: string) =
    if debug: stdout.writeln(msg)

logPlus("x has the value: " & $x)

template withFile(f: expr, filename: string, mode: FileMode,
                  body: stmt): stmt {.immediate.} =
  let fn = filename
  var f: File
  if open(f, fn, mode):
    try:
      body
    finally:
      close(f)
  else:
    quit("cannot open: " & fn)

withFile(txt, "ttempl3.txt", fmWrite):
  txt.writeln("line 1")
  txt.writeln("line 2")
