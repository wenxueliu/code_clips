# This is a comment
echo("What's your name? ")
var name: string = readLine(stdin)
case name
of "":
  echo("Poor soul, you lost your name?")
of "name":
  echo("Very funny, your name is name.")
of "Dave", "Frank":
  echo("Cool name!")
else:
  echo("Hi, ", name, "!")
