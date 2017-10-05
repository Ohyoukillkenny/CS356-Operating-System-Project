所有的程序都是可以编译通过跑起来的，我看那个提交的说明，已经移除了所有的.ko和ARM文件。
但如果助教您发现程序编译通过不了或者因为文件缺失的原因文件无法生成.ko文件或者执行文件，请与我联系：
手机：13968938250 孔令坤 

In BBC is my burger problem's code.
After ndk-build, it will generate the executing file -- lastBBCARM
and you should input command like this: ./lastBBCARM 2 5 20 10

In ptree file is my ptree systemcall and the my use system call code.
In ptreesyscall, is my ptree syscall. And you can make file and get ptree.ko.
After install the module ptree.ko in kernel,you can use the syscall.

In jni is the simple use of the syscall.
It will generate ptreeARM, after input ./ptreeARM in linux terminal, you can see the output of ptree.

In Problem3 file, its my code of the problem3,it will generate pro3ARM.
After you input ./pro3ARM you will see answer of the problem3 including "parent","child",and ptree.
Be careful that you should have installed ptree.ko and pushed the ptreeARM in linux.
