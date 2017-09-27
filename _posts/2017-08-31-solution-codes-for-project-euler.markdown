---
layout: post
title:  "Solution codes for Project Euler"
date:   2017-08-31 12:00:00 +0800
categories: technical
cover_image: "Resources/Posts/ProjectEuler/euler_portrait.png"
---

I created a new [repo][web_repo] on GitHub to share my solution codes for the problems of [Project Euler][web_pe]. I find it is really interesting.

# Comments on the problems.

* 001
* 002
* 003

A function is created to test whether a positive integer is a prime number.

* 004
* 005

Codes are developed to calculate the least common multiple of a group of numbers.

* 006

There are some interesting facts about the sum of the sequence of n^2. And it turns out that 

(1 + 2 + ... + n)^2 = 1^3 + 2^3 + ... + n^3. 

Amazing!

* 007

It turns out that in the Python environment if you check the current number to see whether it is a prime number by referring to the prime numbers already found and stored in a list, the overall performance is not as good as expected.

* 008

No magical things happened.

* 009

There is a genius solution in the discussion thread of Project Euler.

* 010

No magical things happened.

* 011

The `diagonal()` and `fliplr()` functions are utilized in my python code. Remember to handle the opposite diagonals.

* 012

Using the prime factorization is the key.

* 013

In fact, only the first 11 or 12 digits of each number are needed if only the first 10 digits of the final answer are required.

Some people say that, in LISP, amazing things happens when solve this problem with only one "+" operator.

And it turns out that, for 10 digits precision one could use `double` typed values instead of integers. Because for today's PC, the `double` value has 16 significant digits.

* 014

The method which utilizes the Hash table is much faster than the naive implementation.

* 015

The theoretical solution is based on the knowledge of Pascal's triangle and the theory of permutation and combination. I did not quite get the idea of the theoretical solution. However, one guy named RudyPenteado gave an illustrative interpretation using binary codes.

* 016

Today's dynamic or scripting programming language is so powerful that there is virtually no upper limit for integer.

However, I composed another piece of code. This code deliberately does not use the big integer functionality of Python. The long_integer class created in PEID 013 is borrowed. Since PEID 016 only require multiplication by 2, only the addition operation is needed.

* 017

I have to admit that this problem is not about mathematics and programming. It is all about the different usages of the word "and" between British English and  American English. Furthermore, make sure to spell-check your words, like "forty".

* 018

The code also works for PEID 067.

The key is to search from the bottom to the top.

I build a binary tree to do the recursive search. A flag should be set to indicate whether the current node has found a maximum sum.

* 019

Python provides a package with the name of `datetime`. This package should do the trick.  

But I decided to do it by myself. 

And it turns out that 1200/7 is a fairly accurate approximation of this problem. Brilliant!

* 020

Everybody seems to rely on some sort of big-number library.

* 021

It is best to search below sqrt(n) when trying to calculate all the proper divisors of an integer n.

Make sure to check the following two situations:
(1) The two numbers in a pair are identical. And,
(2) Whether [a, b] and [b, a] are treated as different pairs.

* 022

First of all, the knowledge of ASCII codes is a plus. (ord() and chr() functions)

Benefit from list.sort() function of Python.

Then no magical things happened.

* 023

Not as fast as those posted on the discussion thread of Project Euler.

# NOTE:

The cover image is obtained from the home page of [Project Euler][web_pe].

[web_repo]: https://github.com/huyaoyu/ProjectEuler
[web_pe]: https://projecteuler.net/

