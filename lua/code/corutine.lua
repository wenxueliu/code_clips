function test_coroutine()
    local co = coroutine.create(function () print("hi") end)
    print(co)
    print(coroutine.status(co))
    coroutine.resume(co)
    print(coroutine.status(co))
end

function test_coroutine1()
    local co = coroutine.create(function()
        for i=1,5 do
            print("co",i)
            print("------------------")
            coroutine.yield()
        end
    end)

    for i=1,7 do
        print("------------------")
        print(coroutine.resume(co))
        print(coroutine.status(co))
    end
end

--[[
只有第一次调用resume时，并没有对应的 yield 在等待它，所有传递
给 resume 的额外参数都将视为协同程序主函数的参数，之后传递给
resume 的参数并不会为协同主函数的参数
]]--
function test_coroutine2()
    local co = coroutine.create(function(a,b,c)
        for i=1,2 do
            print("------------------")
            print(coroutine.yield())
            print("co",a,b,c)
        end
    end)

    for i=1,4 do
        print("------------------")
        print(coroutine.resume(co,i,i+1,i+2))
        print(coroutine.status(co))
    end
end

function test_coroutine3()
    local co = coroutine.create(function(a,b,c)
        for i=1,2 do
            print("-----before yield-----")
            print(coroutine.yield(a,b,c))
            print("-----after yield-----")
        end
    end)

    for i=1,4 do
        print("-----before resume------")
        print(coroutine.resume(co,i,i+1,i+2))
        print(coroutine.status(co))
        print("-----after resume------")
    end
end

function test_coroutine4()
    local co = coroutine.create(function(a,b)
        for i=1,2 do
            print("-----before yield-----")
            print(coroutine.yield(a+b, a-b))
            print("-----after yield-----")
        end
    end)

    for i=1,4 do
        print("-----before resume------")
        print(coroutine.resume(co,i,i+1,i+2))
        print(coroutine.status(co))
        print("-----after resume------")
    end


end

---------------test-------------------
--test_coroutine()
--test_coroutine1()
--test_coroutine2()
--test_coroutine3()
test_coroutine4()
