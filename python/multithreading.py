# From the OpenStack swiftClient
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
# implied.
# See the License for the specific language governing permissions and
# limitations under the License.


'''
    ##开启一个打印信息和错误的线程
    with MultiThreadingManager() as thread_manager:    
        try:
            #开启num_thread个线程执行func(item,args,kwargs)函数，此时线程阻塞,
            func_manager = thread_manager.queue_manager(func, num_threads,
					*args, **kwargs)
            func_queue = func_manager.queue 
		    #当把对象放入队列，此时func才开始执行.
            func_queue.put(param)
	        #把信息加入正常信息队列，自动输出到错误流
            thread_manager.print_msg("%d of thread 
					call %s", num_threads, func.__name__)
            #把信息加入错误信息队列, 自动输出到输出流
            thread_manager.error("%d of thread 
					call %s", num_threads, func.__name__)
        except Exception as err:
            had_error = thread_manager.error(str(err))
    ###当遇到异常的或with执行完，全部线程退出.
'''




import sys
from time import sleep
from Queue import Queue
from threading import Thread
from traceback import format_exception


class StopWorkerThreadSignal(object):
    pass

#线程一直进行: 1、当队列为空时，阻塞
#              2、直到传入StopWorkerThreadSignal对象到队列时才终止。
class QueueFunctionThread(Thread):
    """
    Calls `func`` for each item in ``queue``; ``func`` is called with a
    de-queued item as the first arg followed by ``*args`` and ``**kwargs``.

    Any exceptions raised by ``func`` are stored in :attr:`self.exc_infos`.

    If the optional kwarg ``store_results`` is specified, it must be a list and
    each result of invoking ``func`` will be appended to that list.

    Putting a :class:`StopWorkerThreadSignal` instance into queue will cause
    this thread to exit.
    """

    def __init__(self, queue, func, *args, **kwargs):
        """
        :param queue: A :class:`Queue` object from which work jobs will be
                      pulled.
        :param func: A callable which will be invoked with a dequeued item
                     followed by ``*args`` and ``**kwargs``.
        :param \*args: Optional positional arguments for ``func``.
        :param \*\*kwargs: Optional kwargs for func.  If the kwarg
                           ``store_results`` is specified, its value must be a
                           list, and every result from invoking ``func`` will
                           be appended to the supplied list.  The kwarg
                           ``store_results`` will not be passed into ``func``.
        """
        Thread.__init__(self)
        self.queue = queue
        self.func = func
        self.args = args
        self.kwargs = kwargs
        self.exc_infos = [] #收集异常信息
        self.store_results = kwargs.pop('store_results', None)

    def run(self):
        while True:
            #当队列为空时，阻塞。可以用get_nowait()为非阻塞形式.
            item = self.queue.get()
            if isinstance(item, StopWorkerThreadSignal):
                break
            try:
                #================================================
                result = self.func(item, *self.args, **self.kwargs)
                #================================================
                if self.store_results is not None:
                    self.store_results.append(result)
            except Exception:
                self.exc_infos.append(sys.exc_info())

    def stop(self):
        st = StopWorkerThreadSignal()
        self.queue.put(st)


#根据__enter__和__exit__可以看出，用with QueueFunctionManager as qeueManage 是一个不错的选择。
#with QueueFunctionManager(fun,1,manage) as que. 
#que是一个Queue对象，que.put(str)。会自动调用fun(str).

class QueueFunctionManager(object):
    """
    A context manager to handle the life-cycle of a single :class:`Queue`
    and a list of associated :class:`QueueFunctionThread` instances.

    This class is not usually instantiated directly.  Instead, call the
    :meth:`MultiThreadingManager.queue_manager` object method,
    which will return an instance of this class.

    When entering the context, ``thread_count`` :class:`QueueFunctionThread`
    instances are created and started.  The input queue is returned.  Inside
    the context, any work item put into the queue will get worked on by one of
    the :class:`QueueFunctionThread` instances.

    When the context is exited, all threads are sent a
    :class:`StopWorkerThreadSignal` instance and then all threads are waited
    upon.  Finally, any exceptions from any of the threads are reported on via
    the supplied ``thread_manager``'s :meth:`error` method.  If an
    ``error_counter`` list was supplied on instantiation, its first element is
    incremented once for every exception which occurred.
    """

    def __init__(self, func, thread_count, thread_manager, thread_args=None,
                 thread_kwargs=None, error_counter=None,
                 connection_maker=None):
        """
        :param func: The worker function which will be passed into each
                     :class:`QueueFunctionThread`'s constructor.
        :param thread_count: The number of worker threads to run.
        :param thread_manager: An instance of :class:`MultiThreadingManager`.
        :param thread_args: Optional positional arguments to be passed into
                            each invocation of ``func`` after the de-queued
                            work item.
        :param thread_kwargs: Optional keyword arguments to be passed into each
                              invocation of ``func``.  If a list is supplied as
                              the ``store_results`` keyword argument, it will
                              be filled with every result of invoking ``func``
                              in all threads.
        :param error_counter: Optional list containing one integer.  If
                              supplied, the list's first element will be
                              incremented once for each exception in any
                              thread.  This happens only when exiting the
                              context.
        :param connection_maker: Optional callable.  If supplied, this callable
                                 will be invoked once per created thread, and
                                 the result will be passed into func after the
                                 de-queued work item but before ``thread_args``
                                 and ``thread_kwargs``.  This is used to ensure
                                 each thread has its own connection to Swift.
        """
        self.func = func
        self.thread_count = thread_count
        self.thread_manager = thread_manager
        self.error_counter = error_counter
        self.connection_maker = connection_maker
        self.queue = Queue(10000)
        self.thread_list = []
        self.thread_args = thread_args if thread_args else ()
        self.thread_kwargs = thread_kwargs if thread_kwargs else {}

    def __enter__(self):
        '''
           开启self.thread_counts个线程，每个线程都从队列中取出对象，对每个对象调用相同的函数。
           当队列为空的时候，阻塞。这样一旦开启之后将不会停止。每个线程不断地取出对象，不断地调用
           当初始为不同的函数就能执行不同的多线程。
           
        '''
        for _junk in xrange(self.thread_count):
            if self.connection_maker:
                thread_args = (self.connection_maker(),) + self.thread_args
            else:
                thread_args = self.thread_args
            #================================
            qf_thread = QueueFunctionThread(self.queue, self.func,
                                            *thread_args, **self.thread_kwargs)
            qf_thread.start()
            #=================================
            self.thread_list.append(qf_thread)
        return self.queue

    def __exit__(self, exc_type, exc_value, traceback):
        '''
            如果线程有的仍然在运行，就用停止的标志来终止这个线程。
        '''
        #依次发出停止信号
        for thread in [t for t in self.thread_list if t.isAlive()]:
            self.queue.put(StopWorkerThreadSignal())

        #??????????????????
        #如果有线程在执行，就等待
        while any(map(QueueFunctionThread.is_alive, self.thread_list)):
            sleep(0.05)

        for thread in self.thread_list:
            #info是list 的一个元素。
            for info in thread.exc_infos: 
                #self.error_counter记录了所有的异常个数.
                if self.error_counter:
                    self.error_counter[0] += 1
                self.thread_manager.error(''.join(format_exception(*info)))


