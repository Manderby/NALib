Quick Start:
------------

- Download whole package.
- HelloWorld examples are in proj folder.
- To compile your own code, there are two ways:
  1. Compile NALib as part of your own solution by simply dragging the whole
     code\NALib\src folder into your project.
  2. Create a static library and reference it. VisualStudio project is in the
     code\NALib folder. XCode project is in proj folder.



Description:
------------

NALib is a collection of source code written in the C programming language
which contain code like system-dependent macros, high-performance functions
and container structs, a small runtime system with memory pools and optional
reference counting, a GUI implementation for Mac and Windows and many helpful
comments.

Email: nalib@manderc.com
Follow on Twitter for latest developments: @Manderby52

- API is documented directly in the .h files.
- Some design principles can be read in the doc/documentation.txt file.
- NALib just needs the standard libraries.
- Compilation of the whole collection usually takes less than 5 seconds.
- The master branch contains the latest developments.
- Older release versions are available as tags.



Contribute & License:
---------------------
This project has till now not been designed with a community in mind, as it
has been used as a private engineering code collection. But input is always
welcome. Write Issues on GitHub or write Emails to nalib@manderc.com.

License: Public Domain



History:
--------

NALib originally started as a C++ helper code collection back in 2004 when I
was working for an electric engineering company and was enhanced with many
more helper snipplets by me over the following years during my research in a
media technology institute. It allowed me and others to quickly implement our
models, algorithms and data visualizations.

In 2009, I collected all the snipplets together and formed a first combined
version, known as MBLib. That version continued to be in use in the research
lab for another three years and allowed me to get started with projects when
I did found my own company Manderim in 2011.

There, I used MBLib in projects for the consumer market and for the industry
where it had been continuously verified in selling products. But as the
field I was working in often times required implementations which work
universally, I started to translate the whole library to plain C in 2013.
This new library was then called NALib.

NALib is short for "Not A Library". This name was chosen with the intention of
it to be primarily used as source code, not as compiled libraries. The library
was implemented so that it would build very fast but universally on multiple
systems. With that, I had to spend less time on library compatibility issues.

NALib was open to the public since the first version. After about 2 years,
all crucial functionality of MBLib was translated to NALib. Therefore I
discontinued development of MBLib and instead started to use NALib for
all projects.

Now, after some more years, the code collection did grow by quite a fair
amount of code and functionality. Unfortunately, I had to close down my
company in 2019, but still to this day, I am using NALib for my private
projects and try to advance it whenever time fits. It currently uses C99
as well as some elements of C11 combined with a modern coding style which
- hopefully - helps people value the C programming language as much as I do.
