#strace_GSOC2014

###Abstract
I want to make it easier to parse the strace’s output. Since the well-structured output in JSON is much better than the current classical output for program analysis. I am going to import a new feature to strace to make it support output in JSON. In my method, I will refactor the currently output-related code in strace and then import a totally-new framework to decouple the currently tightly-coupled high-level format and the low-level output functions.

###Current Work
Currently, I am still working on my prototpe to make it better. The code is changing rappidly and I will not provide any single PATCH file. You can clone the whole strace_GSOC2014 repository and compile it to have a test. If you really need the PATCH, please use git to generate the patch.
Please Note that the prototype is only a simple prototype. The implementation is not optimal and some code may be ugly.

###HowTo(Not Avaliable yet, coming soon)
It's easy to use my modified strace, just like you compling any other linux source code.  
1. git clone https://github.com/zym0017d/strace_GSOC2014.git  
2. cd strace_GSOC2014 && git checkout devel  
3. make clean && ./configure && make  
4. cd strace_GSOC2014/test && make  

After this, You should have the newly compiled strace in the top directory.  
Note: You'd better not run make install  

Let me show you in one simple example:  

**1) First we run strace without option “-j” to see the classical output:**  
$ **./strace  -r -T -i -e trace=open,read,write -o test_result_classical.txt ./test**  

> The content of test\_classical_json.txt.(Only partial of the entire output)  
0.000032 [0000003b3acdb400] open("this_is_a_simple_test", O_WRONLY|O_CREAT|O_TRUNC, 0777) = 4  <0.000046>  
  0.000068 [0000003b3acdb5f0] read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\2\0>\0\1\0\0\0\200\5@\0\0\0\0\0"..., 3500) = 3500  <0.000009>  
  0.000029 [0000003b3acdb650] write(4, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\2\0>\0\1\0\0\0\200\5@\0\0\0\0\0"..., 3500) = 3500  <0.000016>    
  
  
**2) Then we run strace with option “-j” to see the new output:**  
$ **./strace  -r -T -i -j -e trace=open,read,write -o test_result_json.txt ./test**  

> The content of test\_result_json.txt.(Only partial of the entire output)  
  {  
     "TimeRelative" : "     0.000035",  
     "Pointer" : "0000003b3acdb400",  
     "Type" : "syscall",  
     "Name" : "open",  
     "Args" : [ "this_is_a_simple_test", "O_WRONLY|O_CREAT|O_TRUNC", "0777",  ],  
     "Return" : "4",  
     "TimeSpent" : "0.000054",  
  }  
  {  
     "TimeRelative" : "     0.000079",  
     "Pointer" : "0000003b3acdb5f0",  
     "Type" : "syscall",  
     "Name" : "read",  
     "Args" : [ "3", ""\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\2\0>\0\1\0\0\0\200\5@\0\0\0\0\0"...", "3500",  ],  
     "Return" : "3500",  
     "TimeSpent" : "0.000010",  
  }  
  {  
     "TimeRelative" : "     0.000034",  
      "Pointer" : "0000003b3acdb650",  
     "Type" : "syscall",  
     "Name" : "write",  
     "Args" : [ "4", ""\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\2\0>\0\1\0\0\0\200\5@\0\0\0\0\0"...", "3500",  ],  
     "Return" : "3500",  
     "TimeSpent" : "0.000019",  
  }    
     

You can find the JSON output are exactly 3 object corresponding to the classical output. There are still some small format problems in the “TimeRelative” (the leading spaces) and the “Args” (the quotes). I’m keep on working to eliminate these problems. If you have ANY question or suggestion, please feel free to contact me.

###Events and Timeline
* **Current -- May    19: Community Bonding Period**
* May 20  -- June   23: Working Period
* June 23 -- June   27: Mid-term Evaluation
* June 27 -- August 11: Working Period
* August 18           : Pencil Down!

###TODO List (before May 19)
* write a test application to automaticaly compare the json output to the original output.
* add support for all IO functions and do more test.
  * convert all the old prototype code to here
  * start new coding job.