#默认将标准输出和错误重定向到标准错误。

class MultiThreadingManager(object):
    """
    One object to manage context for multi-threading.  This should make
    bin/swift less error-prone and allow us to test this code.

    This object is a context manager and returns itself into the context.  When
    entering the context, two printing threads are created (see below) and they
    are waited on and cleaned up when exiting the context.

    A convenience method, :meth:`queue_manager`, is provided to create a
    :class:`QueueFunctionManager` context manager (a thread-pool with an
    associated input queue for work items).

    Also, thread-safe printing to two streams is provided.  The
    :meth:`print_msg` method will print to the supplied ``print_stream``
    (defaults to ``sys.stdout``) and the :meth:`error` method will print to the
    supplied ``error_stream`` (defaults to ``sys.stderr``).  Both of these
    printing methods will format the given string with any supplied ``*args``
    (a la printf) and encode the result to utf8 if necessary.

    The attribute :attr:`self.error_count` is incremented once per error
    message printed, so an application can tell if any worker threads
    encountered exceptions or otherwise called :meth:`error` on this instance.
    The swift command-line tool uses this to exit non-zero if any error strings
    were printed.
    """

    def __init__(self, print_stream=sys.stdout, error_stream=sys.stderr):
        """
        :param print_stream: The stream to which :meth:`print_msg` sends
                             formatted messages, encoded to utf8 if necessary.
        :param error_stream: The stream to which :meth:`error` sends formatted
                             messages, encoded to utf8 if necessary.
        """
        self.print_stream = print_stream
        self.printer = QueueFunctionManager(self._print, 1, self)
        self.error_stream = error_stream
        self.error_printer = QueueFunctionManager(self._print_error, 1, self)
        self.error_count = 0

    def __enter__(self):
        self.printer.__enter__() #开启一个打印线程
        self.error_printer.__enter__()#开启一个打印错误的线程
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.error_printer.__exit__(exc_type, exc_value, traceback)
        self.printer.__exit__(exc_type, exc_value, traceback)

    #返回值为一个队列对象。只有把要被func调用的参数put进队列就可以了.
    def queue_manager(self, func, thread_count, *args, **kwargs):
        connection_maker = kwargs.pop('connection_maker', None)
        error_counter = kwargs.pop('error_counter', None)
        return QueueFunctionManager(func, thread_count, self, thread_args=args,
                                    thread_kwargs=kwargs,
                                    connection_maker=connection_maker,
                                    error_counter=error_counter)

    def print_msg(self, msg, *fmt_args):
        if fmt_args:
            msg = msg % fmt_args
        self.printer.queue.put(msg)

    def error(self, msg, *fmt_args):
        if fmt_args:
            msg = msg % fmt_args
        self.error_printer.queue.put(msg)

    #这里对item为队列的元素
    def _print(self, item, stream=None):
        if stream is None:
            stream = self.print_stream
        if isinstance(item, unicode):
            item = item.encode('utf8')
        print >>stream, item

    def _print_error(self, item):
        self.error_count += 1
        return self._print(item, stream=self.error_stream)

def func(param)
    pass

if __name__ == "__main__":
    #the param MultiThreadingManager() is NULL, print the standard Output and
    #Error default, you can change it as needed

    
    num_threads = 5
    with MultiThreadingManager() as thread_manager:
        try:
            func_manager = thread_manager.queue_manager(func, num_threads)
            func_queue = func_manager.queue 
            #here the param in put must be same as func
            func_queue.put(param)
            thread_manager.print_msg("%d of thread call %s", num_threads, func.__name__)
        except Exception as err:
            had_error = thread_manager.error(str(err))

