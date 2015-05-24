proc interpret*(code: string) =
    ## Interprets the brainfuck `code` string, reading from stdin and writing to
    ## stdout.

    var
        tape = newSeq[char]()
        codePos = 0
        tapePos = 0
    proc run(skip = false): bool =
        echo "\ncodePos: ", codePos, "\ntapePos: ", tapePos
        while tapePos >= 0 and codePos < code.len:
            if tapePos > tape.len:
                tape.add '\0'

            if code[codePos] == '[':
                inc codePos
                let oldPos = codePos
                while run(tape[tapePos] == '\0'):
                    codePos = oldPos
            elif code[codePos] == ']':
                return tape[tapePos] != '\0'
            if not skip:
                case code[codePos]
                of '+': inc tape[tapePos]
                of '-': dec tape[tapePos]
                of '>': inc tapePos
                of '<': dec tapePos
                of '.': stdout.write tape[tapePos]
                of ',': tape[tapePos] = stdin.readChar
                else: discard

            inc codePos

    discard run()

when isMainModule:
    import os
    echo "please input you code"
    let code = if paramCount() > 0 : readFile paramStr(1)
                else: readAll stdin
    echo "\necho you code: ", code
    interpret code
