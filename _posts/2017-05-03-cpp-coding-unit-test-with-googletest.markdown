---
layout: post
title:  "C++ Coding: Unit Test with Google Test"
date:   2017-05-03 23:00:00 +0800
categories: technical
cover_image: "Resources/Posts/GoogleTest/RunGoogleTestInTerminal.png"
---

Google Test is Google's C++ test framework. For the projects where C++ is adopted as the primary coding language, I use Google Test to do my unit tests. You can find Google Test from its [official web site][gtest_web_site]. Google Test is an open source package.

Here I provide a simple working example of unit test based on Google Test. For simplicity, the Eclipse is chosen as the IDE and the program is compiled and runs on CentOS 6 operating system.

In this example the following aspects of Google Test are covered:

* Defining test case class.
* Adding unit test function for public member function of a sample class.
* Adding unit test function for private/protected member function of a sample class.
* Implementing unit test functions, using assertions that provided by Google Test.
* Compiling and linking application against Google Test.
* Running the unit test.

# Defining test case class #

The following example focuses on unit testing with respect to class.

It suggested by Google Test that a specific purpose "test case class" should be defined in order to conduct the unit test. The following list shows a simple test case class inherited from `::tesing::Test` class provided by Google Test. Be sure to include the Google Test's header file which is gtest/gtest.h.

{% highlight c++ linenos %}
class TestBase : public ::testing::Test
{
public:
	TestBase(void);
	virtual ~TestBase(void);
	static void SetUpTestCase();
	static void TearDownTestCase();
	virtual void SetUp(void);
	virtual void TearDown(void);


public:
	static int mArgc;
	static char** mArgs;

	static SampleClass* pSC;
};
{% endhighlight %}

The above code is saved in the header file of `TestBase` class. On Line 16 `SampleClass` is a class defined outside this header file, and is shown in the next section.

4 member functions are overloaded: `SetUpTestCase()`, `TearDownTestCase()`, `SetUp()`, and `TearDown()`. `SetUpTestCase()` is invoked before any test defined in `TestBase` is taken. It serves as preparation operation. `TearDownTestCase()` will be called after all the tests of `TestBase` are performed. It should clean up all the resources shared by the tests of `TestBase`. Note that both `SetUpTestCase()` and `TearDownTestCase()` are static member functions. `SetUp()` and `TearDown()` are called before and after each individual test defined in `TestBase`.

The implementation of class `TestBase` is very straight forward. In TestBase.cpp file:

{% highlight c++ linenos %}

int TestBase::mArgc = 0;
char** TestBase::mArgs = NULL;

SampleClass* TestBase::pSC = NULL;

TestBase::TestBase(void)
{

}

TestBase::~TestBase(void)
{

}

void TestBase::SetUpTestCase()
{
	std::cout << "Set up test cases." << std::endl;

	if ( pSC != NULL )
	{
		delete pSC;
	}

	pSC = new SampleClass;
}

void TestBase::TearDownTestCase()
{
	std::cout << "Tear down test cases." << std::endl;

	delete pSC; pSC = NULL;
}

void TestBase::SetUp(void)
{

}

void TestBase::TearDown(void)
{

}

{% endhighlight %}

The definition of class `SampleClass` and the implementation of the unit test functions are detailed in the following sections.

# Adding unit test functions #

In order to get access to the private member functions, the unit test functions are designed to be friend functions of class `SampleClass`. In SampleClass.hpp, the header file of class `SampleClass`, after including header TestBase.hpp, the definition of class `SampleClass` is as follows.

{% highlight c++ linenos %}

class SampleClass
{
public:
	SampleClass(void);
	~SampleClass(void);

public:
	int check_positive(int v);

private:
	int check_negative(int v);

public:
	FRIEND_TEST(TestBase, test_check_positive);
	FRIEND_TEST(TestBase, test_check_negative);
	FRIEND_TEST(TestBase, test_check_negative_fail);
};

{% endhighlight %}

It is clear that there are two member functions defined inside class `SampleClass`, they are `check_positive()` and `check_negative()`. These two members are extremely simple that they merely check if the input argument, `v`, is positive or negative. Their implementation codes are in the source file SampleClass.cpp. It is worth to be noted that the unit test functions are declared as public friend functions using macros provided by Google Test, `FRIEND_TEST`. The first argument of `FRIEND_TEST` is the test case class name, `TestBase`. The second is the name of the test function. The test functions could be defined anywhere you want them to be, just make sure that TestBase.hpp is properly included.

# Implementation of unit test functions #

The codes of the unit test functions are listed below.

{% highlight c++ linenos %}
TEST_F(TestBase, test_check_positive)
{
	int v = 1;
	int res = 0;
	int res_expected = 1;

	res = pSC->check_positive(v);

	ASSERT_EQ( res, res_expected );
}

TEST_F(TestBase, test_check_negative)
{
	int v = -1;
	int res = 0;
	int res_expected = 1;

	res = pSC->check_negative(v);

	ASSERT_EQ( res, res_expected );
}

TEST_F(TestBase, test_check_negative_fail)
{
	int v = -1;
	int res = 0;
	int res_expected = 0;

	res = pSC->check_negative(v);

	ASSERT_EQ( res, res_expected );
}
{% endhighlight %}

Use macro `TEST_F` when implementing a Google Test test function. Note that the last of the three test functions is deliberately designed to fail the test.

Here we could see that Google Test uses assertions like `ASSERT_EQ` to verify if an expected condition is met. Google Test predefines lots of such handy assertion routines. You can find them in the documentation of Google Test.

# Compiling, linking and running #

Of course, you have to "configure and make" Google Test libraries before linking to it.

Since I use Eclipse to demonstrate the usage of Google Test, some setting should be tweaked before compiling and linking an c++ application against Google Test. You have to add search path for header files of Google Test. Set options to tell Eclipse where to find the library, libgtest.a. When linking your application, remember to issue `-lgtest` and `-lpthread` flags. 

The last thing you may encountered is that you may have to set the running environment for your application. This is accomplished by referring the following figure.

<p><img src="{{site.baseurl}}/Resources/Posts/GoogleTest/SetEnvironmentForGoogleTest_Annotated.png" alt="<img>Setting." width="800px"></p>

In the above figure, we have to configure to append the path of Google Test's library to the system `LD_LIBRARY_PATH` environment variable.

I prepared a simple main.cpp file.

<p><img src="{{site.baseurl}}/Resources/Posts/GoogleTest/GoogleTestAndEclipse.png" alt="<img>Eclipse." width="800px"></p>

In the above main.cpp file, `::tesing::IniGoogleTest()` and `RUN_ALL_TEST()` are Google Test's APIs. After successfully compiling and linking, we could run this test case. In the console view of Eclipse, and the above figure, we can see that every unit test is invoked and a summary will be shown at the end. Here we prepared two success tests and one failed test. If you run the application in a terminal, then you will get colored text outputs, as illustrated in the following figure.

<p><img src="{{site.baseurl}}/Resources/Posts/GoogleTest/RunGoogleTestInTerminal.png" alt="<img>Run Google Test in Terminal." width="400px"></p>

# Some further topics #

This post only covers a fraction of the usage of Google Test. I hope this will help people who are not familiar with it. Google Test provides additional functionalities to perform test. You can test functions that are not the member of a class, you can also test template class members with "TYPED" macros of Google Test. I will not talk about those topics here.

The source files I used could be obtained here [(zip)]({{site.baseurl}}/Resources/Posts/GoogleTest/GTest.zip).

[gtest_web_site]: https://github.com/google/googletest
