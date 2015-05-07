from strutils import parseInt

when false :
    echo("A number please: ")
    let n = parseInt(readLine(stdin))
    case n
    of 0..2, 4..7   : echo("The number is in the set: {0, 1, 2, 4, 5, 6, 7}")
    of 3, 8         : echo("The number is 3 or 8")
    else            : discard

    echo("while Counting to ten: ")
    var i = 0
    while i <= 10:
        echo i
        inc(i)

    echo("for Counting to ten: ")
    for i in countup(1, 10):
        echo($i)
        ## --> Outputs 1 2 3 4 5 6 7 8 9 10 on different lines

    for i in countdown(10, 1):
        echo($i)

    for i in countup(0,5):
        if i == 2:
            echo("skip 2")
            continue
        echo($i)

when system.hostOS == "windows":
  echo("running on Windows!")
elif system.hostOS == "linux":
  echo("running on Linux!")
elif system.hostOS == "macosx":
  echo("running on Mac OS X!")
else:
  echo("unknown operating system")

const fac4 = (var x = 1; for i in 1..4: x *= i; x)
echo fac4


proc yes(question: string): bool =
  echo(question, " (y/n)")
  while true:
    case readLine(stdin)
    of "y", "Y", "yes", "Yes": return true
    of "n", "N", "no", "No": return false
    else: echo("Please be clear: yes or no")

if yes("Should I delete all your important files?"):
  echo("I'm sorry Dave, I'm afraid I can't do that.")
else:
  echo("I think you know what the problem is just as well as I do.")

proc positiveOrNegative(num : int): string =
  result = case num:
    of low(int).. -1:
      "negative"
    of 0:
      "zero"
    of 1..high(int):
      "positive"
    else:
      "impossiable"

echo positiveOrNegative(-1)
echo positiveOrNegative(0)
echo positiveOrNegative(4)
