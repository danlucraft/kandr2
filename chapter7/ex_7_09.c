/*

Functions like isupper can be implemented to save space or to save time.
Explore both possibilities.

---

NB I've actually done this for toupper instead of isupper as it seemed like it
would be a better test case.

Are we talking about lookup tables here? We could store an "uppercase" value
for every char in a big array? Only 255 possible values?

That would be versus doing a range check and an add.

I'm not sure the lookup table would be faster, since that requires memory
access whereas the addition can all in done without hitting memory?

---

The other ex_7_09* files contain the different implementations:

 * .._calculate.c does it by arithmetic
 * .._lookup.c embeds a lookup table to do the conversion
 * .._stdlib.c uses toupper from ctype.h
 * .._none.c contains the frame that generates random 'char' values to get a
   base level of runtime without the actual calculation

Binary sizes are:

$ ls -l chapter7/ex_7_09*
8480  7 Aug 09:52 chapter7/ex_7_09_isupper_none
8520  7 Aug 09:52 chapter7/ex_7_09_isupper_calculate
8512  7 Aug 09:52 chapter7/ex_7_09_isupper_lookup
8528  7 Aug 09:52 chapter7/ex_7_09_isupper_stdlib

So no big difference there... Which I don't really understand.

And runtimes:

 * none      28.3s
 * calculate 33.0s
 * lookup    24.6s
 * stdlib    54.3s
 
So the lookup table does better than the calculation... but even better than
NOT DOING THE OPERATION AT ALL which is very suspicious... Is there some weird
optimization going on?

The stdlib version takes over double the time... what is it DOING? Looking at
the source code (FreeBSD libc on Apple's site) it seems it does a bunch of
locale checks and then looks in a lookup table. It even does a binary search on
the lookup table.

Conclusion: it seems that lookup tables offer a big speed advantage even for
very simple arithmetic like this. The stdlib uses lookup tables. There doesn't seem to be any difference in the size of the generated binary.

*/

/*

$ clang -Weverything chapter7/ex_7_09_isupper_none.c -o chapter7/ex_7_09_isupper_none && time ./chapter7/ex_7_09_isupper_none
sum: -491543

real	0m28.250s
user	0m28.219s
sys	0m0.021s

$ clang -Weverything chapter7/ex_7_09_isupper_calculate.c -o chapter7/ex_7_09_isupper_calculate && time ./chapter7/ex_7_09_isupper_calculate
sum: -182903

real	0m32.975s
user	0m32.914s
sys	0m0.046s

$ clang -Weverything chapter7/ex_7_09_isupper_lookup.c -o chapter7/ex_7_09_isupper_lookup && time ./chapter7/ex_7_09_isupper_lookup
sum: -182903

real	0m24.593s
user	0m24.566s
sys	0m0.019s

$ clang -Weverything chapter7/ex_7_09_isupper_stdlib.c -o chapter7/ex_7_09_isupper_stdlib && time ./chapter7/ex_7_09_isupper_stdlib
sum: -182903

real  0m54.264s
user  0m53.972s
sys	0m0.240s

*/
