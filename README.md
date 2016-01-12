ThreadPool
=
Simple c++ thread pool inspired by https://github.com/progschj/ThreadPool
Capabilities
-
<ul>
<li>Fixed-size thread pool</li>
<li>Fair non-priority task queue</li>
<li>Improvement over base: does not rely on system notify queue</li>
</ul>
Small Description
-
In general, it is just a code of progschj/ThreadPool with task notification improvement<br>
In previous version you could face a problem, while having lots of heavy tasks and lots of quick task producers<br>
Each task added to task queue produced <code>cv.notify_one()</code>, which added new notification directly to system queue.
Modified version just uses task queue for storing tasks, ant there is one more background process <code>ManageThread</code>,
which efficiently manages notifications. So, you can have maximum of <code>O(THREAD_COUNT)</code> notifications in system queue
Tests
-
I didn't test it quite well, however, [test.cpp](test/test.cpp) provides example, when ThreadPool shows it's best in comparasion with threads spawning
Compilation
-
Supports <b>CMake</b> and simple <b>make</b>
<code>
cmake && make
</code>
or just
<code>
make -f Makefile.in
</code>
Examples
-
For exaples just see [test.cpp](test/test.cpp)<br>
Basic usage:
```c++
// create thread pool with 4 threads
pool::ThreadPool tp(4); 
// add task to thread pool
auto res = tp.AddTask([]() -> int { /* some stuff */ return rand(); };
// show result
std::cout << res.get() << std::endl;
```
