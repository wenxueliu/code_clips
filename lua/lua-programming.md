**作者** 

    刘文学
    
**版本**
    0.1 
    
###问题

* assert 是否可以大量应用
* xpcall pcall 是否可以大量应用

###Tips

* 代替 if else ：x = x or v    max = (x > y) and x or y  assert()
* table 赋值 ： x[#x+1] = i

### string

####连接
	
	a = str1..str2..num1 .. num3 
	--注意字母和数字，数字和数字，字母和之母直接 "." 的区别

上面的代码确实可以正常的完成工作，然而当行数较多时，这种方法将会导致大量的内存重新分配和内存间的数据拷贝，由此而带来的性能开销也是相当可观的。为了解决该问题，Lua中则可以利用table的concat方法来解决这一问题，见如下代码：

	local t = {1,2,3,4,5,6,7}
	local s1 = table.concat(t)
	local s2 = table.concat(t,"\n")
    local s3 = table.concat(t,"\n",1,4)

因此，在数据连接时，优先使用 table.concat()

###table

####构造

	a = {["x"] = 10, ["y"] = 20,}  -- 最后的逗号可以保留

	a = { x = 10, y = 20 }

	a = {}; a.x = 10; a.y = 20

均可以可以通过 a.x 或 a["x"] 访问，这样初始化 a = { "x" = 10, "y" = 20} 错误

	a = {x = 10, y = 45; "one", "two", "three" }

可以看到上面的声明中同时存在逗号(,)和分号(;)两种元素分隔符，这种写法在Lua中是允许的。我们通常会将分号(;)用于分隔不同初始化类型的元素，如上例中分号之前的初始化方式为记录初始化方式，而后面则是数组初始化方式。

####快速测试一个函数

* lua -i file.lua，测试相应接口,**作者** 

    刘文学
    
**版本**
    0.1 
    
###问题

* assert 是否可以大量应用
* xpcall pcall 是否可以大量应用

###Tips

* 代替 if else ：x = x or v    max = (x > y) and x or y  assert()
* table 赋值 ： x[#x+1] = i

###字符串 string

####连接
	
	a = str1..str2..num1 .. num3  -- 注意字母和数字，数字和数字，字母和之母直接 "." 的区别

上面的代码确实可以正常的完成工作，然而当行数较多时，这种方法将会导致大量的内存重新分配和内存间的数据拷贝，
由此而带来的性能开销也是相当可观的。为了解决该问题，Lua中则可以利用table的concat方法来解决这一问题，
见如下代码：

	local t = {}
	for line in io.lines() do
		t[#t + 1] = line .. "\n"
	end
	local s = table.concat(t)

	--concat方法可以接受两个参数，因此上面的方式还可以改为：
	local t = {}
	for line in io.lines() do
		t[#t + 1] = line
	end
	local s = table.concat(t,"\n")

因此，在数据连接时，优先使用 table.concat()

###table

####构造

	a = {["x"] = 10, ["y"] = 20,}  -- 最后的逗号可以保留

	a = { x = 10, y = 20 }

	a = {}; a.x = 10; a.y = 20

均可以可以通过 a.x 或 a["x"] 访问，这样初始化 a = { "x" = 10, "y" = 20} 错误

	a = {x = 10, y = 45; "one", "two", "three" }

可以看到上面的声明中同时存在逗号(,)和分号(;)两种元素分隔符，这种写法在Lua中是允许的。我们通常
会将分号(;)用于分隔不同初始化类型的元素，如上例中分号之前的初始化方式为记录初始化方式，而后面则
是数组初始化方式。


###函数

在 Lua 中函数的调用方式和 C 语言基本相同，如：print("Hello World")和a = add(x, y)。唯一的差别是，如果函数只有一个参数，并且该参数的类型为字符串常量或table的构造器，那么圆括号可以省略，如

    print "Hello World"
    f {x = 20, y = 20}。

Lua中函数的声明方式如下：

	function maximum(a)
		local mi = 1
		local m = a[mi]
		for i, val in ipairs(a) do
		    if val > m then
		        mi,m = i,val
		    end
		end
		return m,mi
	end
	
	print(maximum{8,10,23,12,5})
	--23   3

在以上声明中，包含了函数名(add)，参数列表(a)，以及函数体。

####参数

Lua中实参和形参的数量可以不一致，一旦出现这种情况，Lua的处理规则等同于多重赋值，即实参多于形参，多出的部分被忽略，如果相反，没有被初始化的形参的缺省值为nil。

    function foo(a,b,c) return a,b,c  end
    
    x,y,z = foo('a','b')  # x = 'a', y = 'b', z = nill  c 缺省为 nil
    x,y,z = foo('a','b','c','d') # x = 'a', y = 'b', z = 'c', 'd'被丢弃

Lua为面向对象式的调用还提供了一个的语法糖--冒号操作符。表达式 o.foo(o,x) 的另一种写法是 o:foo(x)。冒号操作符使调用 o.foo 时将 o 隐含的作为函数的第一个参数。
    
**变长参数**

    function sum(sum,...) 
    	local s = sum or 0 
        for i,v in ipairs{...} do
            s = s + v 
        end 
        return s
    end
    
    print(add(3,4,5,6,7))
	--输出结果为：25
    
关于 Lua 的变长参数最后需要说明的是，由于变长参数中可能包含 nil 值，因此再使用类似获取 table 元素数量(#)的方式获取变参的数量就会出现问题。如果要想始终获得正确的参数数量，可以使用 Lua 提供的 select 函数，如：

    function sum1(...)
        local s = 0
        for i = 1, select('#',...) do
           s = s + select(i,...)
        end
        return s
    end


在含有变长参数的函数中，同样可以带有固定参数，但是固定参数一定要在变长参数之前声明，如：
 
    function test(arg1,arg2,...)
        ...
    end
    
    
**具名实参**

在函数调用时，Lua的传参规则和C语言相同，并不真正支持具名实参。但是我们可以通过table来模拟，比如：
 
    function rename(arg)
        local old = arg.old or "old"
        local new = arg.new or "new"
        return old, new
    end

即如果函数只有一个参数且为string或table类型，在调用该函数时，可以不用加圆括号

    print(rename{old = "oldfile", new = "newfile"}) 
    

 
####返回值

1 多重返回值

	function foo() return "a","b" end 

1 返回值是表达式的一部分 保留第一个返回值

    x,y,z = foo(),'c','d'  # x = 'a', y = 'c', z = 'd', 'b'被丢弃

2 返回值是一系列表达式最后一个元素，全部返回值

    x,y,z = 'z',foo() # x = 'z', y = 'a', z ='b'

3 返回值是单独的语句  丢弃所有返回值
 
    foo()  丢弃所有返回值
    
4 table构造器可以完整的接收一个函数调用的所有结果，即不会有任何数量方面的调整。 

	t = {foo()}  # t = {"a","b"}
	
__*快速测试一个函数*__

*  lua -i file.lua，测试相应接口，之后直接可以在交互shell中调用file.lua 相应函数
* 交互shell 调用 dofile(file.lua)，然后测试调用文件中的接口
 
###深入函数

**闭包**

    function newCounter() 
        local i = 0
        return function() --匿名函数
            i = i + 1
            return i
        end
    end
    
    c1 = newCounter()
    c2 = newCounter()
    print("The return value of first call is " .. c1())
    print("The return value of first call is " .. c2())
    print("The return value of second call is " .. c1())
   
输出结果为：

    --The return value of first call is 1
    --The return value of first call is 1
    --The return value of second call is 2
    
由此可以推出，Lua每次在给新的闭包变量赋值时，都会让不同的闭包变量拥有独立的"非局部变量"。

 
 一个更实际的例子
 
	 do
		--这里将原有的文件打开函数赋值给"私有变量"oldOpen，该变量在块外无法访问。
		local oldOpen = io.open
		--新增一个匿名函数，用于判断本次文件打开操作的合法性。
		local access_OK = function(filename,mode) <检查访问权限> end
		--将原有的io.open函数变量指向新的函数，同时在新函数中调用老函数以完成真正的打开操作。
		io.open = function(filename,mode)
		    if access_OK(filename,mode) then
		        return oldOpen(filename,mode)
		    else
		        return nil,"Access denied"
		    end
		end
	end

**匿名函数**

    Lib = {}
    Lib.add = function(x,y) return x + y end
    Lib.sub = function(x,y) return x - y end
    
或者是在table的构造式中直接初始化，如：
    
    Lib = { add = function(x,y) return x + y end,
            sub = function(x,y) return x - y end
             }

除此之外，Lua还提供另外一种语法来定义此类函数，如：
    
    Lib = {}
    function Lib.add(x,y) return x + y end
    function Lib.sub(x,y) return x - y end


**尾调用**

在 Lua 中支持这样一种函数调用的优化，即“尾调用消除”。我们可以将这种函数调用方式视为 goto 语句，如：

    function f(x) return g(x) end
    
由于 g(x) 函数是 f(x) 函数的最后一条语句，在函数 g() 返回之后，f() 函数将没有任何指令需要被执行，因此在函数 g() 返回时，可以直接返回到 f() 函数的调用点。由此可见，Lua 解释器一旦发现 g() 函数是 f() 函数的尾调用，那么在调用 g() 时将不会产生因函数调用而引起的栈开销。这里需要强调的是，尾调用函数一定是其调用函数的最后一条语句，否则Lua不会进行优化。然而事实上，我们在很多看似是尾调用的场景中，实际上并不是真正的尾调用，如：

    function f(x) g(x) end            --没有return语句的明确提示
    function f(x) return g(x) + 1  --在g()函数返回之后仍需执行一次加一的指令。
    function f(x) return x or g(x) --如果g()函数返回多个值，该操作会强制要求g()函数只返回一个值。
    function f(x) return (g(x))     --原因同上。
    
在Lua中，只有"return <func>(<args>)"形式才是标准的尾调用，至于参数中(args)是否包含表达式，由于表达
式的执行是在函数调用之前完成的，因此不会影响该函数成为尾调用函数。



###迭代器

在 Lua 中，迭代器通常为函数，每调用一次函数，即返回集合中的“下一个”元素。每个迭代器都需要在每次成功调用之间保持一些状态，这样才能知道它所在的位置和下一次遍历时的位置。从这一点看，Lua中 closure 机制为此问题提供了语言上的保障，见如下示例：

	function values(t)
		local i = 0
		return function()
		    i = i + 1
		    return t[i]
		end
	end
	t = {10, 20, 30}
	it = values(t)
	while true do
		local element = it()
		if element == nil then
		    break
		end
		print(element)
	end
	--另外一种基于foreach的调用方式(泛型for)
	t2 = {15, 25, 35}
	for element in values(t2) do
		print(element)
	end
	--输出结果为：
	--10
	--20
	--30
	--15
	--25
	--35
	
###泛型for的语义

上面示例中的迭代器有一个明显的缺点，即每次循环时都需要创建一个新的 closure 变量，否则第一次迭代成功后，再将该 closure 用于新的for循环时将会直接退出。

这里我们还是先详细的讲解一下 Lua 中泛型(for)的机制，之后再给出一个无状态迭代器的例子，以便于我们的理解。如果我们的迭代器实现为无状态迭代器，那么就不必为每一次的泛型(for)都重新声明一个新的迭代器变量了。
    
泛型(for)的语法如下：
    
    for <var-list> in <exp-list> do
        <body>
    end

为了便于理解，由于我们在实际应用中 < exp-list >通常只是包含一个表达式(expr)，因此简单起见，这里的说明将只是包含一个表达式，而不是表达式列表。现在我们先给出表达式的原型和实例，如：

	function ipairs2(a)
		return iter,a,0
	end

该函数返回3个值，第一个为实际的迭代器函数变量，第二个是一个恒定对象，这里我们可以理解为待遍历的容器，第三个变量是在调用 iter() 函数时为其传入的初始值。
    
下面我们再看一下 iter() 函数的实现，如：
    
	local function iter(a, i)
		i = i + 1
		local v = a[i]
		if v then
		    return i, v
		else
		    return nil, nil
		end
	end
    
在迭代器函数 iter() 中返回了两个值，分别对应于 table 的 key 和 value，其中 key(返回的i) 如果为nil，泛型(for)将会认为本次迭代已经结束。下面我们先看一下实际用例，如：

	function ipairs2(a)
		return iter,a,0
	end


	local function iter(a, i)
		i = i + 1
		local v = a[i]
		if v then
		    return i, v
		else
		    return nil, nil
		end
	end

	a = {"one","two","three"}
	for k,v in ipairs2(a) do
		print(k, v)
	end
	--输出结果为：
	--1       one
	--2       two
	--3       three

这个例子中的泛型(for)写法可以展开为下面的基于 while 循环的方式，如：

	local function iter(a, i)
		i = i + 1
		local v = a[i]
		if v then
		    return i, v
		else
		    return nil, nil
		end
	end

	function ipairs2(a)
		return iter,a,0
	end

	a = {"one","two","three"}
	do
		local _it,_s,_var = ipairs2(a)
		while true do
		    local var_1,var_2 = _it(_s,_var)
		    _var = var_1
		    if _var == nil then  --注意，这里只判断迭代器函数返回的第一个是否为nil。
		        break
		    end
		    print(var_1,var_2)
		end
	end
	--输出结果同上。

**无状态迭代器的例子**

这里的示例将实现遍历链表的迭代器。

	local function getnext(list, node)  --迭代器函数。
		if not node then
		    return list
		else
		    return node.next
		end
	end

	function traverse(list)  --泛型(for)的expression
		return getnext,list,nil
	end

	--初始化链表中的数据。
	list = nil
	for line in io.lines() do
		line = { val = line, next = list}
	end

	--以泛型(for)的形式遍历链表。
	for node in traverse(list) do
		print(node.val)
	end


这里使用的技巧是将链表的头结点作为恒定状态(traverse返回的第二个值)，而将当前节点作为控制变量。第一次调用迭代器函数 getnext() 时，node 为 nil，因此函数返回 list 作为第一个结点。在后续调用中 node 不再为 nil 了，所以迭代器返回 node.next，直到返回链表尾部的 nil 结点，此时泛型(for)将判断出迭代器的遍历已经结束。

最后需要说明的是，traverse() 函数和 list 变量可以反复的调用而无需再创建新的 closure 变量了。这主要是因为迭代器函数(getnext)实现为无状态迭代器。

**复杂状态的迭代器**

在上面介绍的迭代器实现中，迭代器需要保存许多状态，可是泛型 (for) 却只提供了恒定状态和控制变量用于状态的保存。一个最简单的办法是使用 closure。当然我们还以将所有的信息封装到一个 table 中，并作为恒定状态对象传递给迭代器。虽说恒定状态变量本身是恒定的，即在迭代过程中不会换成其它对象，但是该对象所包含的数据是否变化则完全取决于迭代器的实现。就目前而言，由于 table 类型的恒定对象已经包含了所有迭代器依赖的信息，那么迭代器就完全可以忽略泛型 (for) 提供的第二个参数。下面我们就给出一个这样的实例，见如下代码：

	local iterator
	function allwords()
		local state { line = io.read(), pos = 1 }
		return iterator, state
	end
	--iterator函数将是真正的迭代器
	function iterator(state)
		while state.line do
		    local s,e = string.find(state.line,"%w+",state.pos)
		    if s then
		        state.pos = e + 1
		        return string.sub(state.line,s,e)
		    else
		        state.line = io.read()
		        state.pos = 1
		    end
		end
		return nil
	end


###编译执行与错误

####编译：

Lua 中提供了 dofile 函数，它是一种内置的操作，用于运行Lua代码块。但实际上 dofile 只是一个辅助函数，loadfile 才是真正的核心函数。相比于dofile，loadfile 只是从指定的文件中加载Lua代码块，然后编译这段代码块，如果有编译错误，就返回nil，同时给出错误信息，但是在编译成功后并不真正的执行这段代码块。因此，
我们可以将 dofile 实现为：

	function dofile(filename)
	    local f = assert(loadfile(filename))
	    return f()
	end

这里如果 loadfile 执行失败，assert 函数将直接引发一个错误。通过 dofile 的代码，我们还可以看出，如果打算 多次运行一个文件中的 Lua 代码块，我们可以只执行 loadfile 一次，之后多次运行它返回的结果即可，这样就可以节省多次编译所带来的开销。这一点也是 loadfile 和 dofile 在性能上的区别。

Lua 中还提供了另外一种动态执行 Lua 代码的方式，即 loadstring 函数。顾名思义，相比于 loadfile，loadstring 的代码源来自于其参数中的字符串，如：

    f = loadstring("i = i + 1")
   
此时f就变成了一个函数，每次调用时就执行"i = i + 1"，如：

	1 i = 0
	2 f()
	3 print(i) --将输出1
	4 f()
	5 print(i) --将输出2
	
 loadstring 确实是一个功能强大的函数，但是由此而换来的性能开销也是我们不得不考虑的事情。所以对于很多常量字符串仍然使用loadstring方式，那就没有太大意义了，如上面的例子 f = loadstring("i = i + 1")，因为我们完全可以通过
    
    f = function () i = i + 1 end
    
的形式取而代之。而后者的执行效率要远远高于前者。毕竟后者只编译一次，而前者则在每次调用 loadstring 时均被编译。对于 loadstring，我们还需要注意的是，该函数总是在全局环境中编译它的字符串，因此它将无法访问文件局部变量，而是只能访问全局变量，如：
 
	i = 32
	local i = 0
	f = loadstring("i = i + 1; print(i)")
	g = function() i = i + 1; print(i) end
	f()   --f函数中的i为全局变量i，因此输出33
	g()   --g函数中的i为局部变量i，因此输出1


对于loadstring返回的函数，如果需要对一个表达式求值，则必须在其之前添加return，这样才能构成一条语句，返回表达式的
值，如：

	1 i = 32
	2 f = loadstring("i = i + 1; return i * 2")
	3 print(f()) --输出66
	4 print(f()) --输出68。由于loadstring返回的就是正规的函数，因此可以被反复调用。

Lua将所有独立的程序块视为一个匿名函数的函数体，并且该匿名函数还具有可变长实参，因此在调用loadstring时，可以为其传
递参数，如：

	1 local i = 30
	2 --下面的...表示变长实参，将值赋给局部变量x。
	3 local f = assert(loadstring("local x = ...; return (x + 10)    * 2")) 
	4 for i = 1, 20 do
	5     print(string.rep("*",f(i)))
	6 end

###C代码

上一小节介绍的是动态加载Lua代码，而事实上，Lua本身也支持动态加载C动态库中的代码，要完成该操作，我们需要借助于Lua
内置的系统函数 package.loadlib。该函数有两个字符串参数，分别是动态库的全文件名和该库包含的函数名称，典型的调用代码
如下：

    local path = "/usr/local/lib/test.so"
    local f = package.loadlib(path,"test_func")
    
由于loadlib是非常底层的函数，因为在调用时必须提供完整的路径名和函数名称。

###错误

Lua作为一种嵌入式脚本语言，在发生错误时，不应该只是简单的退出或崩溃。相反，一旦有错误发生，Lua就应该结束当前程序块并
返回到应用程序。

在Lua中我们可以通过 error() 函数获取错误消息，如：

    print "enter a number:"
    n = io.read("*number")
    if not n then error("invalid input") end
    
上面代码中的最后一行我们可以通过 Lua 提供的另外一个内置函数 assert 类辅助完成，如：

    print "enter a number:"
    n = assert(io.read("*number"),"invalid input")
    
assert 函数将检查其第一个参数是否为true，如果是，则简单的返回该参数，否则就引发一个错误。第二个参数是可选字符串。

对于所有的编程语言而言，错误处理都是一个非常重要的环节。在实际的开发中，没有统一的指导原则，只能是在遇到问题后，
经过缜密的分析在结合当时的应用场景，最后结合自己的经验再给出错误的具体处理方式。在有些情况下，我们可以直接返回错
误码，而在另外一些情况下，则需要直接抛出错误，让开发者能够快速定位导致错误的代码源。


####错误处理与异常

Lua 提供了错误处理函数 pcall，该函数的第一个参数为需要“保护执行”的函数，如果该函数执行失败，pcall 将返回 false 及
错误信息，否则返回 true 和函数调用的返回值。见如下代码：

	function foo()
		local a = 10
		print(a[2])
	end

	r, msg = pcall(foo)
	if r then
		print("This is ok.")
	else
		print("This is error.")
		print(msg)
	end
	--输出结果为：
	--This is error.
	--d:/test.lua:3: attempt to index local 'a' (a number value)


我们也可以给pcall函数直接传递匿名函数，如：

	r, msg = pcall(function() error({code = 121}) end)
	if r then
		print("This is ok.")
	else
		print("This is error.")
		print(msg.code)
	end
	--输出结果为：
	--This is error.
	--121

####错误消息与追溯

通常在错误发生时，希望得到更多的调试信息，而不是只有发生错误的位置。至少等追溯到发生错误时和函数调用情况，显示一个完整的函数调用栈轨迹。要完成这一功能，我们需要使用Lua提供的另外一个内置函数xpcall。该函数除了接受一个需要被调用的函数之外，还接受第二个参数，即错误处理函数。当发生错误时，Lua会在调用栈展开前调用错误处理函数。这样，我们就可以在这个函数中使用debug库的debug.traceback函数，它会根据调用栈来构建一个扩展的错误消息。如：


	function errorFunc()
		local a = 20
		print(a[10])
	end

	function errorHandle()
		print(debug.traceback())
	end

	if xpcall(errorFunc,errorHandle) then
		print("This is OK.")
	else
		print("This is error.")
	end

	--输出结果为：
	--[[stack traceback:
		    d:/test.lua:7: in function <d:/test.lua:6>
		    d:/test.lua:3: in function <d:/test.lua:1>
		    [C]: in function 'xpcall'
		    d:/test.lua:10: in main chunk
		    [C]: ?
	This is error.
	--]]


###数据结构

####数组

由于 table 的灵活性，Lua中的数组没有固定的大小，但默认索引是以 1 开始的。而且很多Lua的内置功能和函数都依赖这一特征，
因此在没有充分理由的前提下，尽量保证这一规则。

####序列化

	function serialize(o)
		if type(o) == "number" then
		    io.write(o)
		elseif type(o) == "string" then
		    --string.format函数的"%q"参数可以转义字符串中的元字符。
		    io.write(string.format("%q",o)) 
		elseif type(o) == "table" then
		    io.write("{\n")
		    --迭代table中的各个元素，同时递归的写出各个字段的value。
		    --由此可以看出，这个简单例子可以支持嵌套的table。
		    for k,v in pairs(o) do
		        --这样做是为了防止k中包含非法的Lua标识符。
		        io.write(" ["); serialize(k); io.write("] = ")
		        serialize(v)
		        io.write(",\n")
		    end
		    io.write("}\n")
		else
		    error("cannot serialize a " .. type(o))
		end
	end

###元表及元方法

Lua 中每个值都有一个元表。table和userdata可以有各自独立的元表，而其它数据类型的值则共享其类型
所属的单一元表。缺省情况下，table在创建时没有元表，如：

    t = {}
    print(getmetatable(t))  --输出为nil

这里我们可以使用setmetatable函数来设置或修改任何table的元表。

    t1 = {}
    setmetatable(t,t1)
    assert(getmetatable(t) == t1)
    
任何table都可以作为任何值的元表，而一组相关的table也可以共享一个通用的元表，此元表将描述了它们共同的行为。
一个table甚至可以作为它自己的元表，用于描述其特有的行为。在Lua代码中，只能设置table的元表，若要设置其它
类型值的元表，则必须通过C代码来完成。

###Lua 环境

Lua将其所有的全局变量保存在一个常规的table中，这个table被称为“环境”。它被保存在全局变量_G中。
   
####全局变量声明：
    
Lua中的全局变量不需要声明就可以使用。尽管很方便，但是一旦出现笔误就会造成难以发现的错误。我们可以通过给
_G 表加元表的方式来保护全局变量的读取和设置，这样就能降低这种笔误问题的发生几率了。见如下示例代码：
    
	--该table用于存储所有已经声明过的全局变量名
	local declaredNames = {} 
	local mt = {
		__newindex = function(table,name,value)
		    --先检查新的名字是否已经声明过，如果存在，这直接通过rawset函数设置即可。
		    if not declaredNames[name] then
		        --再检查本次操作是否是在主程序或者C代码中完成的，如果是，就继续设置，否则报错。
		        local w = debug.getinfo(2,"S").what
		        if w ~= "main" and w ~= "C" then
		            error("attempt to write to undeclared variable " .. name)
		        end
		        --在实际设置之前，更新一下declaredNames表，下次再设置时就无需检查了。
		        declaredNames[name] = true
		    end
		    print("Setting " .. name .. " to " .. value)
		    rawset(table,name,value)
		end,
		
		__index = function(_,name)
		    if not declaredNames[name] then
		        error("attempt to read undeclared variable " .. name)
		    else
		        return rawget(_,name)
		    end
		end
	}    
	setmetatable(_G,mt)

	a = 11
	local kk = aa

	--输出结果为：
	--[[
	Setting a to 11
	lua: d:/test.lua:21: attempt to read undeclared variable aa
	stack traceback:
		    [C]: in function 'error'
		    d:/test.lua:21: in function <d:/test.lua:19>
		    d:/test.lua:30: in main chunk
		    [C]: ?
	--]]


####非全局的环境

全局环境存在一个刚性的问题，即它的修改将影响到程序的所有部分。Lua 5为此做了一些改进，新的特征可以支持
每个函数拥有自己独立的全局环境，而由该函数创建的 closure 函数将继承该函数的全局变量表。这里我们可以通
过 setfenv 函数来改变一个函数的环境，该函数接受两个参数，一个是函数名，另一个是新的环境table。第一个
参数除了函数名本身，还可以指定为一个数字，以表示当前函数调用栈中的层数。数字 1 表示当前函数，2 表示它的
调用函数，以此类推。见如下代码：

	a = 1
	setfenv(1,{})
	print(a)

	--输出结果为：
	--[[
	lua: d:/test.lua:3: attempt to call global 'print' (a nil value)
	stack traceback:
		    d:/test.lua:3: in main chunk
		    [C]: ?
	--]]
	
为什么得到这样的结果呢？因为print和变量a一样，都是全局表中的字段，而新的全局表是空的，所以print调用将会报错。

为了应对这一副作用，我们可以让原有的全局表_G作为新全局表的内部表，在访问已有全局变量时，可以直接转到\_G中
的字段，而对于新的全局字段，则保留在新的全局表中。这样即便是函数中的误修改，也不会影响到其他用到全局变量(\_G)
的地方。见如下代码：

	a = 1
	local newgt = {}  --新环境表
	setmetatable(newgt,{__index = _G})
	setfenv(1,newgt)
	print(a)  --输出1

	a = 10
	print(a)  --输出10
	print(_G.a) --输出1
	_G.a = 20
	print(a)  --输出10
	
 最后给出的示例是函数环境变量的继承性。见如下代码：
 
	function factory()
		return function() return a end
	end
	a = 3
	f1 = factory()
	f2 = factory()
	print(f1())  --输出3
	print(f2())  --输出3

	setfenv(f1,{a = 10})
	print(f1())  --输出10
	print(f2())  --输出3

###模块和包

 从Lua 5.1开始，我们可以使用require和module函数来获取和创建Lua中的模块。从使用者的角度来看，
 一个模块就是一个程序库，可以通过require来加载，之后便得到一个类型为table的全局变量。此时的
 table就像名字空间一样，可以访问其中的函数和常量，如：
 
	1 require "mod"
	2 mod.foo()
	3 local m2 = require "mod2"
	4 local f = mod2.foo
	5 f()   
	
####require函数

require函数的调用形式为require "模块名"。该调用会返回一个由模块函数组成的table，并且还会定义
一个包含该table的全局变量。在使用Lua中的标准库时可以不用显示的调用require，因为Lua已经预先加载
了他们。

require函数在搜素加载模块时，有一套自定义的模式，如：

    ?;?.lua;c:/windows/?;/usr/local/lua/?/?.lua

在上面的模式中，只有问号(?)和分号(;)是模式字符，分别表示require函数的参数(模块名)和模式间的分隔符。
如：调用require "sql"，将会打开以下的文件：

    sql
    sql.lua
    c:/windows/sql
    /usr/local/lua/sql/sql.lua
   
Lua 将 require 搜索的模式字符串放在变量 package.path中。当Lua启动后，便以环境变量LUA\_PATH的值来
初始化这个变量。如果没有找到该环境变量，则使用一个编译时定义的默认路径来初始化。如果require无法找到
与模块名相符的Lua文件，就会找C程序库。C程序库的搜索模式存放在变量package.cpath中。而这个变量则是通
过环境变量LUA_CPATH来初始化的。


####编写模块的基本方法

见如下代码和关键性注释：

	--将模块名设置为require的参数，这样今后重命名模块时，只需重命名文件名即可。
	local modname = ...
	local M = {}
	_G[modname] = M

	M.i = {r = 0, i = 1}  --定义一个模块内的常量。
	function M.new(r,i) 
		return {r = r, i = i} 
	end
	
	function M.add(c1,c2) 
		return M.new(c1.r + c2.r,c1.i + c2.i)
	end

	function M.sub(c1,c2)
		return M.new(c1.r - c2.r,c1.i - c2.i)
	end
	--返回和模块对应的table。
	return M

使用环境

仔细阅读上例中的代码，我们可以发现一些细节上问题。比如模块内函数之间的调用仍然要
保留模块名的限定符，如果是私有变量还需要加local关键字，同时不能加模块名限定符。
如果需要将私有改为公有，或者反之，都需要一定的修改。那又该如何规避这些问题呢？
我们可以通过Lua的函数“全局环境”来有效的解决这些问题。见如下修改的代码和关键性注释：

	--模块设置和初始化。这一点和上例一致。
	local modname = ...
	local M = {}
	_G[modname] = M

	--声明这个模块将会用到的全局函数，因为在 setfenv 之后将无法再访问他们，
	--因此需要在设置之前先用本地变量获取。
	local sqrt = math.sqrt
	local io = io

	--在这句话之后就不再需要外部访问了。
	setfenv(1,M)

	--后面的函数和常量定义都无需模块限定符了。
	i = {r = 0, i = 1}
	
	function new(r,i) 
		return {r = r, i = i} 
	end
	
	function add(c1,c2) 
		return new(c1.r + c2.r,c1.i + c2.i)
	end
	 
	function sub(c1,c2)
		return new(c1.r - c2.r,c1.i - c2.i)
	end
	--返回和模块对应的table。
	return M

####module函数：
    
在Lua 5.1中，我们可以用module(...)函数来代替以下代码，如：
    
	local modname = ...
	local M = {}
	_G[modname] = M
	package.loaded[modname] = M
		--[[
		和普通Lua程序块一样声明外部函数。
		--]]
	setfenv(1,M)
	
由于在默认情况下，module不提供外部访问，必须在调用它之前，为需要访问的外部函数或模块声明适当的
局部变量。然后Lua提供了一种更为方便的实现方式，即在调用module函数时，多传入一个package.seeall
的参数，如：

    module(...,package.seeall)


###面向对象

Lua中的table就是一种对象，但是如果直接使用仍然会存在大量的问题，见如下代码：

	1 Account = {balance = 0}
	2 function Account.withdraw(v)
	3     Account.balance = Account.balance - v
	4 end
	5 --下面是测试调用函数
	6 Account.withdraw(100.00)

在上面的withdraw函数内部依赖了全局变量Account，一旦该变量发生改变，将会导致withdraw不再能正常的
工作，如：

	1 a = Account; Account = nil
	2 a.withdraw(100.00)  --将会导致访问空nil的错误。

 这种行为明显的违反了面向对象封装性和实例独立性。要解决这一问题，我们需要给withdraw函数在添加一
 个参数self，他等价于C++中的this，见如下修改：
 

	1 function Account.withdraw(self,v)
	2     self.balance = self.balance - v
	3 end
	4 --下面是基于修改后代码的调用：
	5 a1 = Account; Account = nil
	6 a1.withdraw(a1,100.00)  --正常工作。

针对上述问题，Lua提供了一种更为便利的语法，即将点(.)替换为冒号(:)，这样可以在定义和调用时均隐藏
self 参数，如:

	1 function Account:withdraw(v)
	2     self.balance = self.balance - v
	3 end
	4 --调用代码可改为：
	5 a:withdraw(100.00)

####类

Lua 在语言上并没有提供面向对象的支持，因此想实现该功能，我们只能通过 table 来模拟，见如下代码及关键
性注释：

	--这里balance是一个公有的成员变量。
	Account = {balance = 0}

	--new可以视为构造函数，几乎所有的类都是如此 new 的
	function Account:new(o)
		o = o or {} --如果参数中没有提供table，则创建一个空的。
		--将新对象实例的metatable指向Account表(类)，这样就可以将其视为模板了。
		setmetatable(o,self)
		--在将Account的__index字段指向自己，以便新对象在访问Account的函数和字段时，可被直接重定向。
		self.__index = self
		--最后返回构造后的对象实例
		return o
	end

	--deposite被视为Account类的公有成员函数
	function Account:deposit(v)
		--这里的self表示对象实例本身
		self.balance = self.balance + v
	end

	--下面的代码创建两个Account的对象实例

	--通过Account的new方法构造基于该类的示例对象。
	a = Account:new()
	--[[
	这里需要具体解释一下，此时由于table a中并没有deposite字段，因此需要重定向到Account，
	同时调用Account的deposite方法。在Account.deposite方法中，由于self(a对象)并没有balance
	字段，因此在执行self.balance + v时，也需要重定向访问Account中的balance字段，其缺省值为0。
	在得到计算结果后，再将该结果直接赋值给a.balance。此后a对象就拥有了自己的balance字段和值。
	下次再调用该方法，balance字段的值将完全来自于a对象，而无需在重定向到Account了。
	--]]
	a:deposit(100.00)
	print(a.balance) --输出100

	b = Account:new()
	b:deposit(200.00)
	print(b.balance) --输出200

####继承

继承也是面向对象中一个非常重要的概念，在Lua中我们仍然可以像模拟类那样来进一步实现面向对象中的
继承机制，见如下代码及关键性注释：


	--需要说明的是，这段代码仅提供和继承相关的注释，和类相关的注释在上面的代码中已经给出。
	Account = {balance = 0}

	function Account:new(o)
		o = o or {}
		setmetatable(o,self)
		self.__index = self
		return o
	end

	function Account:deposit(v)
		self.balance = self.balance + v
	end

	function Account:withdraw(v)
		if v > self.balance then
		    error("Insufficient funds")
		end
		self.balance = self.balance - v
	end

	--下面将派生出一个Account的子类，以使客户可以实现透支的功能。
	SpecialAccount = Account:new()  --此时SpecialAccount仍然为Account的一个对象实例

	--派生类SpecialAccount扩展出的方法。
	--下面这些SpecialAccount中的方法代码(getLimit/withdraw)，一定要位于SpecialAccount被Account构造之后。
	function SpecialAccount:getLimit()
		--此时的self将为对象实例。
		return self.limit or 0
	end

	--SpecialAccount将为Account的子类，下面的方法withdraw可以视为SpecialAccount
	--重写的Account中的withdraw方法，以实现自定义的功能。
	function SpecialAccount:withdraw(v)
		--此时的self将为对象实例。
		if v - self.balance >= self:getLimit() then
		    error("Insufficient funds")
		end
		self.balance = self.balance - v
	end

	--在执行下面的new方法时，table s的元表已经是SpecialAccount了，而不再是Account。
	s = SpecialAccount:new{limit = 1000.00}
	--在调用下面的deposit方法时，由于table s和SpecialAccount均未提供该方法，因此访问的仍然是
	--Account的deposit方法。
	s:deposit(100)


	--此时的withdraw方法将不再是Account中的withdraw方法，而是SpecialAccount中的该方法。
	--这是因为Lua先在SpecialAccount(即s的元表)中找到了该方法。
	s:withdraw(200.00)
	print(s.balance) --输出-100


####私密性：

私密性对于面向对象语言来说是不可或缺的，否则将直接破坏对象的封装性。Lua作为一种面向过程的脚本语言，
更是没有提供这样的功能，然而和模拟支持类与继承一样，我们仍然可以在Lua中通过特殊的编程技巧来实现它，
这里我们应用的是Lua中的闭包函数。该实现方式和前面两个示例中基于元表的方式有着很大的区别，见如下代
码示例和关键性注释：

	--这里我们需要一个闭包函数作为类的创建工厂
	function newAccount(initialBalance)
		--这里的self仅仅是一个普通的局部变量，其含义完全不同于前面示例中的self。
		--这里之所以使用self作为局部变量名，也是为了方便今后的移植。比如，以后
		--如果改为上面的实现方式，这里应用了self就可以降低修改的工作量了。
		local self = {balance = initialBalance} --这里我们可以将self视为私有成员变量
		local withdraw = function(v) self.balance = self.balance - v end
		local deposit = function(v) self.balance = self.balance + v end
		local getBalance = function() return self.balance end
		--返回对象中包含的字段仅仅为公有方法。事实上，我们通过该种方式，不仅可以实现
		--成员变量的私有性，也可以实现方法的私有性，如：
		--local privateFunction = function() --do something end
		--只要我们不在输出对象中包含该方法的字段即可。
		return {withdraw = withdraw, deposit = deposit, getBalance = getBalance}
	end

	--和前面两个示例不同的是，在调用对象方法时，不再需要self变量，因此我们可以直接使用点(.)，
	--而不再需要使用冒号(:)操作符了。
	accl = newAccount(100.00)
	--在函数newAccount返回之后，该函数内的“非局部变量”表self就不再能被外部访问了，只能通过
	--该函数返回的对象的方法来操作它们。
	accl.withdraw(40.00)
	print(acc1.getBalance())
	

###弱引用table

Lua采用了基于垃圾收集的内存管理机制，因此对于程序员来说，在很多时候内存问题都将不再困扰他们。
然而任何垃圾收集器都不是万能的，在有些特殊情况下，垃圾收集器是无法准确的判断是否应该将当前对
象清理。这样就极有可能导致很多垃圾对象无法被释放。为了解决这一问题，就需要Lua的开发者予以一定
程度上的配合。比如，当某个table对象被存放在容器中，而容器的外部不再有任何变量引用该对象，对于
这样的对象，Lua的垃圾收集器是不会清理的，因为容器对象仍然引用着他。如果此时针对该容器的应用仅
限于查找，而不是遍历的话，那么该对象将永远不会被用到。事实上，对于这样的对象我们是希望Lua的垃
圾收集器可以将其清理掉的。见如下代码：

	a = {}
	key = {}
	a[key] = 1
	key = {}
	a[key] = 2
	collectgarbage()
	for k,v in pairs(a) do
		print(v)
	end    
	--输出1和2
	
在执行垃圾收集之后，table a中的两个key都无法被清理，但是对value等于1的key而言，如果后面的逻辑不
会遍历table a的话，那么我们就可以认为该对象内存泄露了。在Lua中提供了一种被称为弱引用table的机制，
可以提示垃圾收集器，如果某个对象，如上面代码中的第一个table key，只是被弱引用table引用，那么在执
行垃圾收集时可以将其清理。

Lua中的弱引用表提供了3中弱引用模式，即key是弱引用、value是弱引用，以及key和value均是弱引用。不论
是哪种类型的弱引用table，只要有一个key或value被回收，那么它们所在的整个条目都会从table中删除。

一个table的弱引用类型是通过其元表的__mode字段来决定的。如果该值为包含字符"k"，那么table就是key弱
引用，如果包含"v"，则是value若引用，如果两个字符均存在，就是key/value弱引用。见如下代码：

	a = {}
	b = {__mode = "k"}
	setmetatable(a,b)
	key = {}
	a[key] = 1
	key = {}
	a[key] = 2
	collectgarbage()
	for k,v in pairs(a) do
		print(v)
	end    
	--仅仅输出2
	
 在上面的代码示例中，第一个key在被存放到table a之后，就被第二个key的定义所覆盖，因此它的唯一引用来自
 key弱引用表。
 
最后需要说明的是，Lua中的弱引用表只是作用于 table 类型的变量，对于其他类型的变量，如数值和字符串等，弱引
用表并不起任何作用。

####备忘录(memoize)函数

用“空间换时间”是一种通用的程序运行效率优化手段，比如：对于一个普通的 Server，它接受到的请求中包含 Lua 代码，
每当其收到请求后都会调用Lua的loadstring函数来动态解析请求中的Lua代码，如果这种操作过于频率，就会导致 Server
的执行效率下降。要解决该问题，我们可以将每次解析的结果缓存到一个 tabl e中，下次如果接收到相同的Lua代码，就不
需要调用loadstirng来动态解析了，而是直接从 table 中获取解析后的函数直接执行即可。这样在有大量重复Lua代码的情
况下，可以极大的提高 Server 的执行效率。反之，如果有相当一部分的Lua代码只是出现一次，那么再使用这种机制，就将
会导致大量的内存资源被占用而得不到有效的释放。在这种情况下，如果使用弱引用表，不仅可以在一定程度上提升程序的
运行效率，内存资源也会得到有效的释放。见如下代码：

	local results = {}
	setmetatable(results,{__mode = "v"}) --results表中的key是字符串形式的Lua代码
	function mem_loadstring(s)
		local res = results[s]
		if res == nil then
		    res = assert(loadstring(s))
		    results[s] = res
		end
		return res
	end


###字符串库

####基础字符串函数：

字符串库中有一些函数非常简单，如：

* string.len(s) 返回字符串s的长度；
* string.rep(s,n) 返回字符串s重复n次的结果；
* string.lower(s) 返回s的副本，其中所有的大写都被转换为了小写形式，其他字符不变；
* string.upper(s) 和lower相反，将小写转换为大写；
* string.sub(s,i,j) 提取字符串s的第i个到第j个字符。Lua中，第一个字符的索引值为1，最后一个为-1，以此类推，如：
    
    print(string.sub("[hello world]",2,-2))      --输出hello world
    
* string.format(s,...) 返回格式化后的字符串，其格式化规则等同于C语言中printf函数，如：
    
    print(string.format("pi = %.4f",math.pi)) --输出pi = 3.1416

* string.char(...) 参数为0到多个整数，并将每个整数转换为对应的字符。然后返回一个由这些字符连接而成的字符串，如：
    
    print(string.char(97,98,99)) --输出abc

* string.byte(s,i) 返回字符串s的第i个字符的Ascii值，如果没有第二个参数，缺省返回第一个字符的Ascii值。
    
    print(string.byte("abc"))      --输出97
    print(string.byte("abc",-1))  --输出99
    
由于字符串类型的变量都是不可变类型的变量，因此在所有和string相关的函数中，都无法改变参数中的字符串值，而是生成一个新值返回。


####模式匹配函数

Lua的字符串库提供了一组强大的模式匹配函数，如find、match、gsub和gmatch。

**string.find()**

在目标字符串中搜索一个模式，如果找到，则返回匹配的起始索引和结束索引，否则返回nil。如：

	1 s = "hello world"
	2 i, j = string.find(s,"hello")  
	3 print(i, j)        --输出1  5
	4 i, j = string.find(s,"l")
	5 print(i, j)        --输出3  3
	6 print(string.find(s,"lll"))  --输出nil

 string.find 函数还有一个可选参数，它是一个索引，用于告诉函数从目标字符串的哪个位置开始搜索。主要用于搜索目标字符串中所有匹配的子字符串，且每次搜索都从上一次找到的位置开始。如：
 
	local t = {}
	local i = 0
	while true do
		i = string.find(s,"\n",i+1)
		if i == nil then
		    break
		end
		t[#t + 1] = i
	end

**string.match()**
    
该函数返回目标字符串中和模式字符串匹配的部分。如：

	1 date = "Today is 2012-01-01"
	2 d = string.match(date,"%d+\-%d+\-%d+")
	3 print(d)  --输出2012-01-01

**string.gsub()**

该函数有3个参数，目标字符串、模式和替换字符串。基本用法是将目标字符串中所有出现模式的地方替换为替换字符串。如：
    
    print(string.gsub("Lua is cute","cute","great"))  --输出Lua is great

该函数还有可选的第4个参数，即实际替换的次数。
    
    print(string.gsub("all lii","l","x",1))  --输出axl lii
    print(string.gsub("all lii","l","x",2))  --输出axx lii

函数string.gsub还有另一个结果，即实际替换的次数。
    
    count = select(2, string.gsub(str," "," "))  --输出str中空格的数量

string.gmatch()

返回一个函数，通过这个返回的函数可以遍历到一个字符串中所有出现指定模式的地方。如：

	words = {}
	s = "hello world"
	for w in string.gmatch(s,"%a+") do
		print(w)
		words[#words + 1] = w
	end
	--输出结果为：
	--hello
	--world
	
**模式**

下面的列表给出了Lua目前支持的模式元字符；

		模式元字符 	描述
		. 			所有字符
		%a 			字母
		%c 			控制字符
		%d 			数字
		%l 			小写字母
		%p 			标点符号
		%s 			空白字符
		%u 			大写字母
		%w 			字母和数字字符
		%x 			十六进制数字
		%z 			内部表示为0的字符

这些元字符的大写形式表示它们的补集，如%A，表示所有非字母字符。
    
    print(string.gsub("hello, up-down!","%S","."))   --输出hello..up.down. 4
    
上例中的4表示替换的次数。

除了上述元字符之外，Lua还提供了另外几个关键字符。如：( ) . % + - * ? [ ] ^ $
    
* %  表示转义字符，如%.表示点(.)，%%表示百分号(%)。
* [] 表示将不同的字符分类，即可创建出属于自己的字符分类，如[%w_]表示匹配字符、数字和下划线。
* -  表示连接一个范围，比如[0-9A-Z]
* ^  如果^字符在方括号内，如[^\n]，表示除\n之外的所有字符，即表示方括号中的分类的补集。
	 如果^不在方括号内，则表示以后面的字符开头，
	
* $  与 ^ 正好相反，表示以前面的字符结束。如：^Hello%d$，匹配的字符串可能为Hello1、Hello2等。
    
在Lua中还提供了4种用来修饰模式中的重复部分，如：
	
	+ 重复1次或多次
	* 重复0次或多次，贪婪
	- 重复0次或多次，非贪婪
	? 出现0或1次)

如：

    print(string.gsub("one, and two; and three","%a+","word")) --输出word, word word; word word
    print(string.match("the number 1298 is even","%d+")) --输出1298
   
星号(*)和横线(-)的主要差别是，星号总是试图匹配更多的字符，而横线则总是试图匹配最少的字符。

####捕获(capture)

捕获功能可根据一个模式从目标字符串中抽出匹配于该模式的内容。在指定捕获是，应将模式中需要捕获的部分
写到一对圆括号内。对于具有捕获的模式，函数string.match会将所有捕获到的值作为单独的结果返回。即它会
将目标字符串切成多个捕获到的部分。如：
    
	pair = "name = Anna"
	key,value = string.match(pair,"(%a+)%s*=%s*(%a+)")
	print(key,value)  --输出name anna

	date = "Today is 2012-01-02"
	y,m,d = string.match(date,"(%d+)\-(%d+)\-(%d+)")
	print(y,m,d)      --输出2012    01      02

还可以对模式本身使用捕获。即%1表示第一个捕获，以此类推，%0表示整个匹配，如：

1 print(string.gsub("hello Lua","(.)(.)","%2%1"))  --将相邻的两个字符对调，输出为ehll ouLa
2 print(string.gsub("hello Lua!","%a","%0-%0"))    --输出为h-he-el-ll-lo-o L-Lu-ua-a!

####替换
    
string.gsub函数的第三个参数不仅可以是字符串，也可以是函数或table，如果是函数，string.gsub会在
每次找到匹配时调用该函数，调用时的参数就是捕获到的内容，而该函数的返回值则作为要替换的字符串。
当用一个table来调用时，string.gsub会用每次捕获到的内容作为key，在table中查找，并将对应的 value
作为要替换的字符串。如果table中不包含这个key，那么string.gsub不改变这个匹配。如：

	function expand(s)
		return (string.gsub(s,"$(%w+)",_G))
	end

	name = "Lua"; status = "great"
	print(expand("$name is $status, isn't it?"))  --输出 Lua is great, isn't it?
	print(expand("$othername is $status, isn't it?"))  --输出 $othername is great, isn't it?

	function expand2(s)
		return (string.gsub(s,"$(%w+)",function(n) return tostring(_G[n]) end))
	end

	print(expand2("print = $print; a = $a")) --输出 print = function: 002B77C0; a = nil
	

###输入输出库

 I/O库为文件操作提供了两种不同的模型，简单模型和完整模型。简单模型假设一个当前输入文件和一个当前输出
 文件，他的I/O操作均作用于这些文件。完整模型则使用显式的文件句柄，并将所有的操作定义为文件句柄上的方法。

####简单模型：

I/O库会将进程标准输入输出作为其缺省的输入文件和输出文件。我们可以通过io.input(filename)和
io.output(filename)这两个函数来改变当前的输入输出文件。

**io.write()**

函数原型为io.write(...)。该函数将所有参数顺序的写入到当前输出文件中。如：
    
    io.write("hello","world") --写出的内容为helloworld

**io.read()**
    
下表给出了该函数参数的定义和功能描述：

		参数字符串 		含义
		"*all" 		读取整个文件
		"*line" 	读取下一行
		"*number" 	读取一个数字
		<num> 		读取一个不超过<num>个字符的字符串

调用io.read("*all")会读取当前输入文件的所有内容，以当前位置作为开始。如果当前位置处于文件的末尾，
或者文件为空，那个该调用返回一个空字符串。由于Lua可以高效的处理长字符串，因此在Lua中可以先将数据
从文件中完整读出，之后在通过Lua字符串库提供的函数进行各种处理。

调用io.read("*line")会返回当前文件的下一行，但不包含换行符。当到达文件末尾时，该调用返回nil。如：

	for count = 1,math.huge do
		local line = io.read("*line")  --如果不传参数，缺省值也是"*line"
		if line == nil then 
		    break
		end
		io.write(string.format("%6d  ",count),line,"\n")
	end
	
如果只是为了迭代文件中的所有行，可以使用io.lines函数，以迭代器的形式访问文件中的每一行数据，如：

	local lines = {}
	for line in io.lines() do   --通过迭代器访问每一个数据
		lines[#lines + 1] = line
	end
	table.sort(lines)  --排序，Lua标准库的table库提供的函数。
	for _,l in ipairs(lines) do
		io.write(l,"\n")
	end
	
调用io.read("\*number")会从当前输入文件中读取一个数字。此时read将直接返回一个数字，而不是字符串。
"*number"选项会忽略数字前面所有的空格，并且能处理像-3、+5.2这样的数字格式。如果当前读取的数据不是
合法的数字，read返回nil。在调用read是可以指定多个选项，函数会根据每个选项参数返回相应的内容。如：

	--[[
		6.0  -3.23   1000
		... ...
		下面的代码读取注释中的数字
	--]]
	while true do
		local n1,n2,n3 = io.read("*number","*number","*number")
		if not n1 then
		    break
		end
		print(math.max(n1,n2,n3))
	end
	
调用io.read(<num>)会从输入文件中最多读取n个字符，如果读不到任何字符，返回nil。否则返回读取到的字符串。如：

	while true do
		local block = io.read(2^13)
		if not block then
		    break
		end
		io.write(block)
	end

io.read(0)是一种特殊的情况，用于检查是否到达了文件的末尾。如果没有到达，返回空字符串，否则nil。

####完整I/O模型

Lua中完整I/O模型的使用方式非常类似于C运行时库的文件操作函数，它们都是基于文件句柄的。
    
通过io.open函数打开指定的文件，并且在参数中给出对该文件的打开模式，其中"r"表示读取，"w"表示覆盖写入，即先
删除文件原有的内容，"a"表示追加式写入，"b"表示以二进制的方式打开文件。在成功打开文件后，该函数将返回表示该
文件的句柄，后面所有基于该文件的操作，都需要将该句柄作为参数传入。如果打开失败，返回nil。其中错误信息由该函
数的第二个参数返回，如：

    assert(io.open(filename,mode))  --如果打开失败，assert将打印第二个参数给出的错误信息。


文件读写函数read/write。这里需要用到冒号语法，如：

	1 local f = assert(io.open(filename,"r"))
	2 local t = f:read("*all")  --对于read而言，其参数完全等同于简单模型下read的参数。
	3 f:close()

此外，I/O库还提供了3个预定义的文件句柄，即io.stdin(标准输入)、io.stdout(标准输出)、io.stderr(标准错误输出)。如：
    
    io.stderr:write("This is an error message.")
    
事实上，我们也可以混合使用简单模式和完整模式，如：

	1 local temp = io.input()   --将当前文件句柄保存
	2 io.input("newInputfile")  --打开新的输入文件
	3 io.input():close()        --关闭当前文件
	4 io.input(temp)            --恢复原来的输入文件

####性能小技巧

由于一次性读取整个文件比逐行读取要快一些，但对于较大的文件，这样并不可行，因此Lua提供了一种折中
的方式，即一次读取指定字节数量的数据，如果当前读取中的最后一行不是完整的一行，可通过该方式将该行
的剩余部分也一并读入，从而保证本次读取的数据均为整行数据，以便于上层逻辑的处理。如：

    local lines,rest = f:read(BUFSIZE,"*line") --rest变量包含最后一行中没有读取的部分。
    
下面是Shell中wc命令的一个简单实现。

	local BUFSIZE = 8192
	local f = io.input(arg[1])  --打开输入文件
	local cc, lc, wc, = 0, 0, 0  --分别计数字符、行和单词    
	while true do
		local lines,rest = f:read(BUFSIZE,"*line")
		if not lines then
		    break
		end
		if rest then
		    lines = lines .. rest .. "\n"
		end
		cc = cc + #lines
		--计算单词数量
		local _, t = string.gsub(lines."%S+","")
		wc = wc + t
		--计算行数
		_,t = string.gsub(line,"\n","\n")
		lc = lc + t
	end
	print(lc,wc,cc)

####其它文件操作

如io.flush函数会将io缓存中的数据刷新到磁盘文件上。io.close函数将关闭当前打开的文件。io.seek
函数用于设置或获取当前文件的读写位置，其函数原型为f:seek(whence,offset)，如果whence的值为"set"，
offset的值则表示为相对于文件起始位置的偏移量。如为"cur"(默认值),offset则为相对于当前位置的偏移量，
如为"end"，则为相对于文件末尾的偏移量。函数的返回值与whence参数无关，总是返回文件的当前位置，即相
对于文件起始处的偏移字节数。offset的默认值为0。如：

1 function fsize(file)
2     local current = file:seek()   --获取当前位置
3     local size = file:seek("end") --获取文件大小
4     file:seek("set",current)      --恢复原有的当前位置
5     return size
6 end

最后需要指出的是，如果发生错误，所有这些函数均返回nil和一条错误信息。


###系统库

Lua为了保证高度的可移植性，因此，它的标准库仅仅提供了非常少的功能，特别是和OS相关的库。但是Lua还提供了
一些扩展库，比如Posix库等。对于文件操作而言，该库仅提供了os.rename函数和os.remove函数。

####日期和时间：

在Lua中，函数time和date提供了所有的日期和时间功能。

如果不带任何参数调用time函数，它将以数字形式返回当前的日期和时间。如果以一个table作为参数，它将返回一个
数字，表示该table中所描述的日期和时间。该table的有效字段如下：

	字段名 	描述
	year 	一个完整的年份
	month 	01-12
	day 	01-31
	hour 	00-23
	min 	00-59
	sec 	00-59
	isdst 	布尔值，true表示夏令时

print(os.time{year = 1970, month = 1, day = 1, hour = 8, min = 0}) --北京是东八区，所以hour等于时表示UTC的0。

print(os.time())  --输出当前时间距离1970-1-1 00:00:00所经过的秒数。输出值为 1333594721

函数date是time的反函数，即可以将time返回的数字值转换为更高级的可读格式，其第一个参数是格式化字符串，
表示期望的日期返回格式，第二个参数是日期和时间的数字，缺省为当前日期和时间。如：

	dd = os.date("*t",os.time())  --如果格式化字符串为"*t"，函数将返回table形式的日期对象。如果为"!*t"，则表示为UTC时间格式。
	print("year = " .. dd.year)
	print("month = " .. dd.month)
	print("day = " .. dd.day)
	print("weekday = " .. dd.wday)  --一个星期中的第几天，周日是第一天
	print("yearday = " .. dd.yday)  --一年中的第几天，1月1日是第一天
	print("hour = " .. dd.hour)
	print("min = " .. dd.min)
	print("sec = " .. dd.sec)
		
	--[[
	year = 2012
	month = 4
	day = 5
	weekday = 5
	yearday = 96
	hour = 11
	min = 13
	sec = 44
	--]]
	
date函数的格式化标识和C运行时库中的strftime函数的标识完全相同，见下表：

	关键字 		描述
	%a 		一星期中天数的缩写，如Wed
	%A 		一星期中天数的全称，如Friday
	%b 		月份的缩写，如Sep
	%B 		月份的全称，如September
	%c 		日期和时间
	%d 		一个月中的第几天(01-31)
	%H 		24小时制中的小时数(00-23)
	%I 		12小时制中的小时数(01-12)
	%j 		一年中的第几天(001-366)
	%M 		分钟(00-59)
	%m 		月份(01-12)
	%p 		"上午(am)"或"下午(pm)"
	%S 		秒数(00-59)
	%w 		一星期中的第几天(0--6等价于星期日--星期六)
	%x 		日期，如09/16/2010
	%X 		时间，如23:48:20
	%y 		两位数的年份(00-99)
	%Y 		完整的年份(2012)
	%% 		字符'%'

    print(os.date("%Y-%m-%d"))  --输出2012-04-05
    函数os.clock()返回CPU时间的描述，通常用于计算一段代码的执行效率。如：

 print(os.date("%Y-%m-%d"))  --输出2012-04-05
    
 函数os.clock()返回CPU时间的描述，通常用于计算一段代码的执行效率。如：

	local x = os.clock()
	local s = 0
	for i = 1, 10000000 do 
		s = s + i 
	end
	print(string.format("elapsed time: %.2f\n", os.clock() - x))

	--输出结果为：
	--elapsed time: 0.21
	
####其他系统调用：

函数os.exit()可中止当前程序的执行。

函数os.getenv()可获取一个环境变量的值。如：

    print(os.getenv("PATH"))  --如果环境变量不存在，返回nil。
    
os.execute函数用于执行和操作系统相关的命令，如：

	os.execute("mkdir " .. "hello")


###Lua C API

Lua是一种嵌入式脚本语言，即Lua不是可以单独运行的程序，在实际应用中，主要存在两种应用形式。
第一种形式是，C/C++作为主程序，调用Lua代码，此时可以将Lua看做“可扩展的语言”，我们将这种应
用称为“应用程序代码”。第二种形式是Lua具有控制权，而C/C++代码则作为Lua的“库代码”。在这两种
形式中，都是通过Lua提供的C API完成两种语言之间的通信的。

####基础知识：
   
C API是一组能使C/C++代码与Lua交互的函数。其中包括读写Lua全局变量、调用Lua函数、运行一段
Lua代码，以及注册C函数以供Lua代码调用等。这里先给出一个简单的示例代码：
 
	#include <stdio.h>
	#include <string.h>
	#include <lua.hpp>
	#include <lauxlib.h>
	#include <lualib.h>

	int main(void)
	{
		const char* buff = "print(\"hello\")";
		int error;
		lua_State* L = luaL_newstate();
		luaL_openlibs(L);

		error = luaL_loadbuffer(L,buff,strlen(buff),"line") || lua_pcall(L,0,0,0);
		int s = lua_gettop(L);
		if (error) {
		    fprintf(stderr,"%s",lua_tostring(L,-1));
		    lua_pop(L,1);
		}
		lua_close(L);
		return 0;
	}
 

下面是针对以上代码给出的具体解释：

上面的代码是基于我的C++工程，而非C工程，因此包含的头文件是lua.hpp，如果是C工程，可以直接包含lua.h。

* Lua库中没有定义任何全局变量，而是将所有的状态都保存在动态结构lua_State中，后面所有的C API都需要该指针作为第一个参数。
* luaL_openlibs函数是用于打开Lua中的所有标准库，如io库、string库等。
* luaL_loadbuffer编译了buff中的Lua代码，如果没有错误，则返回0，同时将编译后的程序块压入虚拟栈中。
* lua_pcall函数会将程序块从栈中弹出，并在保护模式下运行该程序块。执行成功返回0，否则将错误信息压入栈中。
* lua_tostring函数中的-1，表示栈顶的索引值，栈底的索引值为1，以此类推。该函数将返回栈顶的错误信息，但是不会将其从栈中弹出。
* lua_pop是一个宏，用于从虚拟栈中弹出指定数量的元素，这里的1表示仅弹出栈顶的元素。
* lua_close用于释放状态指针所引用的资源。
 
####栈

在Lua和C语言之间进行数据交换时，由于两种语言之间有着较大的差异，比如Lua是动态类型，C语言是静态类型，
Lua是自动内存管理，而C语言则是手动内存管理。为了解决这些问题，Lua的设计者使用了虚拟栈作为二者之间数
据交互的介质。在C/C++程序中，如果要获取Lua的值，只需调用Lua的C API函数，Lua就会将指定的值压入栈中。
要将一个值传给Lua时，需要先将该值压入栈，然后调用Lua的C API，Lua就会获取该值并将其从栈中弹出。为了
可以将不同类型的值压入栈，以及从栈中取出不同类型的值，Lua为每种类型均设定了一个特定函数。

**压入元素**

Lua针对每种C类型，都有一个C API函数与之对应，如：

    void lua_pushnil(lua_State* L);  --nil值
    void lua_pushboolean(lua_State* L, int b); --布尔值
    void lua_pushnumber(lua_State* L, lua_Number n); --浮点数
    void lua_pushinteger(lua_State* L, lua_Integer n);  --整型
    void lua_pushlstring(lua_State* L, const char* s, size_t len); --指定长度的内存数据
    void lua_pushstring(lua_State* L, const char* s);  --以零结尾的字符串，其长度可由strlen得出。

对于字符串数据，Lua不会持有他们的指针，而是调用在API时生成一个内部副本，因此，即使在这些函数返回后立刻释放
或修改这些字符串指针，也不会有任何问题。
   
在向栈中压入数据时，可以通过调用下面的函数判断是否有足够的栈空间可用，一般而言，Lua会预留20个槽位，对于普通
应用来说已经足够了，除非是遇到有很多参数的函数。

    int lua_checkstack(lua_State* L, int extra) --期望得到extra数量的空闲槽位，如果不能扩展并获得，返回false。
    
**查询元素**


API使用“索引”来引用栈中的元素，第一个压入栈的为1，第二个为2，依此类推。我们也可以使用负数作为索引值，其中-1表示
为栈顶元素，-2为栈顶下面的元素，同样依此类推。

Lua提供了一组特定的函数用于检查返回元素的类型，如：

    int lua_isboolean (lua_State *L, int index);
    int lua_iscfunction (lua_State *L, int index);
    int lua_isfunction (lua_State *L, int index);
    int lua_isnil (lua_State *L, int index);
    int lua_islightuserdata (lua_State *L, int index);
    int lua_isnumber (lua_State *L, int index);
    int lua_isstring (lua_State *L, int index);
    int lua_istable (lua_State *L, int index);
    int lua_isuserdata (lua_State *L, int index);
    
以上函数，成功返回1，否则返回0。需要特别指出的是，对于lua_isnumber而言，不会检查值是否为数字类型，而是检查值是否
能转换为数字类型。

Lua还提供了一个函数lua_type，用于获取元素的类型，函数原型如下：
    
    int lua_type (lua_State *L, int index);
    
该函数的返回值为一组常量值，分别是：
    
    LUA_TNIL
    LUA_TNUMBER
    LUA_TBOOLEAN
    LUA_TSTRING
    LUA_TTABLE
    LUA_TFUNCTION
    LUA_TUSERDATA
    LUA_TTHREAD
    LUA_TLIGHTUSERDATA。

这些常量通常用于switch语句中。

除了上述函数之外，Lua还提供了一组转换函数，如：

    int lua_toboolean (lua_State *L, int index);
    lua_CFunction lua_tocfunction (lua_State *L, int index);
    lua_Integer lua_tointeger (lua_State *L, int index);    
    const char *lua_tolstring (lua_State *L, int index, size_t *len);
    lua_Number lua_tonumber (lua_State *L, int index);
    const void *lua_topointer (lua_State *L, int index);
    const char *lua_tostring (lua_State *L, int index);
    void *lua_touserdata (lua_State *L, int index);

    --string类型返回字符串长度，table类型返回操作符'#'等同的结果，userdata类型返回分配的内存块长度。
    size_t lua_objlen (lua_State *L, int index);　

对于上述函数，如果调用失败，lua_toboolean、lua_tonumber、lua_tointeger和lua_objlen均返回0，而其他函数
则返回NULL。在很多时候0不是一个很有效的用于判断错误的值，但是ANSI C没有提供其他可以表示错误的值。因此对
于这些函数，在有些情况下需要先使用lua_is*系列函数判断是否类型正确，而对于剩下的函数，则可以直接通过判断
返回值是否为NULL即可。

对于 lua_tolstring 函数返回的指向内部字符串的指针，在该索引指向的元素被弹出之后，将无法保证仍然有效。该函数
返回的字符串末尾均会有一个尾部0。

下面将给出一个工具函数，可用于演示上面提到的部分函数，如：
 
	static void stackDump(lua_State* L) 
	{
		int top = lua_gettop(L);
		for (int i = 1; i <= top; ++i) {
		    int t = lua_type(L,i);
		    switch(t) {
		    case LUA_TSTRING:
		        printf("'%s'",lua_tostring(L,i));
		        break;
		    case LUA_TBOOLEAN:
		        printf(lua_toboolean(L,i) ? "true" : "false");
		        break;
		    case LUA_TNUMBER:
		        printf("%g",lua_tonumber(L,i));
		        break;
		    default:
		        printf("%s",lua_typename(L,t));
		        break;
		    }
		    printf("");
		}
		printf("\n");
	}
	
####其它栈操作函数

除了上面给出的数据交换函数之外，Lua的C API还提供了一组用于操作虚拟栈的普通函数，如：
    
    int lua_gettop(lua_State* L); --返回栈中元素的个数。
    void lua_settop(lua_State* L, int index); --将栈顶设置为指定的索引值。
    void lua_pushvalue(lua_State* L, int index); --将指定索引的元素副本压入栈。
    void lua_remove(lua_State* L, int index); --删除指定索引上的元素，其上面的元素自动下移。
    void lua_insert(lua_State* L, int index); --将栈顶元素插入到该索引值指向的位置。
    void lua_replace(lua_State* L, int index); --弹出栈顶元素，并将该值设置到指定索引上。

Lua还提供了一个宏用于弹出指定数量的元素：#define lua_pop(L,n)  lua_settop(L, -(n) - 1)    
    
见如下示例代码：
 
	int main()
	{
		lua_State* L = luaL_newstate();
		lua_pushboolean(L,1);
		lua_pushnumber(L,10);
		lua_pushnil(L);
		lua_pushstring(L,"hello");
		stackDump(L); //true 10 nil 'hello'

		lua_pushvalue(L,-4);
		stackDump(L); //true 10 nil 'hello' true

		lua_replace(L,3);
		stackDump(L); //true 10 true 'hello'

		lua_settop(L,6);
		stackDump(L); //true 10 true 'hello' nil nil

		lua_remove(L,-3);
		stackDump(L); //true 10 true nil nil

		lua_settop(L,-5);
		stackDump(L); //true

		lua_close(L);
		return 0;
	}

####C API中的错误处理

**C调用Lua**

通常情况下，应用程序代码是以“无保护”模式运行的。因此，当Lua发现“内存不足”这类错误时，只能通过调用“紧急”函数
来通知C语言程序，之后在结束应用程序。用户可通过lua_atpanic来设置自己的“紧急”函数。如果希望应用程序代码在发生
Lua错误时不会退出，可通过调用lua\_pcall函数以保护模式运行Lua代码。这样再发生内存错误时，lua_pcall会返回一个错
误代码，并将解释器重置为一致的状态。如果要保护与Lua的C代码，可以使用lua_cpall函数，它将接受一个C函数作为参数，
然后调用这个C函数。
    
**Lua调用C**

通常而言，当一个被Lua调用的C函数检测到错误时，它就应该调用lua_error，该函数会清理Lua中所有需要清理的资源，
然后跳转回发起执行的那个lua_pcall，并附上一条错误信息。
 
 
###C 调用 Lua 
####基础

Lua的一项重要用途就是作为一种配置语言。现在从一个简单的示例开始吧。
    
    --这里是用Lua代码定义的窗口大小的配置信息
    width = 200
    height = 300
    
下面是读取配置信息的C/C++代码：    
 
	#include <stdio.h>
	#include <string.h>
	#include <lua.hpp>
	#include <lauxlib.h>
	#include <lualib.h>

	void load(lua_State* L, const char* fname, int* w, int* h) {
		if (luaL_loadfile(L,fname) || lua_pcall(L,0,0,0)) {
		    printf("Error Msg is %s.\n",lua_tostring(L,-1));
		    return;
		}
		lua_getglobal(L,"width");
		lua_getglobal(L,"height");
		if (!lua_isnumber(L,-2)) {
		    printf("'width' should be a number\n" );
		    return;
		}
		if (!lua_isnumber(L,-1)) {
		    printf("'height' should be a number\n" );
		    return;
		}
		*w = lua_tointeger(L,-2);
		*h = lua_tointeger(L,-1);
	}


	int main()
	{
		lua_State* L = luaL_newstate();
		int w,h;
		load(L,"D:/test.lua",&w,&h);
		printf("width = %d, height = %d\n",w,h);
		lua_close(L);
		return 0;
	}
	 
下面是针对新函数的解释：
    
    lua_getglobal是宏，其原型为：#define lua_getglobal(L,s)  lua_getfield(L, LUA_GLOBALSINDEX, (s))。
    
每次调用这个宏的时候，都会将Lua代码中与之相应的全局变量值压入栈中，第一次调用时将全局变量"width"的值压入栈中，
之后再次调用时再将"height"的值也压入栈中。
 
####table操作

我们可以在C语言的代码中操作Lua中的table数据，这是一个非常非常方便且实用的功能。这样不仅可以使Lua代码的结构
更加清晰，也可以在C语言代码中定义等同的结构体与之对应，从而大大提高代码的可读性。见如下代码：
 
	#include <stdio.h>
	#include <string.h>
	#include <lua.hpp>
	#include <lauxlib.h>
	#include <lualib.h>

	void load(lua_State* L) {

		if (luaL_loadstring(L,"background = { r = 0.30, g = 0.10, b = 0 }") 
		    || lua_pcall(L,0,0,0)) {
		    printf("Error Msg is %s.\n",lua_tostring(L,-1));
		    return;
		}
		lua_getglobal(L,"background");
		if (!lua_istable(L,-1)) {
		    printf("'background' is not a table.\n" );
		    return;
		}
		lua_getfield(L,-1,"r");
		if (!lua_isnumber(L,-1)) {
		    printf("Invalid component in background color.\n");
		    return;
		}
		int r = (int)(lua_tonumber(L,-1) * 255);
		lua_pop(L,1);
		lua_getfield(L,-1,"g");
		if (!lua_isnumber(L,-1)) {
		    printf("Invalid component in background color.\n");
		    return;
		}
		int g = (int)(lua_tonumber(L,-1) * 255);
		lua_pop(L,1);

		lua_pushnumber(L,0.4);
		lua_setfield(L,-2,"b");

		lua_getfield(L,-1,"b");
		if (!lua_isnumber(L,-1)) {
		    printf("Invalid component in background color.\n");
		    return;
		}
		int b = (int)(lua_tonumber(L,-1) * 255);
		printf("r = %d, g = %d, b = %d\n",r,g,b);
		lua_pop(L,1);
		lua_pop(L,1);
		return;
	}

	int main()
	{
		lua_State* L = luaL_newstate();
		load(L);
		lua_close(L);
		return 0;
	}

void lua_getfield(lua_State *L, int idx, const char *k); 

第二个参数是table变量在栈中的索引值，最后一个参数是table的键值，该函数执行成功后会将字段值压入栈中。


    void lua_setfield(lua_State *L, int idx, const char *k); 

第二个参数是table变量在栈中的索引值，最后一个参数是table的键名称，而字段值是通过上一条命令
lua_pushnumber(L,0.4)压入到栈中的，该函数在执行成功后会将刚刚压入的字段值弹出栈。
   
下面的代码示例是在C语言代码中构造table对象，同时初始化table的字段值，最后再将table对象赋值给Lua中的一个
全局变量。

	#include <stdio.h>
	#include <string.h>
	#include <lua.hpp>
	#include <lauxlib.h>
	#include <lualib.h>

	void load(lua_State* L) 
	{
		lua_newtable(L);
		lua_pushnumber(L,0.3);
		lua_setfield(L,-2,"r");

		lua_pushnumber(L,0.1);
		lua_setfield(L,-2,"g");

		lua_pushnumber(L,0.4);
		lua_setfield(L,-2,"b");
		lua_setglobal(L,"background");

		lua_getglobal(L,"background");
		if (!lua_istable(L,-1)) {
		    printf("'background' is not a table.\n" );
		    return;
		}
		lua_getfield(L,-1,"r");
		if (!lua_isnumber(L,-1)) {
		    printf("Invalid component in background color.\n");
		    return;
		}
		int r = (int)(lua_tonumber(L,-1) * 255);
		lua_pop(L,1);
		lua_getfield(L,-1,"g");
		if (!lua_isnumber(L,-1)) {
		    printf("Invalid component in background color.\n");
		    return;
		}
		int g = (int)(lua_tonumber(L,-1) * 255);
		lua_pop(L,1);

		lua_getfield(L,-1,"b");
		if (!lua_isnumber(L,-1)) {
		    printf("Invalid component in background color.\n");
		    return;
		}
		int b = (int)(lua_tonumber(L,-1) * 255);
		printf("r = %d, g = %d, b = %d\n",r,g,b);
		lua_pop(L,1);
		lua_pop(L,1);
		return;
	}

	int main()
	{
		lua_State* L = luaL_newstate();
		load(L);
		lua_close(L);
		return 0;
	}

上面的代码将输出和之前代码相同的结果。

    lua_newtable是宏，其原型为：#define lua_newtable(L) lua_createtable(L, 0, 0)。调用该宏后，Lua会生成一个新的table对象并将其压入栈中。
    lua_setglobal是宏，其原型为：#define lua_setglobal(L,s) lua_setfield(L,LUA_GLOBALSINDEX,(s))。调用该宏后，Lua会将当前栈顶的值赋值给第二个参数指定的全局变量名。该宏在执行成功后，会将刚刚赋值的值从栈顶弹出。

####调用Lua函数：
    
调用函数的API也很简单。首先将待调用函数压入栈，再压入函数的参数，然后使用lua_pcall进行实际的调用，最后将调用结果从栈中弹出。见如下代码：


	#include <stdio.h>
	#include <string.h>
	#include <lua.hpp>
	#include <lauxlib.h>
	#include <lualib.h>

	const char* lua_function_code = "function add(x,y) return x + y end";

	void call_function(lua_State* L) 
	{
		//luaL_dostring 等同于luaL_loadstring() || lua_pcall()
		//注意：在能够调用Lua函数之前必须执行Lua脚本，否则在后面实际调用Lua函数时会报错，
		//错误信息为:"attempt to call a nil value."
		if (luaL_dostring(L,lua_function_code)) {
		    printf("Failed to run lua code.\n");
		    return;
		}
		double x = 1.0, y = 2.3;
		lua_getglobal(L,"add");
		lua_pushnumber(L,x);
		lua_pushnumber(L,y);
		//下面的第二个参数表示带调用的lua函数存在两个参数。
		//第三个参数表示即使带调用的函数存在多个返回值，那么也只有一个在执行后会被压入栈中。
		//lua_pcall调用后，虚拟栈中的函数参数和函数名均被弹出。
		if (lua_pcall(L,2,1,0)) {
		    printf("error is %s.\n",lua_tostring(L,-1));
		    return;
		}
		//此时结果已经被压入栈中。
		if (!lua_isnumber(L,-1)) {
		    printf("function 'add' must return a number.\n");
		    return;
		}
		double ret = lua_tonumber(L,-1);
		lua_pop(L,-1); //弹出返回值。
		printf("The result of call function is %f.\n",ret);
	}

	int main()
	{
		lua_State* L = luaL_newstate();
		call_function(L);
		lua_close(L);
		return 0;
	}

 Lua可以调用C函数的能力将极大的提高Lua的可扩展性和可用性。对于有些和操作系统相关的功能，
 或者是对效率要求较高的模块，我们完全可以通过C函数来实现，之后再通过Lua调用指定的C函数。
 对于那些可被Lua调用的C函数而言，其接口必须遵循Lua要求的形式，
 即typedef int (*lua_CFunction)(lua_State* L)。简单说明一下，该函数类型仅仅包含一个表
 示Lua环境的指针作为其唯一的参数，实现者可以通过该指针进一步获取Lua代码中实际传入的参数。
 返回值是整型，表示该C函数将返回给Lua代码的返回值数量，如果没有返回值，则return 0即可。
 需要说明的是，C函数无法直接将真正的返回值返回给Lua代码，而是通过虚拟栈来传递Lua代码和
 C函数之间的调用参数和返回值的。这里我们将介绍两种Lua调用C函数的规则。
 
 
 **C函数作为应用程序的一部分**
 
	#include <stdio.h>
	#include <string.h>
	#include <lua.hpp>
	#include <lauxlib.h>
	#include <lualib.h>

	//待Lua调用的C注册函数。
	static int add2(lua_State* L)
	{
		//检查栈中的参数是否合法，1表示Lua调用时的第一个参数(从左到右)，依此类推。
		//如果Lua代码在调用时传递的参数不为number，该函数将报错并终止程序的执行。
		double op1 = luaL_checknumber(L,1);
		double op2 = luaL_checknumber(L,2);
		//将函数的结果压入栈中。如果有多个返回值，可以在这里多次压入栈中。
		lua_pushnumber(L,op1 + op2);
		//返回值用于提示该C函数的返回值数量，即压入栈中的返回值数量。
		return 1;
	}

	//另一个待Lua调用的C注册函数。
	static int sub2(lua_State* L)
	{
		double op1 = luaL_checknumber(L,1);
		double op2 = luaL_checknumber(L,2);
		lua_pushnumber(L,op1 - op2);
		return 1;
	}

	const char* testfunc = "print(add2(1.0,2.0)) print(sub2(20.1,19))";

	int main()
	{
		lua_State* L = luaL_newstate();
		luaL_openlibs(L);
		//将指定的函数注册为Lua的全局函数变量，其中第一个字符串参数为Lua代码
		//在调用C函数时使用的全局函数名，第二个参数为实际C函数的指针。
		lua_register(L, "add2", add2);
		lua_register(L, "sub2", sub2);
		//在注册完所有的C函数之后，即可在Lua的代码块中使用这些已经注册的C函数了。
		if (luaL_dostring(L,testfunc))
		    printf("Failed to invoke.\n");
		lua_close(L);
		return 0;
	}


**C函数库成为Lua的模块**

将包含C函数的代码生成库文件，如Linux的so，或Windows的DLL，同时拷贝到Lua代码所在的当前目录，
或者是LUA_CPATH环境变量所指向的目录，以便于Lua解析器可以正确定位到他们。在我当前的Windows系统中，
我将其copy到"C:\Program Files\Lua\5.1\clibs\"，这里包含了所有Lua可调用的C库。见如下C语言代码和
关键性注释：

	#include <stdio.h>
	#include <string.h>
	#include <lua.hpp>
	#include <lauxlib.h>
	#include <lualib.h>

	//待注册的C函数，该函数的声明形式在上面的例子中已经给出。
	//需要说明的是，该函数必须以C的形式被导出，因此extern "C"是必须的。
	//函数代码和上例相同，这里不再赘述。
	extern "C" int add(lua_State* L) 
	{
		double op1 = luaL_checknumber(L,1);
		double op2 = luaL_checknumber(L,2);
		lua_pushnumber(L,op1 + op2);
		return 1;
	}

	extern "C" int sub(lua_State* L)
	{
		double op1 = luaL_checknumber(L,1);
		double op2 = luaL_checknumber(L,2);
		lua_pushnumber(L,op1 - op2);
		return 1;
	}

	//luaL_Reg结构体的第一个字段为字符串，在注册时用于通知Lua该函数的名字。
	//第一个字段为C函数指针。
	//结构体数组中的最后一个元素的两个字段均为NULL，用于提示Lua注册函数已经到达数组的末尾。
	static luaL_Reg mylibs[] = { 
		{"add", add},
		{"sub", sub},
		{NULL, NULL} 
	}; 

	//该C库的唯一入口函数。其函数签名等同于上面的注册函数。见如下几点说明：
	//1. 我们可以将该函数简单的理解为模块的工厂函数。
	//2. 其函数名必须为luaopen_xxx，其中xxx表示library名称。Lua代码require "xxx"需要与之对应。
	//3. 在luaL_register的调用中，其第一个字符串参数为模块名"xxx"，第二个参数为待注册函数的数组。
	//4. 需要强调的是，所有需要用到"xxx"的代码，不论C还是Lua，都必须保持一致，这是Lua的约定，
	//   否则将无法调用。
	extern "C" __declspec(dllexport)
	int luaopen_mytestlib(lua_State* L) 
	{
		const char* libName = "mytestlib";
		luaL_register(L,libName,mylibs);
		return 1;
	}

见如下Lua代码：

	1 require "mytestlib"  --指定包名称
	2 
	3 --在调用时，必须是package.function
	4 print(mytestlib.add(1.0,2.0))
	5 print(mytestlib.sub(20.1,19))


####数组操作

在Lua中，“数组”只是table的一个别名，是指以一种特殊的方法来使用table。出于性能原因，
Lua的C API为数组操作提供了专门的函数，如：
    
    void lua_rawgeti(lua_State* L, int index, int key);
    void lua_rawseti(lua_State* L, int index, int key);
 
 以上两个函数分别用于读取和设置数组中的元素值。其中index参数表示待操作的table在栈中的位置，
 key表示元素在table中的索引值。由于这两个函数均为原始操作，比涉及元表的table访问更快。
 通常而言，作为数组使用的table很少会用到元表。
 
见如下代码示例和关键性注释：

	#include <stdio.h>
	#include <string.h>
	#include <lua.hpp>
	#include <lauxlib.h>
	#include <lualib.h>

	extern "C" int mapFunc(lua_State* L)
	{
		//检查Lua调用代码中传递的第一个参数必须是table。否则将引发错误。
		luaL_checktype(L,1,LUA_TTABLE);
		luaL_checktype(L,2,LUA_TFUNCTION);
		//获取table中的字段数量，即数组的元素数量。
		int n = lua_objlen(L,1);
		//Lua中的数组起始索引习惯为1，而不是C中的0。
		for (int i = 1; i <= n; ++i) {
		    lua_pushvalue(L,2);  //将Lua参数中的function(第二个参数)的副本压入栈中。
		    lua_rawgeti(L,1,i);  //压入table[i]
		    lua_call(L,1,1);     //调用function(table[i])，并将函数结果压入栈中。
		    lua_rawseti(L,1,i);  //table[i] = 函数返回值，同时将返回值弹出栈。
		}

		//无结果返回给Lua代码。
		return 0;
	}

####字符串操作

当一个C函数从Lua收到一个字符串参数时，必须遵守两条规则：不要在访问字符串时从栈中将其弹出，
不要修改字符串。在Lua的C API中主要提供了两个操作Lua字符串的函数，即：

    void  lua_pushlstring(lua_State *L, const char *s, size_t l);
    const char* lua_pushfstring(lua_State* L, const char* fmt, ...);
    
第一个API用于截取指定长度的子字符串，同时将其压入栈中。而第二个API则类似于C库中的sprintf函数，
并将格式化后的字符串压入栈中。和sprintf的格式说明符不同的是，该函数只支持%%(表示字符%)、
%s(表示字符串)、%d(表示整数)、%f(表示Lua中的number)及%c(表示字符)。除此之外，不支持任何例如
宽度和精度的选项。


	#include <stdio.h>
	#include <string.h>
	#include <lua.hpp>
	#include <lauxlib.h>
	#include <lualib.h>

	extern "C" int splitFunc(lua_State* L)
	{
		const char* s = luaL_checkstring(L,1);
		const char* sep = luaL_checkstring(L,2); //分隔符
		const char* e;
		int i = 1;
		lua_newtable(L); //结果table
		while ((e = strchr(s,*sep)) != NULL) {
		    lua_pushlstring(L,s,e - s);  //压入子字符串。
		    //将刚刚压入的子字符串设置给table，同时赋值指定的索引值。
		    lua_rawseti(L,-2,i++);       
		    s = e + 1;
		}
		//压入最后一个子串
		lua_pushstring(L,s);
		lua_rawseti(L,-2,i);
		return 1; //返回table。
	}

 Lua API中提供了lua_concat函数，其功能类似于Lua中的".."操作符，用于连接(并弹出)栈顶的n个值，
 然后压入连接后的结果。其原型为：
    
    void  lua_concat(lua_State *L, int n);
    
参数n表示栈中待连接的字符串数量。该函数会调用元方法。然而需要说明的是，如果连接的字符串数量较少，
该函数可以很好的工作，反之，则会带来性能问题。为此，Lua API提供了另外一组函数专门解决由此而带来
的性能问题，见如下代码示例：

	#include <stdio.h>
	#include <string.h>
	#include <lua.hpp>
	#include <lauxlib.h>
	#include <lualib.h>

	extern "C" int strUpperFunc(lua_State* L)
	{
		size_t len;
		luaL_Buffer b;
		//检查参数第一个参数是否为字符串，同时返回字符串的指针及长度。
		const char* s = luaL_checklstring(L,1,&len);
		//初始化Lua的内部Buffer。
		luaL_buffinit(L,&b);
		//将处理后的字符依次(luaL_addchar)追加到Lua的内部Buffer中。
		for (int i = 0; i < len; ++i)
		    luaL_addchar(&b,toupper(s[i]));
		//将该Buffer及其内容压入栈中。
		luaL_pushresult(&b);
		return 1;
	}

使用缓冲机制的第一步是声明一个luaL_Buffer变量，并用luaL_buffinit来初始化它。初始化后，就可通过
luaL_addchar将一个字符放入缓冲。除该函数之外，Lua的辅助库还提供了直接添加字符串的函数，如：

    void luaL_addlstring(luaL_Buffer* b, const char* s, size_t len);
    void luaL_addstring(luaL_Buffer* b, const char* s);
   
最后luaL_pushresult会更新缓冲，并将最终的字符串留在栈顶。通过这些函数，就无须再关心缓冲的分配了。
但是在追加的过程中，缓冲会将一些中间结果放到栈中。因此，在使用时要留意此细节，只要保证压入和弹出的
次数相等既可。Lua API还提供一个比较常用的函数，用于将栈顶的字符串或数字也追加到缓冲区中，函数原型为：

    void luaL_addvalue(luaL_Buffer* b);


####C函数中保存状态

Lua API提供了三种方式来保存非局部变量，即注册表、环境和upvalue。
   
**注册表**

注册表是一个全局的table，只能被C代码访问。通常用于保存多个模块间的共享数据。我们可以通过
LUA_REGISTRYINDEX 索引值来访问注册表。

	#include <stdio.h>
	#include <string.h>
	#include <lua.hpp>
	#include <lauxlib.h>
	#include <lualib.h>

	void registryTestFunc(lua_State* L)
	{
		lua_pushstring(L,"Hello");
		lua_setfield(L,LUA_REGISTRYINDEX,"key1");
		lua_getfield(L,LUA_REGISTRYINDEX,"key1");
		printf("%s\n",lua_tostring(L,-1));
	}

	int main()
	{
		lua_State* L = luaL_newstate();
		registryTestFunc(L);
		lua_close(L);
		return 0;
	}
	
**环境**

如果需要保存一个模块的私有数据，即模块内各函数需要共享的数据，应该使用环境。
我们可以通过LUA_ENVIRONINDEX索引值来访问环境。

	#include <lua.hpp>
	#include <lauxlib.h>
	#include <lualib.h>

	//模块内设置环境数据的函数
	extern "C" int setValue(lua_State* L)
	{
		lua_pushstring(L,"Hello");
		lua_setfield(L,LUA_ENVIRONINDEX,"key1");
		return 0;
	}

	//模块内获取环境数据的函数
	extern "C" int getValue(lua_State* L)
	{
		lua_getfield(L,LUA_ENVIRONINDEX,"key1");
		printf("%s\n",lua_tostring(L,-1));
		return 0;
	}

	static luaL_Reg myfuncs[] = { 
		{"setValue", setValue},
		{"getValue", getValue},
		{NULL, NULL} 
	}; 


	extern "C" __declspec(dllexport)
	int luaopen_testenv(lua_State* L)
	{
		lua_newtable(L);  //创建一个新的表用于环境
		lua_replace(L,LUA_ENVIRONINDEX); //将刚刚创建并压入栈的新表替换为当前模块的环境表。
		luaL_register(L,"testenv",myfuncs);
		return 1;
	}

Lua测试代码如下。

	1 require "testenv"
	2 
	3 print(testenv.setValue())
	4 print(testenv.getValue())
	5 --输出为：Hello

 **upvalue**
 
upvalue是和特定函数关联的，我们可以将其简单的理解为函数内的静态变量。 

	#include <lua.hpp>
	#include <lauxlib.h>
	#include <lualib.h>

	extern "C" int counter(lua_State* L)
	{
		//获取第一个upvalue的值。
		int val = lua_tointeger(L,lua_upvalueindex(1));
		//将得到的结果压入栈中。
		lua_pushinteger(L,++val);
		//赋值一份栈顶的数据，以便于后面的替换操作。
		lua_pushvalue(L,-1);
		//该函数将栈顶的数据替换到upvalue(1)中的值。同时将栈顶数据弹出。
		lua_replace(L,lua_upvalueindex(1));
		//lua_pushinteger(L,++value)中压入的数据仍然保留在栈中并返回给Lua。
		return 1;
	}

	extern "C" int newCounter(lua_State* L)
	{
		//压入一个upvalue的初始值0，该函数必须先于lua_pushcclosure之前调用。
		lua_pushinteger(L,0);
		//压入闭包函数，参数1表示该闭包函数的upvalue数量。该函数返回值，闭包函数始终位于栈顶。
		lua_pushcclosure(L,counter,1);
		return 1;
	}

	static luaL_Reg myfuncs[] = { 
		{"counter", counter},
		{"newCounter", newCounter},
		{NULL, NULL} 
	}; 


	extern "C" __declspec(dllexport)
	int luaopen_testupvalue(lua_State* L)
	{
		luaL_register(L,"testupvalue",myfuncs);
		return 1;
	}

 Lua测试代码如下:
 
	require "testupvalue"

	func = testupvalue.newCounter();
	print(func());
	print(func());
	print(func());

	func = testupvalue.newCounter();
	print(func());
	print(func());
	print(func());

	--[[ 输出结果为：
	1
	2
	3
	1
	2
	3
	--]]


###UserData

在Lua中可以通过自定义类型的方式与C语言代码更高效、更灵活的交互。这里我们通过一个简单完整的示例来学习
一下Lua中userdata的使用方式。需要说明的是，该示例完全来自于Programming in Lua。其功能是用C程序实现
一个Lua的布尔数组，以提供程序的执行效率。见下面的代码和关键性注释。   

	#include <lua.hpp>
	#include <lauxlib.h>
	#include <lualib.h>
	#include <limits.h>

	#define BITS_PER_WORD (CHAR_BIT * sizeof(int))
	#define I_WORD(i)     ((unsigned int)(i))/BITS_PER_WORD
	#define I_BIT(i)      (1 << ((unsigned int)(i)%BITS_PER_WORD))

	typedef struct NumArray {
		int size;
		unsigned int values[1];
	} NumArray;

	extern "C" int newArray(lua_State* L)
	{
		//1. 检查第一个参数是否为整型。以及该参数的值是否大于等于1.
		int n = luaL_checkint(L,1);
		luaL_argcheck(L, n >= 1, 1, "invalid size.");
		size_t nbytes = sizeof(NumArray) + I_WORD(n - 1) * sizeof(int);
		//2. 参数表示Lua为userdata分配的字节数。同时将分配后的userdata对象压入栈中。
		NumArray* a = (NumArray*)lua_newuserdata(L,nbytes);
		a->size = n;
		for (int i = 0; i < I_WORD(n - 1); ++i)
		    a->values[i] = 0;
		//获取注册表变量myarray，该key的值为metatable。
		luaL_getmetatable(L,"myarray");
		//将userdata的元表设置为和myarray关联的table。同时将栈顶元素弹出。
		lua_setmetatable(L,-2);
		return 1;
	}

	extern "C" int setArray(lua_State* L)
	{
		//1. Lua传给该函数的第一个参数必须是userdata，该对象的元表也必须是注册表中和myarray关联的table。
		//否则该函数报错并终止程序。
		NumArray* a = (NumArray*)luaL_checkudata(L,1,"myarray");
		int index = luaL_checkint(L,2) - 1;
		//2. 由于任何类型的数据都可以成为布尔值，因此这里使用any只是为了确保有3个参数。
		luaL_checkany(L,3);
		luaL_argcheck(L,a != NULL,1,"'array' expected.");
		luaL_argcheck(L,0 <= index && index < a->size,2,"index out of range.");
		if (lua_toboolean(L,3))
		    a->values[I_WORD(index)] |= I_BIT(index);
		else
		    a->values[I_WORD(index)] &= ~I_BIT(index);
		return 0;
	}

	extern "C" int getArray(lua_State* L)
	{
		NumArray* a = (NumArray*)luaL_checkudata(L,1,"myarray");
		int index = luaL_checkint(L,2) - 1;
		luaL_argcheck(L, a != NULL, 1, "'array' expected.");
		luaL_argcheck(L, 0 <= index && index < a->size,2,"index out of range");
		lua_pushboolean(L,a->values[I_WORD(index)] & I_BIT(index));
		return 1;
	}

	extern "C" int getSize(lua_State* L)
	{
		NumArray* a = (NumArray*)luaL_checkudata(L,1,"myarray");
		luaL_argcheck(L,a != NULL,1,"'array' expected.");
		lua_pushinteger(L,a->size);
		return 1;
	}

	extern "C" int array2string(lua_State* L)
	{
		NumArray* a = (NumArray*)luaL_checkudata(L,1,"myarray");
		lua_pushfstring(L,"array(%d)",a->size);
		return 1;
	}

	static luaL_Reg arraylib_f [] = { 
		{"new", newArray},
		{NULL, NULL} 
	}; 

	static luaL_Reg arraylib_m [] = {
		{"set", setArray},
		{"get", getArray},
		{"size", getSize},
		{"__tostring", array2string}, //print(a)时Lua会调用该元方法。
		{NULL, NULL} 
	};

	extern "C" __declspec(dllexport)
	int luaopen_testuserdata(lua_State* L)
	{
		//1. 创建元表，并将该元表指定给newArray函数新创建的userdata。在Lua中userdata也是以table的身份表现的。
		//这样在调用对象函数时，可以通过验证其metatable的名称来确定参数userdata是否合法。
		luaL_newmetatable(L,"myarray");
		lua_pushvalue(L,-1);
		//2. 为了实现面对对象的调用方式，需要将元表的__index字段指向自身，同时再将arraylib_m数组中的函数注册到
		//元表中，之后基于这些注册函数的调用就可以以面向对象的形式调用了。
		//lua_setfield在执行后会将栈顶的table弹出。
		lua_setfield(L,-2,"__index");
		//将这些成员函数注册给元表，以保证Lua在寻找方法时可以定位。NULL参数表示将用栈顶的table代替第二个参数。
		luaL_register(L,NULL,arraylib_m);
		//这里只注册的工厂方法。
		luaL_register(L,"testuserdata",arraylib_f);
		return 1;
	}

**轻量级userdata**

之前介绍的是full userdata，Lua还提供了另一种轻量级userdata(light userdata)。事实上，轻量级userdata仅仅表示的是
C指针的值，即(void*)。由于它只是一个值，所以不用创建。如果需要将一个轻量级userdata放入栈中，调用lua_pushlightuserdata
即可。full userdata和light userdata之间最大的区别来自于相等性判断，对于一个full userdata，它只是与自身相等，而
light userdata则表示为一个C指针，因此，它与所有表示同一指针的light userdata相等。再有就是light userdata不会受到
垃圾收集器的管理，使用时就像一个普通的整型数字一样。





Lua C API
--------------------

    void lua_createtable (lua_State *L, int narr, int nrec)

* 操作说明：
  创建一个新的table, 并把它放在栈顶. narr和nrec分别指定该table的array部分和hash部分的预分配元素数量

* 返回值：无

* 栈影响：  栈高+1, 栈顶元素是新table

* 注： #define lua_newtable(L) lua_createtable(L, 0, 0) 常用这个

 
----------------

    void lua_getfield (lua_State *L, int index, const char *k)

* 操作说明:

        arr = Stack[index]    // arr肯定是表
        Stack.push( arr[k] )
    
    取表中键为k的元素, 这里的表是由index指向的栈上的一个表
* 返回值: 无

* 栈影响：栈高+1, 栈顶元素是(Stack[index])[k]

* 注：, 该操作将触发 __index 元方法

 

-----------------------

    void lua_setfield (lua_State *L, int index, const char *k)

* 操作说明：  

        arr = Stack[index]
        arr[k] = Stack.top()
        Stack.pop()
    
    给表中键为k的元素赋值value(value就是栈顶元素), 这里的表是由index指向的栈上的一个表

* 返回值： 无

* 栈影响：高度-1, 被弹出的是value

* 注： 该操作将触发 __newindex 元方法

 
--------------------

    void lua_gettable (lua_State *L, int index)

* 操作说明: 

        ele  = Stack[index]
        key = Stack.top()
        Stack.pop()
        value = ele[key]
        Stack.push(value)

    根据 index 指定取到相应的表; 取栈顶元素为 key, 并弹出栈; 获取表中 key 的值压入栈顶.

* 返回值：无

* 栈影响：
    高度不变, 但是发生了一次弹出和压入的操作, 弹出的是key, 压入的是value

* 注：将触发 __index 元方法

--------------


    void lua_settable (lua_State *L, int index)

* 操作说明:   

        ele    = Stack[index]
        value  = Stack.top()
        Stack.pop()
        key    = Stack.top()
        Stack.pop()
        ele[key] = value

    根据index指定取到相应的表; 取栈顶元素做value, 弹出之; 再取当前栈顶元素做key, 亦弹出之; 然后将表的键为key的元素赋值为value

* 返回值: 无

* 栈影响：高度-2, 第一次弹出value, 第二次弹出key

* 注： 该操作将触发 __newindex 元方法

-------------------------- 

    void lua_rawget (lua_State *L, int index)

* 操作说明：和lua_gettable操作一样,但是不触发相应的元方法

--------------------------  

    void lua_rawgeti(lua_State *L, int index, int n)

* 操作说明  

        ele = Stack[index]
        value = ele[n]
        Stack.push(value)

* 返回值：无

* 栈影响：栈高+1， 栈顶新增元素就是 value

* 注：不触发相应的元方法

------------------------

    void lua_rawset (lua_State *L, int index) 

* 操作说明：和lua_settable操作一样，但是不触发相应的原方法

------------------------

    void lua_rawseti (lua_State *L, int index, int n) 

* 操作说明:   

        ele = Stack[index]
        value = Stack.top()
        Stack.pop()
        ele[n] = value

* 返回值：无

* 栈影响：-1, 栈顶将value弹出

* 注：不触发相应的元方法

------------------------------ 

    void lua_pushvalue (lua_State *L, int index)

* 操作说明:

        value = Stack[index]       
        Stack.push(value)

* 返回值：无

* 栈影响：栈高 +1 

------------------------------

    int luaL_newmetatable (lua_State *L, const char *tname)

* 操作说明:   
    1. 在注册表中查找tname, 如果已经注册, 就返回0, 否者继续, 并平栈

            lua_getfield(L, LUA_REGISTRYINDEX, tname)
            if (!lua_isnil(L, -1))
                return 0;
            lua_pop(L, 1);

    2. 创建一个表, 并注册, 返回1

            lua_newtable(L)
            lua_pushvalue(L, -1)
            lua_setfield(L, LUA_REGISTRYINDEX, tname)
            return 1

* 返回值：已注册, 就返回0；未注册,创建表后，返回1

* 栈影响：栈高+1, 栈顶元素是在注册表中注册过的新表

------------------------------------

    void *lua_newuserdata (lua_State *L, size_t size)

* 操作说明：该函数分配一块由size指定大小的内存块, 并放在栈顶

* 返回值：新分配的块的地址

* 栈影响：栈高+1， 栈顶是userdata

* 注：userdata用来在lua中表示c中的值. 一个完整的userdata有自己的元表, 在垃圾回收时, 可以调用它的元表的__gc方法

-------------------------------

    void lua_pushcclosure (lua_State *L, lua_CFunction fn, int n)

* 操作说明：
  向栈上压一个C闭包。当一个c函数被创建时, 可以绑定几个值在它上面, 从而形成一个闭包.  在任何时刻调用这个c函数时, 都可以访问这几个绑定值. 

  绑定的方法: 先一次压入要绑定的n个值到栈上, 然后调用lua_pushcclosure(L, fn, n)这样就形成的一个c闭包

* 返回值：无

* 栈影响：栈高 –(n - 1) , 一共弹出n个元素(及那些绑定的值), 压入一个cclosure

* 注

        #define lua_pushcfunction(L, f) lua_pushcclosure(L, f, 0)
        #define lua_register(L, n, f) (lua_pushcfunction(L, f), lua_setglobal(L, n))

   这个是比较常用的, 以n为lua中的key压入一个0个绑定值的cclosure.
   
-------------------------------
    void lua_call(lua_State* L, int nargs, int nresults)

* 操作说明

        argn = Stack.pop()
        ... // 一共压入nargs个参数
        arg2 = Stack.pop()
        arg3 = Stack.pop()
        func = Stack.pop() // 函数本身也弹出
        res1， res2， ..., resj = func(arg1, arg2, ..., argn)
        Stack.push(res1)
        Stack.push(res2)
        … // 压入nresults个返回值
        Stack.push(resj)

* 返回值：无

* 栈影响：调用结束后, 栈高度增加 nresults – (1 + nargs), 如果将 nresults 参数设置为LUA_MULTRET, 那么lua返回几个值, 栈上就压入几个值, 否者强制压入 nresults 个值, 不足的是空值, 多余的抛弃掉

* 注：意, 这个函数是有危险的, 如果在其中发生了错误, 会直接退出程序

    这个函数的用途: 尚未发现, 除非你能接受出错立马退出, 反正我是做游戏的, 我受不起, 呵呵, 顺便一说, lauxlib.h中的luaL_check*一族函数也是这样的, 不符合预期的话, 直接退出, 这些函数都要小心, 有类似于断言的效果.

----------------------

    int lua_pcall(lua_State* L, int nargs, int nresults, int errfunc)

* 操作说明：
    参数, 行为和lua_call都一样, 如果在调用中没有发生任何错误, lua_pcall == lua_call; 但是如果有错误发生时, lua_pcall会捕获它
    errfunc指出了 Stack 上的一个元素, 这个元素应该是一个函数, 当发生错误的时候

        ef = Stack[errfunc]
        value = ef(errmsg)
        Stack.push(value)

    也就是说, 在错误的时候, errfunc指定的错误处理函数会被调用, 该处理函数的返回值被压到栈上.

    默认情况下, 可以给errfunc传值 0, 实际的效果是指定了这样一个函数做出错处理 function defaulterr(errmsg) return errmsg end.

* 注
本函数有返回值 LUA_ERRRUN运行时错误  LUA_ERRMEM内存分配错误[注意, 这种错会导致lua调用不了错误处理函数]  LUA_ERRERR运行错误处理函数时出错了, 写程序的时候必须检查返回值:)

    强烈推荐该函数, 不过事实上大家也都用的这个函数:)

---------------------

    int lua_cpcall (lua_State *L, lua_CFunction func, void *ud)

* 操作说明
   以保护模式调用c函数, func 中可以且只能从堆栈上拿到一个参数, 就是 ud, 当有错误时, 和 lua_pcall 返回相同的错误代码, 并在堆栈顶部留下errmsg字符串, 调用成功的话它返回0, 并且不会修改堆栈, 所有从func中返回的值都被扔掉.

* 注:
1. "当有错误时", 这个错误的意思是lua的错误, 而不是c/c++的错误. 在 func 中使用 lua_call 和 lua_check* 族函数, 并不会导致程序退出了, 而是表现的像 lua_pcall 那样.
2. 调用成功的时候 func 中的返回值都被扔掉了.
