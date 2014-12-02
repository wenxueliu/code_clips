function sum(...)
    local s = 0
    for i,v in ipairs{...} do
        s = s + v
    end
    return s
end

function sum1(...)
    local s = 0
    for i = 1, select('#',...) do
        s = s + select(i,...)
    end
    return s
end

function tc_sum()
    -- print(sum{1,2,3,4}) error
    print(sum(1,2,3,4))
    print(sum1(1,2,3,4))
    -- print(sum({1,2,3,4}))  error
end

-------------------------------------

function rename(arg)
    arg.new,arg.old = arg.old,arg.new
    return arg.old, arg.new
end

function tc_rename()
    print(rename{old = "oldfile", new = "newfile"})
end

-------------------------------------

function foo() return 'a', 'b' end

function tc_foo()
    x,y = foo(),'c'

    print("x= ",x)
    print("y= ",y)
end

--------------------------------------

function newCount(i)
    local i = i or 0
    return function()
        i = i + 1
        return i
    end
end


function tc_newCount()
    c1 = newCount(1)
    c2 = newCount(0)
    print("The return value of first call with c1 is " .. c1())
    print("The return value of second call with c1 is " .. c1())
    print("The return value of first call with c2 is " .. c2())
end

function main()
    tc_newCount()
end

main()
