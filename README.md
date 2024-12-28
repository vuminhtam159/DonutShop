# DonutShop
A multithreaded C program that handles Threads

A C program that handles threading in such a way to prevent deadlock from happening using tools such as critical sections, counting semaphores, mutexes, events.

**Detail Descriptions:**

The donut shop will have 2 bakers.  For each baker, input the number of donuts the baker will bake today.  Also input the time it takes to bake one donut.  The baker will randomly bake 4 different types of donuts.  When the baker chooses what kind of donut to make the baker looks at the bin where the donut will go, if there is room in the bin the baker makes the donut and puts it into the bin.  The baker continues to bake donuts until the baker makes all of the donuts (donuts for today).  Once they bake all of their donuts they can go home (thread dies).  If all of the bins are full the baker will have to wait to bake donuts until space opens.  Only one baker or worker can be in a bin at a time.

You will have 4 bins one for each type of donut.  The user should input how many donuts fit in a bin.  All bins will hold the same number of donuts.  Remember each bin will only allow one thread (baker or worker) to modify it at a time.  Each bin should store the maximum number of donuts it can hold, current number of donuts in the bin and the number of donuts that where taken out.

You will have 6 worker which fill donut orders.  For each worker input the time between orders.  When it is time for a worker to get an order the program will randomly pick the kind of donut (bin #).  Each order will only be one donut.  If the bin has the donut we take the donut and give it to the customer.  If the bin is empty we wait at the bin until a donut is made.  We will wait in line at the bin.  First worker in the line is the first worker to get a donut.  If the worker gets to a bin and it is empty and both bakers are done baking then we need to ask the customer to pick a different kind of donut.  All workers will continue to work until all bakers are done working and the bins are empty.  Each worker should keep track of the number of donuts sold for each type of donut.

For this program there will be a total of 12 threads.  
2 Baker threads
6 Worker threads
4 Queue manager threads
