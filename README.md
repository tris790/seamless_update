# seamless_update
Update a running process without a hitch

The main process creates a child process with shared memory.  
When you want to update the child process, the main process creates an other child process that will take over from there.
