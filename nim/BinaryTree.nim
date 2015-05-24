type
    BinaryTreeObj[T] = object
        le, ri: BinaryTreeObj
        data: T

    BinaryTree*[T] = ref BinaryTreeObj[T]  # type that is exported

proc newNode*(data: T): BinaryTree[T] =
    new(result)
    result.data = data

proc add*[T](root: var BinaryTree[T], n: BinaryTree[T]) =
    if root == nil:
        root = n
    else:
        var it = root
        while it != nil:
            # compare the data items; uses the generic ``cmp`` proc
            # that works for any type that has a ``==`` and ``<`` operator
            var c = cmp(n.data, it.data)
            if c < 0:
                if it.le == nil:
                    it.le = n
                    return
                it = it.le
            else:
                if it.ri == nil:
                    it.ri = n
                    return
                it = it.ri

proc add*[T](root: var BinaryTree[T], data: T) =
    add(root, newNode(data))

iterator preorder*[T](root: BinaryTree[T]): T =
    var stack: seq[BinaryTree[T]] = @[root]
    while stack.len > 0:
        var n = stack.pop()
        while n != nil:
            yield n.data
            add(stack, n.ri)
            n = n.le

var
    root: BinaryTree[string]
add(root, newNode("hello"))
for str in preorder(root):
    stdout.writeln(str)
