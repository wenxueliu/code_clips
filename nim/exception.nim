var
    f: File

if open(f, "number.txt"):
    try:
        let a = readLine(f)
        let b = readLine(f)
        echo "sum: ", parseInt(a) + parseInt(b)
    except OverflowError:
        echo "overflow!"
    except ValueError:
        echo "could not convert string to integer"
    except IOError:
        echo "IOError"
    except:
        echo "Unknown exception!"
        let
            e = getCurrentException()
            msg = getCurrentExceptionMsg()
        echo "Got exception ", repr(e), " with message ", msg
        raise
    finally:
        close(f)

proc Myopen(fileName: string) {.raises: [IOError].} : File =
    var f : File
    new(f)
    open(f,fileName)
    var line : string
    while true:
        line = readLine(f)
        if line == "":
            break
