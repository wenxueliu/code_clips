##Expression Macros
###to work with Nim syntax trees, we need an API that is defined in the
### ``macros`` module:
###import macros

macro debug(n: varargs[expr]): stmt =
  # `n` is a Nim AST that contains a list of expressions;
  # this macro returns a list of statements:
  result = newNimNode(nnkStmtList, n)
  # iterate over any argument that is passed to this macro:
  for i in 0..n.len-1:
    # add a call to the statement list that writes the expression;
    # `toStrLit` converts an AST to its string representation:
    result.add(newCall("write", newIdentNode("stdout"), toStrLit(n[i])))
    # add a call to the statement list that writes ": "
    result.add(newCall("write", newIdentNode("stdout"), newStrLitNode(": ")))
    # add a call to the statement list that writes the expressions value:
    result.add(newCall("writeln", newIdentNode("stdout"), n[i]))

var
  a: array[0..10, int]
  x = "some string"
a[0] = 42
a[1] = 45

debug(a[0], a[1], x)


##Statement macros

macro case_token(n: stmt): stmt =
  # creates a lexical analyzer from regular expressions
  # ... (implementation is an exercise for the reader :-)
  discard

case_token: # this colon tells the parser it is a macro statement
of r"[A-Za-z_]+[A-Za-z_0-9]*":
  return tkIdentifier
of r"0-9+":
  return tkInteger
of r"[\+\-\*\?]+":
  return tkOperator
else:
  return tkUnknown


##Building your first macro

import strutils, tables

proc readCfgAtRuntime(cfgFilename: string): Table[string, string] =
  let
    inputString = readFile(cfgFilename)
  var
    source = ""

  result = initTable[string, string]()
  for line in inputString.splitLines:
    # Ignore empty lines
    if line.len < 1: continue
    var chunks = split(line, ',')
    if chunks.len != 2:
      quit("Input needs comma split values, got: " & line)
    result[chunks[0]] = chunks[1]

  if result.len < 1: quit("Input file empty!")

let info = readCfgAtRuntime("data.cfg")

when isMainModule:
  echo info["licenseOwner"]
  echo info["licenseKey"]
  echo info["version"]

##Generating source code
import macros, strutils

macro readCfgAndBuildSource(cfgFilename: string): stmt =
  let
    inputString = slurp(cfgFilename.strVal)
  var
    source = ""

  for line in inputString.splitLines:
    # Ignore empty lines
    if line.len < 1: continue
    var chunks = split(line, ',')
    if chunks.len != 2:
      error("Input needs comma split values, got: " & line)
    source &= "const cfg" & chunks[0] & "= \"" & chunks[1] & "\"\n"

  if source.len < 1: error("Input file empty!")
  result = parseStmt(source)

readCfgAndBuildSource("data.cfg")

when isMainModule:
  echo cfglicenseOwner
  echo cfglicenseKey
  echo cfgversion

##Generating AST by hand
import macros

dumpTree:
  const cfgversion: string = "1.1"
  const cfglicenseOwner= "Hyori Lee"
  const cfglicenseKey= "M1Tl3PjBWO2CC48m"

import macros, strutils

macro readCfgAndBuildAST(cfgFilename: string): stmt =
  let
    inputString = slurp(cfgFilename.strVal)

  result = newNimNode(nnkStmtList)
  for line in inputString.splitLines:
    # Ignore empty lines
    if line.len < 1: continue
    var chunks = split(line, ',')
    if chunks.len != 2:
      error("Input needs comma split values, got: " & line)
    var
      section = newNimNode(nnkConstSection)
      constDef = newNimNode(nnkConstDef)
    constDef.add(newIdentNode("cfg" & chunks[0]))
    constDef.add(newEmptyNode())
    constDef.add(newStrLitNode(chunks[1]))
    section.add(constDef)
    result.add(section)

  if result.len < 1: error("Input file empty!")

readCfgAndBuildAST("data.cfg")

when isMainModule:
  echo cfglicenseOwner
  echo cfglicenseKey
  echo cfgversion
