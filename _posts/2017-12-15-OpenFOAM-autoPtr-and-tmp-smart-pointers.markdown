---
layout: post
title:  "OpenFOAM: The autoPtr and tmp smart pointers"
date:   2017-12-15 22:23:00 +0800
categories: technical
cover_image: "Resources/Posts/OpenFOAM/001-autoptr-and-tmp/OpenFOAM-autoPtr-and-tmp.png"
tag: OpenFOAM
---

# Introduction

I was learning to program with OpenFOAM these days. This post is my notes on the smart pointers of OpenFOAM. The source code is originally created by Tomislav Maric, Jens Hoepken, and Kyle Mooney. You can find their emails in the source code. I made some changes in order to show more information that concerns me.

OpenFOAM provides two types of "smart pointer". They are `autoPtr` and `tmp`. Before I learned these smart pointers, I was thinking that they will work like the smart pointer in `boost`. It turns out that I was wrong.

After digging into the source code, I got a new understanding of `autoPtr`  and `tmp`.

# autoPtr

It seems that `autoPtr` is designed to be used when you want a temporary object, which is created by the `new` operation, that gets destroyed automatically when the program goes out of a certain scope. It does NOT have a reference counter and is different from the smart pointer in `boost`. `autoPtr` has such a unique behavior that when you assign the value of one `autoPtr` to another or create a new `autoPtr` by the copy construction function, the reference to the actual raw pointer in the original `autoPtr` is released. That is to say, the ownership of the raw c++ pointer is transferred between `autoPtr`s.

To be honest I was not able to figure out why we have to design `autoPtr` like this. After searching on Google, it seems that `autoPtr` is for the purpose of dynamics data manipulation. I have not yet got that far with OpenFOAM. 

# tmp

`tmp` is more interesting. `tmp` provides the function of reference count by the `refCount` class. However, you can only raise the number of reference to a raw C++ pointer up to 2. The following commented codes will cause an exception raised by OpenFOAM when the user is attempting to refer to a raw pointer for the third time.

{% highlight C++ %}

// Showing reference counts.
tmp<infoScalarField> t1(new infoScalarField(1e06, 0, "if02")); 
Info << "reference count = " << t1->count() << endl; 
{
    tmp<infoScalarField> t2 (t1); 
    Info << "reference count = " << t1->count() << endl; 
    {
        // This will cause an exception since tmp doesn't allow
        // reusing an object for more than two times.
        // tmp<infoScalarField> t3(t2); 
        // Info << "reference count = " << t1->count() << endl; 
    } // t3 destructor called
    
    Info << "reference count = " << t1->count() << endl; 
} // t2 destructor called
Info << "reference count = " << t1->count() << endl; 

{% endhighlight %}

I was totally confused when I was studying the source code of `tmp`. Then I found a good explanation on [OpenFOAM Wiki][OpenFOAMWikiWebsite]. `tmp` is designed for handling a large amount of memory between function calls. That is, you could use `tmp` to pass objects to a function as an argument, or get the return value from a function, with the object holding a large amount of memory. During the above operation, no significant memory copy occurs. And you do not need more than two references to a raw C++ pointer when invoking a function or returning from a function.

[OpenFOAMWikiWebsite]: http://openfoamwiki.net/index.php/OpenFOAM_guide/tmp

[OpenFOAM Wiki][OpenFOAMWikiWebsite] lists a sample code to illustrate the usage of `tmp`. I just copy it here.

{% highlight C++ %}

// Original:
largeClass myFunc ()
    {
        largeClass returnMe(new largeClass);
        // Somehow fill the data in returnMe
        return returnMe;
    }
 
// Becomes:
tmp<largeClass> myFunc()
    {
        tmp<largeClass> tReturnMe(new largeClass);
        largeClass& returnMe = tReturnMe();
        // Somehow fill the data in returnMe
        return tReturnMe;
    }

{% endhighlight %}

# Other C++ stuff

My C++ knowledge still needs improvements. When studying the code provided by Tomislav Maric, et al. I learned that the copy constructor, not the assignment operator (=), will be called when an object is created at initialization. To be specific, I was talking about the line of `Type temp = t;` in the following code.

{% highlight C++ %}

// The argument t doesn't require a copy operation since it is
// a reference to a const Type object.
template<typename Type>
Type valueReturn(Type const & t)
{
    Info << "Inside valueReturn." << endl;

    // One copy construction for the temporary.
    Type temp = t; 

    // One explicit assignment.
    temp = t;

    // ... operations (e.g. interpolation) on the temporary variable. 
    
    return temp; 
}

{% endhighlight %}

# Acknowledgment

The cover image of this post is copied from the website of [OpenFOAM][OpenFOAMWebsite]. And I modified the words on the original image. The image may be the property of The OpenFOAM Foundation. 

[OpenFOAMWebsite]: https://openfoam.org/

# The source code

The source code could be found on my [github repo][GithubRepo]. And the output is like this.

[GithubRepo]: https://github.com/huyaoyu/LearnProgrammingOpenFOAM/tree/master/testSmartPointer

<p><img src="{{site.baseurl}}/Resources/Posts/OpenFOAM/001-autoptr-and-tmp/output-of-testSmartPointers.png" alt="<img>The output of the code."></p>
